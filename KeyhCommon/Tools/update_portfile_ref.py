from __future__ import annotations

import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path


VERSION_FIELDS = ("version", "version-semver", "version-date", "version-string")


def read_json(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, value: dict) -> None:
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


def detect_text_encoding(data: bytes) -> tuple[str, bool]:
    if data.startswith(b"\xef\xbb\xbf"):
        return "utf-8-sig", True
    if data.startswith(b"\xff\xfe"):
        return "utf-16-le", True
    if data.startswith(b"\xfe\xff"):
        return "utf-16-be", True

    try:
        if data.decode("utf-8").encode("utf-8") == data:
            return "utf-8", True
    except UnicodeDecodeError:
        pass

    return "latin-1", False


def get_version_field_info(value: dict) -> tuple[str, str] | None:
    for field_name in VERSION_FIELDS:
        if field_name in value:
            field_value = str(value[field_name]).strip()
            if field_value:
                return field_name, field_value
    return None


def has_default_port_version(value: dict) -> bool:
    return "port-version" not in value or value.get("port-version") in (0, "0", None)


def run_git(repo_root: Path, *args: str, env: dict[str, str] | None = None, check: bool = True) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["git", "-C", str(repo_root), *args],
        check=check,
        text=True,
        capture_output=True,
        env=env,
    )


def hash_blob(repo_root: Path, content: bytes, path_in_repo: str, env: dict[str, str]) -> str:
    result = subprocess.run(
        ["git", "-C", str(repo_root), "hash-object", "-w", "--stdin", "--path", path_in_repo],
        check=True,
        input=content,
        capture_output=True,
        env=env,
    )
    blob_hash = result.stdout.decode("utf-8").strip()
    if not re.fullmatch(r"[0-9A-Fa-f]{40}", blob_hash):
        raise RuntimeError(f"Unable to hash synthetic file contents for {path_in_repo}.")
    return blob_hash


def get_port_tree_hash(repo_root: Path, port_dir: Path, overrides: dict[str, bytes] | None = None) -> str:
    if not any(path.is_file() for path in port_dir.rglob("*")):
        raise RuntimeError("No files were found under ports/keyhcommon.")

    temp_index_fd, temp_index_path = tempfile.mkstemp()
    os.close(temp_index_fd)
    os.unlink(temp_index_path)

    env = os.environ.copy()
    env["GIT_INDEX_FILE"] = temp_index_path

    try:
        run_git(repo_root, "read-tree", "--empty", env=env)
        run_git(repo_root, "read-tree", "--empty", env=env)
        run_git(repo_root, "add", "--all", "--force", "--", "ports/keyhcommon", env=env)
            normalized_relative_path = relative_path.replace("\\", "/")
            path_in_repo = f"ports/keyhcommon/{normalized_relative_path}"
            blob_hash = hash_blob(repo_root, content, path_in_repo, env)
            run_git(
                repo_root,
                "update-index",
                "--cacheinfo",
                f"100644,{blob_hash},{path_in_repo}",
                env=env,
            )

        root_tree_hash = run_git(repo_root, "write-tree", env=env).stdout.strip()
        if not re.fullmatch(r"[0-9A-Fa-f]{40}", root_tree_hash):
            raise RuntimeError("Unable to compute git tree hash for the temporary index.")

        tree_entry = run_git(repo_root, "ls-tree", root_tree_hash, "--", "ports/keyhcommon", env=env).stdout.strip()
        match = re.match(r"^[0-9]{6}\s+tree\s+([0-9A-Fa-f]{40})\t", tree_entry)
        if not match:
            raise RuntimeError("Unable to parse the ports/keyhcommon tree hash.")

        return match.group(1)
    finally:
        try:
            os.remove(temp_index_path)
        except FileNotFoundError:
            pass


def main() -> int:
    script_dir = Path(__file__).resolve().parent
    repo_root = script_dir.parent.parent
    port_dir = repo_root / "ports" / "keyhcommon"
    portfile_path = port_dir / "portfile.cmake"
    vcpkg_json_path = port_dir / "vcpkg.json"
    versions_path = repo_root / "versions" / "k-" / "keyhcommon.json"
    baseline_path = repo_root / "versions" / "baseline.json"

    for required_path in (portfile_path, vcpkg_json_path, versions_path, baseline_path):
        if not required_path.exists():
            raise FileNotFoundError(f"Required file not found: {required_path}")

    head_result = run_git(repo_root, "rev-parse", "--verify", "HEAD", check=False)
    head_sha = head_result.stdout.strip()
    if head_result.returncode != 0 or not re.fullmatch(r"[0-9A-Fa-f]{40}", head_sha):
        print("[vcpkg registry] Skip: repository HEAD commit is unavailable or invalid.")
        return 0

    portfile_bytes = portfile_path.read_bytes()
    portfile_encoding, use_direct_text_encoding = detect_text_encoding(portfile_bytes)
    updated_portfile_bytes: bytes | None = None

    if use_direct_text_encoding:
        portfile_content = portfile_bytes.decode(portfile_encoding)
        ref_regex = re.compile(r'(?m)^(\s*REF\s+")([0-9A-Fa-f]{40})("\s*)$')
        ref_match = ref_regex.search(portfile_content)
        if ref_match is None:
            raise RuntimeError("REF line not found in portfile.")

        current_ref = ref_match.group(2)
        if current_ref.lower() != head_sha.lower():
            updated_portfile_content = ref_regex.sub(
                lambda match: f"{match.group(1)}{head_sha}{match.group(3)}",
                portfile_content,
                count=1,
            )
            updated_portfile_bytes = updated_portfile_content.encode(portfile_encoding)
    else:
        ref_regex = re.compile(rb'(?m)^(\s*REF\s+")([0-9A-Fa-f]{40})("\s*)$')
        ref_match = ref_regex.search(portfile_bytes)
        if ref_match is None:
            raise RuntimeError("REF line not found in portfile.")

        current_ref = ref_match.group(2).decode("ascii")
        if current_ref.lower() != head_sha.lower():
            updated_portfile_bytes = ref_regex.sub(
                lambda match: match.group(1) + head_sha.encode("ascii") + match.group(3),
                portfile_bytes,
                count=1,
            )

    manifest = read_json(vcpkg_json_path)
    version_field = get_version_field_info(manifest)
    if version_field is None:
        raise RuntimeError("A supported version field is missing from ports/keyhcommon/vcpkg.json.")

    version_field_name, version_value = version_field
    manifest_has_port_version = "port-version" in manifest
    port_version_value = manifest.get("port-version")

    tree_hash = get_port_tree_hash(
        repo_root,
        port_dir,
        {"portfile.cmake": updated_portfile_bytes} if updated_portfile_bytes is not None else None,
    )

    if updated_portfile_bytes is not None:
        portfile_path.write_bytes(updated_portfile_bytes)

    versions_document = read_json(versions_path)
    existing_versions = versions_document.get("versions", [])
    filtered_entries: list[dict] = []
    matching_entry: dict | None = None
    matching_entry_index = -1
    matching_entry_count = 0
    for index, entry in enumerate(existing_versions):
        entry_version_field = get_version_field_info(entry)
        same_port_version = entry.get("port-version") == port_version_value if manifest_has_port_version else has_default_port_version(entry)
        if (
            entry_version_field is not None
            and entry_version_field[0] == version_field_name
            and entry_version_field[1] == version_value
            and same_port_version
        ):
            matching_entry_count += 1
            if matching_entry is None:
                matching_entry = entry
                matching_entry_index = index
            continue
        filtered_entries.append(entry)

    updated_entry: dict[str, object] = {
        "git-tree": tree_hash,
        version_field_name: version_value,
    }
    if manifest_has_port_version:
        updated_entry["port-version"] = port_version_value

    updated_versions_document = {key: value for key, value in versions_document.items() if key != "versions"}
    updated_versions_document["versions"] = [updated_entry, *filtered_entries]
    if not (matching_entry_count == 1 and matching_entry_index == 0 and matching_entry == updated_entry):
        write_json(versions_path, updated_versions_document)

    baseline_document = read_json(baseline_path)
    default_entries = dict(baseline_document.get("default", {}))
    keyhcommon_baseline_entry = dict(default_entries.get("keyhcommon", {}))
    keyhcommon_baseline_entry["baseline"] = version_value
    if manifest_has_port_version:
        keyhcommon_baseline_entry["port-version"] = port_version_value
    else:
        keyhcommon_baseline_entry.pop("port-version", None)
    default_entries["keyhcommon"] = keyhcommon_baseline_entry

    updated_baseline_document = {key: value for key, value in baseline_document.items() if key != "default"}
    updated_baseline_document["default"] = default_entries
    if updated_baseline_document != baseline_document:
        write_json(baseline_path, updated_baseline_document)

    if current_ref.lower() == head_sha.lower():
        print(f"[vcpkg registry] REF already up to date: {head_sha}")
    else:
        print(f"[vcpkg registry] Updated REF: {current_ref} -> {head_sha}")

    version_label = version_value if not manifest_has_port_version else f"{version_value}#{port_version_value}"
    print(f"[vcpkg registry] Synced versions/k-/keyhcommon.json to git-tree {tree_hash} for version {version_label}")
    print(f"[vcpkg registry] Synced versions/baseline.json to version {version_label}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"[vcpkg registry] Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
