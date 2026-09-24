# KeyhCommon custom vcpkg registry

This repository can be used directly as a custom vcpkg registry for the `keyhcommon` package.

## 1. Add the registry

Create `vcpkg-configuration.json` in the consuming project:

```json
{
  "default-registry": {
    "kind": "builtin",
    "baseline": "<builtin-vcpkg-baseline>"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/jooyounghan/KeyhCommon.git",
      "baseline": "<registry-commit-with-keyhcommon-version-metadata>",
      "packages": [
        "keyhcommon"
      ]
    }
  ]
}
```

Use the registry commit you want to pin in the `baseline` field.

## 2. Request the package

Add `keyhcommon` to the consuming project's `vcpkg.json`:

```json
{
  "name": "sample-app",
  "version-string": "0.1.0",
  "dependencies": [
    "keyhcommon"
  ]
}
```

## 3. Link from CMake

```cmake
find_package(KeyhCommon CONFIG REQUIRED)
target_link_libraries(sample-app PRIVATE KeyhCommon::KeyhCommon)
```

You can also link `KeyhCommon::CommonBase`, `KeyhCommon::ReflectSystem`, or `KeyhCommon::AppSystem` individually.
