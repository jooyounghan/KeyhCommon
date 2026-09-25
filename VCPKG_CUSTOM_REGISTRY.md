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

## 4. Publish an update

The CommonBase post-build event runs `KeyhCommon/Tools/update_portfile_ref.bat`.
It prepares local registry metadata; it does **not** upload the locally built
libraries, commit files, or push to GitHub. vcpkg builds the source commit named
by the port's `REF`, not the consumer's registry baseline commit.

1. Build and verify your source changes, then commit changes under `KeyhCommon/`
   and to `LICENSE`. If these paths contain staged, unstaged, or untracked
   changes, the post-build script warns and skips the metadata update without
   failing your development build.
2. After committing the source, run `KeyhCommon/Tools/update_portfile_ref.bat`
   explicitly. An up-to-date Visual Studio project may not run its post-build
   event again.
3. Review and commit **all** changes under `ports/keyhcommon/` and `versions/`
   together. Do not edit the port between generating metadata and committing it.
4. Push both the source commit and the registry commit. Preserve these commits:
   squashing or rewriting them can leave the source `REF` or historical port
   trees unavailable to consumers.
5. In KeyhEngine's `vcpkg-configuration.json`, set the **KeyhCommon entry in
   `registries[].baseline`** to the published registry commit, not the source
   commit or the builtin registry baseline. Run `vcpkg install` again.

When the port contents or source change for an existing package version, the
script automatically increments `port-version` (for example, `0.1.0#0` to
`0.1.0#1`) and updates the baseline. It never replaces a committed version
entry. For a new upstream release, change the version in
`ports/keyhcommon/vcpkg.json` and remove or reset `port-version` to zero before
running the script.

Repeated runs reuse an unpublished revision, regenerating its pending entry
from committed history. Metadata-only commits do not advance the source `REF`
or create another revision when its source content is unchanged. If the old
source commit is unavailable in a shallow checkout, the script conservatively
uses `HEAD`; use a checkout with that commit available to avoid unnecessary
revisions. Extracted vcpkg source directories without their own `.git` are
skipped, so installing the package does not try to publish registry metadata.
