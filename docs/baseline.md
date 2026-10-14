# Baseline

## Initial import
Imported as-is from production tag `v7.03` of each repo.

Excluded from import: helper output files `package_versions.txt`, `project_config.txt` (not part of the project).

## Synced to production

| Model  | Source repo                               | Commit                                     |
|--------|-------------------------------------------|--------------------------------------------|
| JC6416 | https://github.com/yeudap/JC6416_latest   | 570bbceb03b08091a6bf7975a354b5a2afb5d872   |
| JC4020 | https://github.com/yeudap/JC4020_latest   | 90ab89684ec00b27066fa12ad0bcd7edaeb4258d   |
| JC6432 | https://github.com/yeudap/JC6432_latest   | c310df3f009e8f7e635b2936c7c76bb2fbfc85b5   |

Changes in production since `v7.03`, all included in this sync:
- `lib_deps` and `framework-arduinoespressif32` pinned to the exact versions the 7.03 build used.
- Adafruit GFX `gfxfont.h` was patched locally in `.pio` (missing `<stdint.h>`). Fixed in project sources: `#include <stdint.h>` before `gfxfont.h`.
- HUB75 `ESP32-HUB75-MatrixPanel-I2S-DMA.h` was patched locally in `.pio` (panel width / height / chain, color depth 3). Fixed in project sources: `initDisplay()` sets geometry and color depth explicitly.

The code differs from the published 7.03 binaries. The next production release from this state must be 7.04, not 7.03.