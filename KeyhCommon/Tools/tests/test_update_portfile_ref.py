"""Integration regressions for the vcpkg registry updater (standard library only)."""

import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import unittest


UPDATER = Path(__file__).resolve().parents[1] / "update_portfile_ref.py"


class UpdatePortfileRefTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="registry-tests-")
        self.addCleanup(self.temporary.cleanup)
        self.area = Path(self.temporary.name)
        self.repo = self.area / "repo"
        self.repo.mkdir()
        self.env = {
            key: value for key, value in os.environ.items()
            if not key.startswith("GIT_")
        }
        self.env.update({
            "GIT_CONFIG_NOSYSTEM": "1",
            "GIT_CONFIG_GLOBAL": os.devnull,
            "GIT_TERMINAL_PROMPT": "0",
            "TMPDIR": str(self.area),
            "TMP": str(self.area),
            "TEMP": str(self.area),
        })
        self.init_repository(self.repo)
        self.script = self.repo / "KeyhCommon/Tools/update_portfile_ref.py"
        self.script.parent.mkdir(parents=True)
        shutil.copyfile(UPDATER, self.script)
        (self.repo / "KeyhCommon/source.h").write_text(
            "// initial source\n", encoding="utf-8"
        )
        (self.repo / "LICENSE").write_text("Fixture license\n", encoding="utf-8")
        self.source_commit = self.commit("Bootstrap source")

        self.portfile = self.repo / "ports/keyhcommon/portfile.cmake"
        self.manifest = self.repo / "ports/keyhcommon/vcpkg.json"
        self.versions = self.repo / "versions/k-/keyhcommon.json"
        self.baseline = self.repo / "versions/baseline.json"
        self.portfile.parent.mkdir(parents=True)
        self.versions.parent.mkdir(parents=True)
        self.portfile.write_text(
            'vcpkg_from_github(\n'
            '    OUT_SOURCE_PATH SOURCE_PATH\n'
            '    REPO example/keyhcommon\n'
            f'    REF "{self.source_commit}"\n'
            ')\n',
            encoding="utf-8",
        )
        self.write_json(self.manifest, {
            "name": "keyhcommon", "version-string": "0.1.0"
        })
        self.git("add", "ports")
        port_tree = self.staged_port_tree()
        self.write_json(self.versions, {
            "versions": [{"version-string": "0.1.0", "git-tree": port_tree}]
        })
        self.write_json(self.baseline, {
            "default": {
                "keyhcommon": {"baseline": "0.1.0", "port-version": 0},
                "unrelated": {"baseline": "2.0.0", "port-version": 3},
            }
        })
        self.registry_commit = self.commit("Register initial port")
        self.history = self.read_json(self.versions)["versions"]

    def git(self, *args, repo=None):
        return subprocess.run(
            ["git", "-C", str(repo or self.repo), *args],
            env=self.env, text=True, capture_output=True, check=True,
        ).stdout.strip()

    def init_repository(self, path):
        self.git("init", "--quiet", repo=path)
        self.git("config", "user.name", "Registry test", repo=path)
        self.git("config", "user.email", "registry-test@example.invalid", repo=path)
        self.git("config", "commit.gpgsign", "false", repo=path)
        self.git("config", "core.autocrlf", "false", repo=path)

    def commit(self, message):
        self.git("add", "--all")
        self.git("commit", "--quiet", "-m", message)
        return self.git("rev-parse", "HEAD")

    @staticmethod
    def read_json(path):
        return json.loads(path.read_text(encoding="utf-8"))

    @staticmethod
    def write_json(path, document):
        path.write_text(json.dumps(document, indent=2) + "\n", encoding="utf-8")

    def metadata(self, root=None):
        root = root or self.repo
        return {
            name: (root / name).read_bytes()
            for name in (
                "ports/keyhcommon/portfile.cmake",
                "ports/keyhcommon/vcpkg.json",
                "versions/k-/keyhcommon.json",
                "versions/baseline.json",
            )
        }

    def run_updater(self, root=None, success=True):
        root = root or self.repo
        index = root / ".git/index"
        before = index.read_bytes() if index.exists() else None
        result = subprocess.run(
            [sys.executable, str(root / "KeyhCommon/Tools/update_portfile_ref.py")],
            cwd=root, env=self.env, text=True, capture_output=True,
        )
        self.assertEqual(
            index.read_bytes() if index.exists() else None, before,
            "Updater changed the caller's Git index",
        )
        if success:
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        else:
            self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
        return result

    def current_ref(self):
        return re.search(
            r'^\s*REF\s+"([0-9a-f]{40})"',
            self.portfile.read_text(encoding="utf-8"), re.MULTILINE,
        ).group(1)

    def staged_port_tree(self):
        tree = self.git("write-tree")
        return self.git("rev-parse", f"{tree}:ports/keyhcommon")

    def assert_registered(self, revision, history=None, version="0.1.0"):
        manifest = self.read_json(self.manifest)
        self.assertEqual(manifest["version-string"], version)
        self.assertEqual(manifest.get("port-version", 0), revision)
        entries = self.read_json(self.versions)["versions"]
        self.assertEqual(entries[1:], self.history if history is None else history)
        self.assertEqual(entries[0]["version-string"], version)
        self.assertEqual(entries[0].get("port-version", 0), revision)
        baseline = self.read_json(self.baseline)["default"]
        self.assertEqual(baseline["keyhcommon"]["baseline"], version)
        self.assertEqual(baseline["keyhcommon"].get("port-version", 0), revision)
        self.assertEqual(
            baseline["unrelated"], {"baseline": "2.0.0", "port-version": 3}
        )
        index = self.repo / ".git/index"
        original_index = index.read_bytes()
        try:
            self.git("add", "--all", "--", "ports/keyhcommon")
            self.assertEqual(entries[0]["git-tree"], self.staged_port_tree())
        finally:
            index.write_bytes(original_index)

    def change_source(self, path="KeyhCommon/source.h"):
        with (self.repo / path).open("a", encoding="utf-8") as stream:
            stream.write("// committed source change\n")
        return self.commit("Change source")

    def change_port(self, text="# changed packaging\n"):
        with self.portfile.open("a", encoding="utf-8") as stream:
            stream.write(text)

    def assert_rejected(self, root=None):
        before = self.metadata(root)
        result = self.run_updater(root, success=False)
        self.assertEqual(self.metadata(root), before)
        self.assertIn("[vcpkg registry] Error:", result.stderr)

    def test_unchanged_registry_and_metadata_only_commit_are_noops(self):
        before = self.metadata()
        self.run_updater()
        self.assertEqual(self.metadata(), before)
        (self.repo / "notes.txt").write_text("Metadata only\n", encoding="utf-8")
        self.commit("Metadata only")
        self.run_updater()
        self.assertEqual(self.metadata(), before)
        self.assertEqual(self.current_ref(), self.source_commit)

    def test_committed_source_change_updates_ref_and_adds_revision(self):
        head = self.change_source()
        self.run_updater()
        self.assertEqual(self.current_ref(), head)
        self.assert_registered(1)

    def test_committed_license_change_updates_ref(self):
        head = self.change_source("LICENSE")
        self.run_updater()
        self.assertEqual(self.current_ref(), head)
        self.assert_registered(1)

    def test_dirty_source_and_license_fail_without_writes(self):
        for path in ("KeyhCommon/source.h", "LICENSE"):
            for staged in (False, True):
                with self.subTest(path=path, staged=staged):
                    self.git("reset", "--hard", self.registry_commit)
                    with (self.repo / path).open("a", encoding="utf-8") as stream:
                        stream.write("Uncommitted change\n")
                    if staged:
                        self.git("add", "--", path)
                    self.assert_rejected()

    def test_untracked_source_fails_without_writes(self):
        (self.repo / "KeyhCommon/untracked.h").write_text(
            "// untracked\n", encoding="utf-8"
        )
        self.assert_rejected()

    def test_untracked_license_fails_without_writes(self):
        self.git("rm", "LICENSE")
        self.commit("Remove license")
        (self.repo / "LICENSE").write_text("Untracked license\n", encoding="utf-8")
        self.assert_rejected()

    def test_dirty_source_fails_even_with_pending_port_edits(self):
        self.change_port()
        (self.repo / "KeyhCommon/source.h").write_text(
            "// dirty source\n", encoding="utf-8"
        )
        self.assert_rejected()

    def test_port_edit_bumps_revision_without_changing_source_ref(self):
        self.change_port()
        self.run_updater()
        self.assertEqual(self.current_ref(), self.source_commit)
        self.assert_registered(1)

    def test_repeated_pending_changes_reuse_revision_and_committed_history(self):
        self.change_port()
        self.run_updater()
        self.assert_registered(1)
        first_tree = self.read_json(self.versions)["versions"][0]["git-tree"]
        first_files = self.metadata()
        self.run_updater()
        self.assertEqual(self.metadata(), first_files)

        self.change_port("# another packaging change\n")
        self.run_updater()
        self.assert_registered(1)
        self.assertNotEqual(
            self.read_json(self.versions)["versions"][0]["git-tree"], first_tree
        )
        self.change_source()
        # change_source commits the pending metadata as well, making revision 1
        # immutable before the subsequent source update is registered.
        committed_history = self.read_json(self.versions)["versions"]
        self.run_updater()
        self.assert_registered(2, history=committed_history)

    def test_new_source_commits_reuse_uncommitted_pending_revision(self):
        self.change_source()
        self.run_updater()
        first_tree = self.read_json(self.versions)["versions"][0]["git-tree"]
        with (self.repo / "KeyhCommon/source.h").open("a", encoding="utf-8") as stream:
            stream.write("// next source commit\n")
        self.git("add", "KeyhCommon/source.h")
        self.git("commit", "--quiet", "-m", "Only source, leave pending registry")
        head = self.git("rev-parse", "HEAD")
        self.run_updater()
        self.assertEqual(self.current_ref(), head)
        self.assert_registered(1)
        self.assertNotEqual(
            self.read_json(self.versions)["versions"][0]["git-tree"], first_tree
        )

    def test_committing_generated_metadata_does_not_cause_another_update(self):
        self.change_source()
        self.run_updater()
        self.assert_registered(1)
        before = self.metadata()
        self.commit("Commit generated registry metadata")
        self.run_updater()
        self.assertEqual(self.metadata(), before)
        self.change_port()
        history = self.read_json(self.versions)["versions"]
        self.run_updater()
        self.assert_registered(2, history=history)

    def test_explicit_upstream_version_starts_at_revision_zero(self):
        manifest = self.read_json(self.manifest)
        manifest["version-string"] = "0.2.0"
        manifest["port-version"] = 0
        self.write_json(self.manifest, manifest)
        self.run_updater()
        self.assert_registered(0, version="0.2.0")
        before = self.metadata()
        self.run_updater()
        self.assertEqual(self.metadata(), before)

    def test_explicit_revision_increment_is_respected(self):
        manifest = self.read_json(self.manifest)
        manifest["port-version"] = 4
        self.write_json(self.manifest, manifest)
        self.run_updater()
        self.assert_registered(4)

    def test_automatic_revision_exceeds_highest_committed_revision(self):
        manifest = self.read_json(self.manifest)
        manifest["port-version"] = 7
        self.write_json(self.manifest, manifest)
        self.git("add", "ports/keyhcommon")
        old_tree = self.staged_port_tree()
        manifest.pop("port-version")
        self.write_json(self.manifest, manifest)
        history = [
            {"version-string": "0.1.0", "port-version": 7, "git-tree": old_tree},
            *self.history,
        ]
        self.write_json(self.versions, {"versions": history})
        self.commit("Record previous higher revision")
        self.change_port()
        self.run_updater()
        self.assert_registered(8, history=history)

    def test_uncommitted_history_entries_are_regenerated_from_head(self):
        self.change_port()
        document = self.read_json(self.versions)
        document["versions"].insert(0, {
            "version-string": "0.1.0", "port-version": 99, "git-tree": "f" * 40
        })
        self.write_json(self.versions, document)
        self.run_updater()
        self.assert_registered(1)

    def test_preexisting_staged_port_and_unrelated_changes_leave_index_untouched(self):
        self.change_port("# staged port edit\n")
        (self.repo / "notes.txt").write_text("Staged notes\n", encoding="utf-8")
        self.git("add", "ports/keyhcommon", "notes.txt")
        self.change_port("# unstaged port edit\n")
        before = self.git("diff", "--cached", "--binary")
        self.run_updater()
        self.assertEqual(self.git("diff", "--cached", "--binary"), before)
        self.assert_registered(1)

    def test_corrupt_json_fails_before_any_metadata_write(self):
        self.change_source()
        for path in (self.manifest, self.versions, self.baseline):
            with self.subTest(path=path.relative_to(self.repo)):
                original = path.read_bytes()
                try:
                    path.write_text("{not valid json\n", encoding="utf-8")
                    before = self.metadata()
                    self.run_updater(success=False)
                    self.assertEqual(self.metadata(), before)
                finally:
                    path.write_bytes(original)

    def test_portfile_encoding_is_preserved_when_updating_ref(self):
        for encoding, bom in (
            ("latin-1", b""),
            ("utf-16-le", b"\xff\xfe"),
            ("utf-16-be", b"\xfe\xff"),
        ):
            with self.subTest(encoding=encoding):
                self.git("reset", "--hard", self.registry_commit)
                text = self.portfile.read_text(encoding="utf-8") + "# café\n"
                self.portfile.write_bytes(bom + text.encode(encoding))
                head = self.change_source()
                self.run_updater()
                self.assertEqual(
                    self.portfile.read_bytes(),
                    bom + text.replace(self.source_commit, head).encode(encoding),
                )
                self.assert_registered(1)
                before = self.metadata()
                self.run_updater()
                self.assertEqual(self.metadata(), before)

    def test_extracted_sources_outside_git_fail(self):
        extracted = self.area / "extracted"
        shutil.copytree(self.repo, extracted, ignore=shutil.ignore_patterns(".git"))
        self.assert_rejected(extracted)

    def test_extracted_sources_inside_parent_git_repository_fail(self):
        self.init_repository(self.area)
        self.git("commit", "--quiet", "--allow-empty", "-m", "Parent", repo=self.area)
        extracted = self.area / "extracted"
        shutil.copytree(self.repo, extracted, ignore=shutil.ignore_patterns(".git"))
        self.assert_rejected(extracted)
        self.assertEqual(self.git("ls-files", repo=self.area), "")

    def test_shallow_boundary_does_not_force_metadata_only_ref_update(self):
        (self.repo / ".git/shallow").write_text(
            self.registry_commit + "\n", encoding="ascii"
        )
        before = self.metadata()
        self.run_updater()
        self.assertEqual(self.metadata(), before)

    def test_unavailable_old_ref_can_register_current_head(self):
        self.portfile.write_text(
            self.portfile.read_text(encoding="utf-8").replace(
                self.source_commit, "a" * 40
            ),
            encoding="utf-8",
        )
        self.git("add", "ports/keyhcommon")
        document = self.read_json(self.versions)
        document["versions"][0]["git-tree"] = self.staged_port_tree()
        self.write_json(self.versions, document)
        head = self.commit("Registry with unavailable old source reference")
        history = self.read_json(self.versions)["versions"]
        self.run_updater()
        self.assertEqual(self.current_ref(), head)
        self.assert_registered(1, history=history)


if __name__ == "__main__":
    unittest.main()
