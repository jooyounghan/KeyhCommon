"""Exercise port installation with mocked source fetching and MSBuild."""

from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


REPO_ROOT = Path(__file__).resolve().parents[3]


@unittest.skipUnless(shutil.which("cmake"), "CMake is required for port installation tests")
class ToolPackagingTests(unittest.TestCase):
    def test_installed_tools_and_relocated_config(self):
        with tempfile.TemporaryDirectory(prefix="keyhcommon tools ") as directory:
            area = Path(directory)
            source = area / "source"
            project_root = source / "KeyhCommon"
            tools = project_root / "Tools"
            tools.mkdir(parents=True)
            for name in ("reflect_codegen.py", "run_reflect_codegen.bat"):
                shutil.copyfile(REPO_ROOT / "KeyhCommon/Tools" / name, tools / name)
            for name in ("update_portfile_ref.py", "tests/test_dummy.py", "__pycache__/dummy.pyc"):
                excluded = tools / name
                excluded.parent.mkdir(parents=True, exist_ok=True)
                excluded.write_text("not for distribution", encoding="utf-8")
            for project in ("CommonBase", "ReflectSystem", "AppSystem"):
                root = project_root / project
                for configuration in ("Release", "Debug"):
                    output = root / configuration
                    output.mkdir(parents=True)
                    (output / f"{project}.lib").write_bytes(b"fixture")
                (root / f"{project}.h").write_text("// fixture\n", encoding="utf-8")
            (project_root / "CommonBase/CommonBase.natvis").write_text(
                "<AutoVisualizer/>\n", encoding="utf-8"
            )
            (source / "LICENSE").write_text("Fixture license\n", encoding="utf-8")
            package = area / "package"
            install = area / "install.cmake"
            install.write_text(
                'function(vcpkg_check_linkage)\nendfunction()\n'
                'function(vcpkg_from_git)\n'
                f'  set(SOURCE_PATH "{source.as_posix()}" PARENT_SCOPE)\n'
                'endfunction()\n'
                'function(vcpkg_build_msbuild)\nendfunction()\n'
                'set(VCPKG_TARGET_ARCHITECTURE x64)\n'
                'set(PORT keyhcommon)\n'
                f'set(CURRENT_PACKAGES_DIR "{package.as_posix()}")\n'
                f'include("{(REPO_ROOT / "ports/keyhcommon/portfile.cmake").as_posix()}")\n',
                encoding="utf-8",
            )
            self.run_command("cmake", "-P", str(install))
            relocated = area / "relocated package"
            package.rename(relocated)
            installed_tools = relocated / "tools/keyhcommon"
            self.assertEqual(
                {path.name for path in installed_tools.iterdir()},
                {"reflect_codegen.py", "run_reflect_codegen.bat"},
            )
            for path in installed_tools.iterdir():
                self.assertEqual(path.read_bytes(), (tools / path.name).read_bytes())
            for project in ("CommonBase", "ReflectSystem", "AppSystem"):
                self.assertTrue((relocated / "lib" / f"{project}.lib").is_file())
                self.assertTrue((relocated / "debug/lib" / f"{project}.lib").is_file())
                self.assertTrue(
                    (relocated / "include/keyhcommon" / project.lower() / f"{project}.h").is_file()
                )
            result = self.run_command(
                sys.executable, str(installed_tools / "reflect_codegen.py"), "--help"
            )
            self.assertIn("--project-dir", result.stdout)
            consumer = area / "consumer"
            consumer.mkdir()
            (consumer / "CMakeLists.txt").write_text(
                'cmake_minimum_required(VERSION 3.20)\n'
                'project(ToolConsumer NONE)\n'
                'find_package(KeyhCommon CONFIG REQUIRED)\n'
                f'if(NOT KeyhCommon_TOOLS_DIR STREQUAL "{installed_tools.as_posix()}")\n'
                '  message(FATAL_ERROR "Incorrect tools directory")\n'
                'endif()\n'
                'if(NOT KeyhCommon_REFLECT_CODEGEN STREQUAL '
                '"${KeyhCommon_TOOLS_DIR}/reflect_codegen.py")\n'
                '  message(FATAL_ERROR "Incorrect generator path")\n'
                'endif()\n'
                'if(NOT EXISTS "${KeyhCommon_REFLECT_CODEGEN}")\n'
                '  message(FATAL_ERROR "Missing generator")\n'
                'endif()\n',
                encoding="utf-8",
            )
            self.run_command(
                "cmake", "-S", str(consumer), "-B", str(area / "build"),
                f"-DCMAKE_PREFIX_PATH={relocated.as_posix()}",
            )

    def run_command(self, *args):
        result = subprocess.run(args, text=True, capture_output=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result
