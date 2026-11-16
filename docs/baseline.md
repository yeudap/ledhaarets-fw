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