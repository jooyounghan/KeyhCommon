"""Packaged reflection generator regressions (standard library only)."""

import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


REPO = Path(__file__).resolve().parents[3]
TOOLS = REPO / "KeyhCommon/Tools"
HEADER = "KEYH_REFLECT_ENUM\nenum class State { Idle, Running };\n"


@unittest.skipUnless(shutil.which("cmake"), "CMake is not installed")
class PackagedReflectCodegenTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="reflect-tests-")
        self.addCleanup(self.temporary.cleanup)
        self.area = Path(self.temporary.name)
        self.source = self.area / "source"
        source_tools = self.source / "KeyhCommon/Tools"
        source_tools.mkdir(parents=True)
        for name in ("reflect_codegen.py", "run_reflect_codegen.bat"):
            # Simulate a Git archive, not a Windows checkout with CRLF.
            (source_tools / name).write_bytes(
                (TOOLS / name).read_bytes().replace(b"\r\n", b"\n")
            )
        (self.source / "LICENSE").write_text("Fixture license\n")
        for project in ("CommonBase", "ReflectSystem", "AppSystem"):
            root = self.source / "KeyhCommon" / project
            for config in ("Release", "Debug"):
                (root / config).mkdir(parents=True)
                (root / config / f"{project}.lib").write_bytes(b"")
            (root / f"{project}.h").write_text("// Fixture header\n")
        (self.source / "KeyhCommon/CommonBase/CommonBase.natvis").write_text("")
        self.package = self.area / "installed triplet ! (test)"
        fixture = self.area / "install.cmake"
        fixture.write_text(
            'cmake_minimum_required(VERSION 3.20)\n'
            'macro(vcpkg_check_linkage)\nendmacro()\n'
            'macro(vcpkg_from_git)\nendmacro()\n'
            'macro(vcpkg_build_msbuild)\nendmacro()\n'
            f'set(SOURCE_PATH "{self.source.as_posix()}")\n'
            f'set(CURRENT_PACKAGES_DIR "{self.package.as_posix()}")\n'
            'set(PORT keyhcommon)\n'
            'set(VCPKG_TARGET_ARCHITECTURE x64)\n'
            f'include("{(REPO / "ports/keyhcommon/portfile.cmake").as_posix()}")\n',
            encoding="utf-8",
        )
        result = subprocess.run(
            ["cmake", "-P", str(fixture)], capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        shutil.rmtree(self.source)
        self.tools = self.package / "tools/keyhcommon"
        self.project = self.area / "consumer project ! (test)"
        self.header = self.project / "nested/State.h"
        self.header.parent.mkdir(parents=True)
        self.header.write_text(HEADER, encoding="utf-8")
        self.cwd = self.area / "unrelated working directory"
        self.cwd.mkdir()
        self.env = os.environ.copy()
        self.env["KEYHCOMMON_PYTHON"] = sys.executable
        self.env["PYTHONDONTWRITEBYTECODE"] = "1"

    def run_python(self, *args):
        return subprocess.run(
            [sys.executable, "-B", str(self.tools / "reflect_codegen.py"),
             "--project-dir", str(self.project), *args],
            cwd=self.cwd, env=self.env, capture_output=True, text=True,
        )

    def run_batch(self, *args):
        command = f'call "{self.tools / "run_reflect_codegen.bat"}"'
        command += "".join(f' "{arg}"' for arg in args)
        return subprocess.run(
            [os.environ.get("COMSPEC", "cmd.exe"), "/d", "/v:off", "/c", command],
            cwd=self.cwd, env=self.env, capture_output=True, text=True,
        )

    def assert_success(self, result):
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_packaged_batch_is_ascii_crlf(self):
        content = (self.tools / "run_reflect_codegen.bat").read_bytes()
        content.decode("ascii")
        self.assertIn(b"\r\n", content)
        self.assertNotIn(b"\n", content.replace(b"\r\n", b""))
        self.assertTrue(content.startswith(b"@echo off\r\n"))

    def test_external_project_and_incremental_generation(self):
        self.assert_success(self.run_python())
        generated = self.header.with_suffix(".reflect_generated.inl")
        self.assertTrue(generated.is_file())
        self.assertIn("KEYH_REFLECT_ENUM_BEGIN(State)", generated.read_text())
        self.assertIn('#include "State.reflect_generated.inl"', self.header.read_text())
        original = generated.read_bytes()
        self.assert_success(self.run_python())
        self.assertEqual(generated.read_bytes(), original)

    def test_custom_output_without_header_patching(self):
        output = self.area / "generated files"
        self.assert_success(self.run_python(
            "--output-dir", str(output), "--no-patch-headers",
        ))
        self.assertTrue((output / "State.reflect_generated.inl").is_file())
        self.assertEqual(self.header.read_text(), HEADER)

    def test_installed_dependencies_are_not_modified(self):
        dependencies = []
        for directory in ("vcpkg_installed", ".git", ".vs"):
            header = self.project / directory / "include/Dependency.h"
            header.parent.mkdir(parents=True)
            header.write_text(HEADER)
            dependencies.append(header)
        self.assert_success(self.run_python())
        for header in dependencies:
            self.assertEqual(header.read_text(), HEADER)
            self.assertFalse(header.with_suffix(".reflect_generated.inl").exists())

    def test_invalid_project_fails(self):
        result = self.run_python("--project-dir", str(self.area / "missing"))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("project directory not found", result.stderr)

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_default_output_and_trailing_separator(self):
        self.assert_success(self.run_batch(str(self.project) + "\\"))
        self.assertTrue(self.header.with_suffix(".reflect_generated.inl").is_file())
        self.assertFalse((self.project / "State.reflect_generated.inl").exists())

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_custom_output(self):
        output = self.area / "generated files ! (test)"
        self.assert_success(self.run_batch(str(self.project) + "\\", str(output) + "\\"))
        self.assertTrue((output / "State.reflect_generated.inl").is_file())

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_python_discovery(self):
        self.env.pop("KEYHCOMMON_PYTHON")
        self.env["PATH"] = str(Path(sys.executable).parent)
        self.assert_success(self.run_batch(self.project))

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_missing_python(self):
        self.env.pop("KEYHCOMMON_PYTHON")
        self.env["PATH"] = str(self.cwd)
        result = self.run_batch(self.project)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Python 3 not found", result.stdout)

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_missing_argument(self):
        result = self.run_batch()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("project directory argument is required", result.stdout)

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_invalid_python_override(self):
        self.env["KEYHCOMMON_PYTHON"] = str(self.area / "missing python.exe")
        result = self.run_batch(self.project)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("KEYHCOMMON_PYTHON must name", result.stdout)

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_propagates_exit_code(self):
        (self.tools / "reflect_codegen.py").write_text("import sys\nsys.exit(23)\n")
        result = self.run_batch(self.project)
        self.assertEqual(result.returncode, 23, result.stdout + result.stderr)
        self.assertIn("failed with error code 23", result.stdout)

    @unittest.skipUnless(os.name == "nt", "Requires Windows cmd.exe")
    def test_batch_missing_generator(self):
        (self.tools / "reflect_codegen.py").unlink()
        result = self.run_batch(self.project)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("must be installed beside", result.stdout)


if __name__ == "__main__":
    unittest.main()
