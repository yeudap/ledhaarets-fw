# Baseline

## Initial import
Imported as-is from production tag `v7.03` of each repo.

Excluded from import: helper output files `package_versions.txt`, `project_config.txt` (not part of the project).

## Sync 1: post-7.03 cleanup

| Model  | Source repo                               | Commit                                     |
|--------|-------------------------------------------|--------------------------------------------|
| JC6416 | https://github.com/yeudap/JC6416_latest   | 570bbceb03b08091a6bf7975a354b5a2afb5d872   |
| JC4020 | https://github.com/yeudap/JC4020_latest   | 90ab89684ec00b27066fa12ad0bcd7edaeb4258d   |
| JC6432 | https://github.com/yeudap/JC6432_latest   | c310df3f009e8f7e635b2936c7c76bb2fbfc85b5   |

Changes in production since `v7.03`, all included in this sync:
- `lib_deps` and `framework-arduinoespressif32` pinned to the exact versions the 7.03 build used.
  JC4020 and JC6432 were pinned in a later production commit and synced in `e9c7639`.
- Adafruit GFX `gfxfont.h` was patched locally in `.pio` (missing `<stdint.h>`). Fixed in project sources: `#include <stdint.h>` before `gfxfont.h`.
- HUB75 `ESP32-HUB75-MatrixPanel-I2S-DMA.h` was patched locally in `.pio` (panel width / height / chain, color depth 3). Fixed in project sources: `initDisplay()` sets geometry and color depth explicitly.

This state is not byte-identical to the published 7.03 binaries.

## Sync 2: production 7.06

| Model  | Source repo                               | Tag     | Commit    |
|--------|-------------------------------------------|---------|-----------|
| JC6416 | https://github.com/yeudap/JC6416_latest   | `v7.06` | `b08543b` |
| JC4020 | https://github.com/yeudap/JC4020_latest   | `v7.06` | `3327c9d` |
| JC6432 | https://github.com/yeudap/JC6432_latest   | `v7.06` | `a6c343d` |

Covers 7.05 and 7.06. Method: full copy of `src`, `include`, `lib`, `platformio.ini` and `stamp_app_desc.py`, because no refactor work existed yet in any model.

Changed in all three models (same 22 files):
- BLE: `CoreService.cpp`
- BTN: `Button.cpp`, `ButtonLH0826.*`, `ButtonLH1022.*`
- `Hardware.h`, `Metadata.h`, `metadata.cpp`, `main.cpp`, `Logic.*`
- DB: `DayInfo.cpp`, `Place.cpp`, `Shita.*`, `Strings.*`
- States: `ConfigState.cpp`, `OfekState.cpp`, `States.h`
- `platformio.ini`

`Preferences/` unchanged: keys, indexes, types and defaults are the same as in Sync 1.

From now on, a production release is ported by patch (`git diff vX..vY`), since refactor work will exist in the models.

## Sync 3: production 7.07

| Model  | Tag     | Commit    |
|--------|---------|-----------|
| JC6416 | `v7.07` | `592c606` |
| JC4020 | `v7.07` | `101dd95` |
| JC6432 | `v7.07` | `644c0c1` |

Method: full copy (no refactor work existed yet in any model).

Changes in 7.07:
- `PREF_OP_WRITE shita` applies immediately (`ShitaState::setShita`: setDefault, configStates, recalculation, success message).
- `PREF_OP_WRITE` range checks before `fromBytes`: `shita` ≥ `numOfShitot` and `ofek` out of range return `CORE_ERR_INVALID_VALUE`.
- `DayInfo` `setHul` / after-shkiaa changes from JC6432 ported to JC6416 and JC4020; the three models are aligned.
- `hardwareModel` defined `inline` in JC6416 and JC4020 (JC6432 already was).
- JC4020 `main.cpp`: formatting aligned with the other models.

`Preferences/` unchanged. `stamp_app_desc.py` docstring fix (F13) not yet in production.

## Sync 4: production 7.08

| Model  | Tag     | Commit    |
|--------|---------|-----------|
| JC6416 | `v7.08` | `09a6c82` |
| JC4020 | `v7.08` | `bbceb47` |
| JC6432 | `v7.08` | `b75d6a7` |

Repo commit: `d01dd0f`. Method: mixed.
- JC4020 and JC6432: full copy (no refactor work exists yet in these models).
- JC6416: patch (`git diff v7.07..v7.08`), applied on top of 2a.1–2a.2. The new code was then
  adapted to the 2a views in a separate step, 2a.2.7 (`7b60c79`).

Changes in 7.08:
- F16: `dayIndex` removed in all three models: the definition, the `simplePrefs.push_back`, and the
  write in `MainState::setNewDay` (JC6416; the commented-out write in JC6432). Index 3 becomes a gap.
- F11: `PREF_OP_WRITE` range check for `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom`:
  value > `numOfZeetCochavin` returns `CORE_ERR_INVALID_VALUE`. The four keys are identified by
  preference address.
- F8: factory-reset comment in `Authmanager.h` / `Authmanager.cpp` fixed (BACK+OK for 2 seconds
  opens the settings menu). The replaced block in `Authmanager.h` also lost an unrelated line (F19).
- S1: `ClockRV3028` with `ensureBackupConfiguration` copied from JC6416 to JC4020 and JC6432.
- S2: JC4020 `validate_time` default date aligned with the other models (2026-09-01).
- S3 (partial): JC6432 `initDisplay` debug prints removed. `dumpMem` kept on purpose, as a debug
  tool, because memory is tight on JC6432.
- S5: commented-out debug code removed in JC6432 (`DayInfo.cpp`, `main.cpp`).
- S9: uniform `.gitignore`.
- `PROJECT_VER` 7.07 → 7.08.

`Preferences/`: `dayIndex` removed. All other keys, indexes, types and defaults unchanged. The NVS
key `dayIndex` stays in `clk` as an orphan entry.

Not in 7.08: F13. `stamp_app_desc.py` comes from `ledhaarets-build`, and the docstring is fixed
there separately.

## Sync 5: production 7.09

| Model  | Tag     | Commit    | Repo commit |
|--------|---------|-----------|-------------|
| JC6416 | `v7.09` | `28e1732` | `7066e4e`   |
| JC4020 | `v7.09` | `8941daa` | `da5a349`   |
| JC6432 | `v7.09` | `5e38256` | `1cb790a`   |

Branch `709` points at the same commits. Method: patch (`git diff v7.08..v7.09`) in all three
models, on top of 2a.1–2a.3.2. `UserPreferences.h` was copied as is (one-time exception to the
editing rule, approved by the owner). Before the sync: `672aa78` (contract tool stub, F22).

Changes in 7.09:
- `UserPreferences.h`: the unified table of 2a.3.2 in JC6416 / JC4020; `saveAll`, `resetAll`,
  `setValueByKey`, `getValueByKey`, `getPref` removed, `printAll` kept on purpose (debug);
  defaults `leil` 0 → 1 and `screen` 1 → 2 in JC6416 / JC4020 (aligned with JC6432; `screen` has
  no consumer there), with the `screen` comment; F22 (`String(s, strnlen(s, length))`).
- F19 (`Authmanager.h`), F17 (`RegsScreen.cpp`, d2 = `timeFormat`), F21 (JC4020
  `MainStateTimeSets.cpp`, four prints removed).
- `PROJECT_VER` 7.08 → 7.09.

Result: in all three models the compiled content is identical to production v7.09 (procedure
below). The repo tree differs from production only in the 2a views and their consumers (state at
that sync).
Not in 7.09: F13 (`stamp_app_desc.py` docstring, to be fixed in `ledhaarets-build` (open)).

### Procedure: comparing the repo with a production release (from Sync 5)

Applies only while the compiled repo is identical to production (Sync 5 and Sync 6). Since 2a.3.3
it is not; from Sync 7 the differential procedure below applies.

1. Reference: `git -C <production> archive <commit> | tar -x -C <scratch>/b/<model>` (the full
   commit hash of the release or tag; recorded in the sync table), then `pio run`
   there with `SOURCE_DATE_EPOCH=1763337600`. Keep `firmware.bin` / `.elf`, then move the folder
   away. Nothing is built in the production project.
2. Candidate: copy the repo model folder without `.pio` to the same path `<scratch>/b/<model>`
   and build with the same value. Same path → same `__FILE__` strings and library paths.
3. Expected: `tools/compare_bins.py` "Only metadata differs" (or identical); the two ELFs after
   `xtensa-esp32-elf-objcopy --strip-debug` byte-identical; `nm -C --size-sort` identical.

## Sync 6: production 709A

| Model  | Tag    | Commit                                     | Repo commit |
|--------|--------|--------------------------------------------|-------------|
| JC6416 | `709A` | `8d4db32dc3000fc33d533aa5423c92ff79b4187b` | `2b1617c`   |
| JC4020 | `709A` | `225ed5ed5fa4fdae77f0cdf8763cc2ddb49d2e78` | `c53643a`   |
| JC6432 | `709A` | `7da8a8aaf6e6ed7f405bfeab928dd4c7bc653699` | `cdc789c`   |

`709A` is a temporary production tag, not a release. It will be replaced by `v7.10` and deleted;
the commit hashes above are the reference. Method: patch (`git diff v7.09..709A`) in all three
models, applied with the editor tool on top of 2a.1–2a.3.2. One-time exception (owner's decision):
the trailing whitespace of the first two lines of `MemoryMapState::showStateScreen`
(`src/States/ConfigState.cpp`) in JC4020 and JC6432, which the editor
tool strips, was written by an exact byte replacement script.

Changes in 709A (after v7.09):
- S4: `display` defined in `Display.cpp` (JC6416 `b929601`, JC4020 `fbf9eb9`; JC6432 already was).
- S6: the four orphan `or_*` headers in `JC6432/src/DB/DataBase/orHachaim/` deleted (`8249d5e`).
- S8: whitespace aligned with JC6416 (JC4020 `5d948bb`, JC6432 `916eac2`).
- F13: `stamp_app_desc.py` docstring from `ledhaarets-build` (JC6416 `8d4db32`, JC4020 `225ed5e`,
  JC6432 `7da8a8a`).
- `PROJECT_VER` 7.09 → 7.10.

`Preferences/` and `BLE/` unchanged.

Result: in all three models the compiled content is identical to production 709A (procedure above,
reference built from the commit hash). At that sync the repo tree differed from production only in
the 2a views and their consumers. Since 2a.3.3 it also differs in `Preferences/`: the layer objects
own their preferences, and the registry keeps only the lists and load / save.

## Sync 7: production v7.10

| Model  | Tag     | Commit                                     | Repo commit |
|--------|---------|--------------------------------------------|-------------|
| JC6416 | `v7.10` | `ab8b65f1617570b3c0ba181d31f5228084344a7b` | `ddfa24b`   |
| JC4020 | `v7.10` | `0a7e50c0be07c9ecbcceadeb215048f190cfc169` | `81c79b9`   |
| JC6432 | `v7.10` | `b61a53d5c2db66aee6ca89531efb8c544d98b637` | `33379aa`   |

`v7.10^{commit}` = `main` = `origin/main` in all three production repos, and the tag is in `origin`
(`git ls-remote`). `709A` (Sync 6) stays as a historical mark. Method: patch (`git diff 709A..v7.10`)
in all three models, applied with the editor tool on top of 2a.1–2a.4. Against 709A only these files
changed; checked again at the start of the sync.

Changes in v7.10 (after 709A):
- F25: `Place::Place` copies at most the last 19 bytes of the name into `name[20]` (the beginning of
  the reversed name); `src/DB/Place.h`, all three (JC6416 `ab8b65f`, JC4020 `f864ded`, JC6432 `b61a53d`).
- F24: `SetTohenBottomLinePSZ1::okAction` writes `PSZ` instead of `HAZ`; `src/States/SetTochenState.cpp`,
  JC4020 only (`0a7e50c`). In the repo: `logicPrefs.HAZ` → `logicPrefs.PSZ`.
- `PROJECT_VER` unchanged (7.10 since 709A).

Result: the change in the repo image matches the change in production 709A → v7.10 (procedure below).
`Place.h` equals v7.10 after EOL normalization. The repo tree differs from production in the 2a
layer objects and their consumers, as before.

### Procedure: differential verification (from Sync 7)

Since 2a.3.3 the repo image is not identical to production, so the change is compared instead of the
image.

1. Files: a file that the repo has not changed since the previous sync (identical to the previous
   production commit after EOL normalization) takes the new production version; afterwards it must equal
   the production file after EOL normalization (production index LF, repo working copy CRLF). A file
   that differs (2a changes) gets the same fix in the repo's form; the two diffs are compared side by side.
   If a file that should be identical is not, stop.
2. Four builds per model, all with `SOURCE_DATE_EPOCH=1763337600` at the same path `<scratch>/b/<model>`:
   production previous and new (`git archive <full hash>`), the repo before and after. Nothing is built
   in the production project. Keep `firmware.bin`, `firmware.elf` and the `src/**/*.o`.
3. Objects: compare every `.o` of before / after, debug info ignored, every section with its
   relocations (relocations are symbolic, so code moved elsewhere does not show). Expected: the same
   objects and the same sections change in production and in the repo; sections that do not depend on
   preference addressing are byte- and relocation-identical to the new production version; in the
   others the only difference is the preference address (layer object vs `userPreferences`). Any other
   changed object or section is explained or stops the sync.
4. ELF: `nm -S -C` before / after; the changed symbols match production, other functions only by
   linker layout. RAM unchanged unless production's RAM changes.
5. `tools/flash_check.py` as a filter: before → after, `manual` only in the functions of step 3; and
   production vs repo (reference: production's `src/Preferences/UserPreferences.h`) for the previous
   pair and for the new pair, both with `manual` 0. A new category stops.
6. The fixed criterion: `prefs_contract.py check` (also with the `.cpp` files in reverse order),
   `count_prefs.py` sums unchanged (shifts between keys only as the fix explains), `count_deps.py
   --compare`, `gen_deps_doc.py check`, contract strings, `git ls-files --eol`.