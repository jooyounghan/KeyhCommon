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
      "reference": "main",
      "baseline": "<registry-commit-with-keyhcommon-version-metadata>",
      "packages": [
        "keyhcommon"
      ]
    }
  ]
}
```

Use the **published registry commit** from the **Publish vcpkg registry** Actions
run summary in the `baseline` field, not the source commit that started the run.
`reference` tells baseline updates which branch to follow; it does not make
`vcpkg install` automatically update the pinned baseline.

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

Publication is handled by `.github/workflows/publish-vcpkg.yml`, **not by a
Visual Studio build**. The old post-build events and `update_portfile_ref.bat`
have been removed. Building or installing KeyhCommon no longer edits registry
files.

1. Build and verify the source, then push or merge it into `main`. Source PRs
   may be squash-merged: registry metadata is generated **after** the merge.
2. Wait for **Publish vcpkg registry** to finish. It checks out the latest
   `main` with full history, runs the registry tests, and prepares a revision
   with `KeyhCommon/Tools/update_portfile_ref.py`.
3. The workflow pins the committed source in `REF`, increments `port-version`
   when source or packaging changes, and commits/pushes `ports/keyhcommon/`
   and `versions/` together. Existing version entries remain immutable.
4. Use the registry commit in the run summary to update consumers (below).

The first run after this change also replaces the old source pin. No-op runs
do not create commits or revisions. Concurrent publication jobs are serialized
and each checks out the latest `main`, rather than publishing a queued old
event's source. The metadata commit made with `GITHUB_TOKEN` does not trigger
another workflow run.

**Repository setup:** Actions must be enabled, and repository/organization
policy must permit the workflow's `contents: write` permission and bot pushes
to `main`. If branch protection requires PRs for every change or otherwise
blocks the bot, publication fails; arrange an approved publishing permission
before using this workflow. No personal access token is required. A rejected
push is an error, not a successful publication. For a transient push race,
the next main push runs publication again; alternatively rerun the workflow
on `main` using **Run workflow**. Do not force-push.

Do not squash or rewrite already **published** source/registry commits:
consumers need their source commits and historical port trees to stay fetchable.
For a new upstream release, change the version in
`ports/keyhcommon/vcpkg.json` and remove or reset `port-version` to zero.

## 5. Update KeyhEngine to the published version

A Git registry baseline is a lock, not a "latest" alias. Publishing a new
KeyhCommon revision cannot change KeyhEngine's existing lock. Neither cache
deletion nor repeatedly running `vcpkg install` updates that baseline.

After publication succeeds, run these commands from KeyhEngine's manifest
directory using the same vcpkg executable used by its build:

```console
vcpkg x-update-baseline
vcpkg install
```

`x-update-baseline` advances **all configured registry baselines**, including
the builtin one. To update only KeyhCommon, instead copy the published registry
commit from the Actions summary into the KeyhCommon entry's
`registries[].baseline` in `vcpkg-configuration.json`, then run `vcpkg install`.
Commit the updated consumer configuration so teammates and CI use the same
revision. If the configuration is embedded in `vcpkg.json`, update it there.
Remove or update any consumer `overrides` pin or overlay port that intentionally
selects an older/local `keyhcommon`; those can supersede the registry selection.

Check the install plan for the newly published version (for example,
`keyhcommon:...@0.1.0#1`). Its port snapshot pins the corresponding source
commit, so vcpkg will build or restore the matching package rather than reuse
an older revision. The workflow publishes source/port metadata, not prebuilt
Visual Studio libraries.

KeyhEngine is a separate repository: this workflow does not edit its files.
If every KeyhEngine build must consume the latest published revision, its own
build/update automation must run the baseline update **before** installation
and only after KeyhCommon publication completes.
