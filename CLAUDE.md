# LED Haarets clock firmware — refactor workspace

## What this repo is

Refactor workspace for the ESP32 firmware of the LED Haarets clocks (JC6416, JC4020, JC6432).
**This is not production.** Production firmware lives in separate, frozen projects at
`C:\Users\yeuda\Dev\<model>_latest`    (last field release: tag `v7.03`; `main` is 7.04, not yet released) and is built and published only by
`ledhaarets-build/build_tool.py`. Nothing from this repo reaches the field until the user
explicitly declares it the new production.

Talk to the user in Hebrew. Code, identifiers and commit messages in English; keep the
existing comment language of each file.

## Hard contracts — must stay compatible with clocks in the field

If a change touches any of these, stop, explain the impact, and wait for explicit approval,
even if it looks like a pure rename or move.

1. **BLE protocol** — service/characteristic UUIDs, command and response codes, payload byte
   layout and byte order, auth flow (`Authmanager`), PIN behavior, preference indexes.
2. **Advertisement** — `LEDHAARETS` + version(4) + hardware(6) + model(6), exact lengths.
3. **NVS** — namespace names, key names, value types and units (e.g. `rabenuTam` is minutes
   in 7.x). Stages 1–2 must not change NVS in any way. The per-layer namespace split is a
   separate stage with a migration design approved in advance.
4. **OTA protocol** — HELLO → START → DATA → FINISH, packet size assumptions, MD5 and size
   checks, `START_OK` semantics.
5. **Image metadata** — `esp_app_desc_t` fields written by `stamp_app_desc.py`
   (`project_name` = model, `version`), `Metadata.h` fixed lengths and `static_assert`s,
   build flags `PROJECT_VER`, `PRODUCT_MANUFACTURER`, `HARDWARE_VERSION`, `DEVICE_MODEL`.
6. **Flash layout** — `default_16MB.csv` (6.25MB per OTA slot), flash `dio / 80m / 16MB`.
   The app image must fit one OTA slot.
7. **Toolchain** — `espressif32 @ 6.12.0`, Arduino-ESP32 2.0.17, `gnu++17`, pinned
   `lib_deps`. Never upgrade platform or libraries as part of a refactor step.

Reference snapshots of these contracts live in `docs/contracts/`. If code and a contract
file disagree, report it. Do not "fix" either side.

## Repo layout

- `JC6416/`, `JC4020/`, `JC6432/` — synced to production main (see docs/baseline.md). Each is a standalone PlatformIO project.
- `docs/contracts/` — BLE, NVS, OTA, advertisement, metadata.
- `docs/refactor-log.md` — one entry per approved step, plus a "Findings" section.
- `baseline/` — reference outputs of the import commit (symbol lists, bin sizes) used to
  verify no-logic-change steps.

Current structure of each project: root (`main.cpp`, `Display`, `Area`, `Logic`, `Text`,
`Colors.h`, `Hardware.h`, `Metadata`), `BLE/`, `BTN/` (`ButtonDriver`: LH1022, LH0826),
`CLK/` (`ClockDriver`: DS3231, RV3028), `DB/` and `DB/DataBase/`, `Fonts/`,
`Preferences/` (`UserPreferences`, NVS), `Screens/`, `States/`. Same skeleton in all three,
but the content differs in many places.

- `docs/environment.md` — background snapshot of the LED Haarets environment (exported 14/10/2026). Context only; the code is the source of truth.
## Stages

**Current stage: 0**

- **0** — Baseline: contracts extraction, baseline builds, per-file comparison of the three
  projects. Read-only except for files under `docs/` and `baseline/`.
- **1** — Reorganize into layer folders. File moves, file renames and include/path updates
  only. No logic changes, no signature changes, no reformatting. Same target layout in all
  three projects, so later diffs between them show only real differences.
- **2** — Isolate layers behind interfaces and break couplings.
- **Later, each approved separately** — NVS namespace per layer with migration; one
  project with `[env:<model>]` per model; per-module versioning.

Only the user changes the current stage.

## Working rules

- One small, reviewable step at a time. Before editing, say which files will change, what
  will change, and why. Then wait.
- Never mix a move and a content change in one step.
- Never touch anything outside this repository, especially the production projects and
  `ledhaarets-build`.
- Never flash, upload, or talk to hardware: no upload targets, no `esptool`, no serial
  monitor. The user flashes test clocks.
- Never run `build_tool.py` or anything that uploads to the server.
- Do not commit, push, reset, or switch branches. Propose a commit message; the user commits.
- Do not change `PROJECT_VER` unless asked.
- No reformatting, no fixing unrelated warnings or typos, no "improving" code being moved.
  Anything that looks like a bug goes to `docs/refactor-log.md` → Findings, not into the code.
- A production hotfix reported by the user is ported as its own step, labeled "hotfix port".

## Verification after every step

1. `pio run` succeeds for every env of every affected project, with no new warnings.
2. Stage 1 steps: compare the symbol table with `baseline/`
   (`xtensa-esp32-elf-nm -C --size-sort .pio/build/<env>/firmware.elf`, from PlatformIO's
   `toolchain-xtensa-esp32` package). Function symbols and sizes must match. Small
   `.rodata` differences from `__FILE__` strings are expected; report them.
3. Report the bin size delta per env.
4. Confirm that contract strings are unchanged (UUIDs, NVS namespaces and keys,
   advertisement prefix).
5. Summarize what changed and what needs testing on a real clock.
