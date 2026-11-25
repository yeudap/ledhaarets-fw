# LED Haarets clock firmware — refactor workspace

## What this repo is

Refactor workspace for the ESP32 firmware of the LED Haarets clocks (JC6416, JC4020, JC6432).
**This is not production.** Production firmware lives in separate projects at
`C:\Users\yeuda\Dev\<model>_latest` (last field release: tag `v7.10`) and is built and published only by `ledhaarets-build/build_tool.py` (`C:\Users\yeuda\myProjects\JewishClock\ledhaarets-build`). Nothing from
this repo reaches the field until the user explicitly declares it the new production.

Always reply to the user in Hebrew, even when the code, tool output or recent messages are
in English. Code, identifiers and commit messages stay in English; keep the existing
comment language of each file.

## Hard contracts — must stay compatible with clocks in the field

If a change touches any of these, stop, explain the impact, and wait for explicit approval,
even if it looks like a pure rename or move.

1. **BLE protocol** — service/characteristic UUIDs, command and response codes, payload byte
   layout and byte order, auth flow (`Authmanager`), PIN behavior, preference indexes and
   preference defaults (`PREF_OP_SYNC` reports values that differ from the firmware default).
2. **Advertisement** — `LEDHAARETS` + version(4) + hardware(6) + model(6), exact lengths.
3. **NVS** — namespace names, key names, value types and units (e.g. `rabenuTam` is minutes
   in 7.x). Stages 1–2 must not change NVS in any way. The per-layer namespace split is a
   separate stage with a migration design approved in advance.
4. **OTA protocol** — HELLO → START → DATA → FINISH, packet size assumptions, MD5 and size
   checks, `START_OK` semantics.
5. **Image metadata** — `esp_app_desc_t` fields written by `stamp_app_desc.py`
   (`project_name` = model, `version`), `Metadata.h` fixed lengths and `static_assert`s,
   build flags `PROJECT_VER`, `PRODUCT_MANUFACTURER`, `DEVICE_MODEL`. The hardware version is
   detected at runtime (`detectHardwareModel` → `Metadata::setHardwareVersion`), 6 chars; it is
   not stamped into `esp_app_desc_t`.
6. **Flash layout** — `default_16MB.csv` (6.25MB per OTA slot), flash `dio / 80m / 16MB`.
   The app image must fit one OTA slot.
7. **Toolchain** — `espressif32 @ 6.12.0`, Arduino-ESP32 2.0.17, `gnu++17`, pinned
   `lib_deps`. Never upgrade platform or libraries as part of a refactor step.

Reference snapshots of contracts live in `docs/contracts/`: the BLE contract (`ble.md`, with
preference indexes and defaults) and the preference contract baselines (`prefs/`). The NVS, OTA,
advertisement and metadata contracts are not written yet. If code and a contract file disagree,
report it. Do not "fix" either side.

## Repo layout

- `JC6416/`, `JC4020/`, `JC6432/` — synced to production main (see `docs/baseline.md`).
  Each is a standalone PlatformIO project.
- `docs/contracts/` — `ble.md` (BLE contract) and `prefs/` (preference contract baselines,
  `tools/prefs_contract.py`). NVS, OTA, advertisement and metadata contracts: not written yet.
- `docs/architecture/` — `dependencies.md` (unit map and layers), `preferences.md`
  (preference classification by layer), `notes.md` (the owner's answers on intent and
  constraints — the source of truth for "why", read it before proposing structure).
- `docs/refactor-log.md` — one entry per approved step, plus a "Findings" section.
- `docs/environment.md` — background snapshot of the LED Haarets environment (exported
  14/10/2026). Context only; the code is the source of truth.
- No `baseline/` folder. The reference for each step is built at the start of that step
  (`pio run` before editing; `firmware.bin`, build log and `nm` output saved in the scratchpad).
- `tools/compare_bins.py` — compares two firmware images and labels where they differ.

Current structure of each project: root (`main.cpp`, `Display`, `Area`, `Logic`, `Text`,
`Colors.h`, `Hardware.h`, `Metadata`), `BLE/`, `BTN/` (`ButtonDriver`: LH1022, LH0826),
`CLK/` (`ClockDriver`: DS3231, RV3028), `DB/` and `DB/DataBase/`, `Fonts/`,
`Preferences/` (`UserPreferences`, NVS), `Screens/`, `States/`. Same skeleton in all three,
but the content differs in many places.

## Stages

**Current stage: 2a** — split `userPreferences` into per-layer structs. 2a.1 and 2a.2 are done
in all three models (verification rules in `refactor-log.md`); `count_deps.py` counts access
through the layer objects, and `dependencies.md` is generated with `tools/gen_deps_doc.py`. Production v7.10 (commit hashes in `docs/baseline.md`, Sync 7; synced with the
differential verification described there, since the repo image is no longer identical to production) is
synced into this repo. 2a.3 is planned with the owner (decisions in `refactor-log.md`, "2a.3 —
plan"); done: 2a.3.0 (contract tool `tools/prefs_contract.py`, baselines in
`docs/contracts/prefs/`), 2a.3.1 (unused registry functions removed, all models), 2a.3.2 (one
preference table for all models, as in JC6432; documented contract change), 2a.3.3 (the layer
objects own their preferences, all three models in one step; 2a.3.4 cancelled; flash-check
criterion for non-identical images in `refactor-log.md`, "Decisions"). Docs step done: references
by symbol (file + function or class), line numbers only where there is no other way (rule and
exceptions in `refactor-log.md`). Clock test of 2a.3 (2a.3.5) passed on all three
models (23.11.2026). 2a.4 done in all three models (preference definitions as `constexpr` entries in
flash with a draft range and `static_assert`, the registry as one const array; heap measured on JC6416;
flash-check categories of 2a.4 in `refactor-log.md`). Roadmap: `docs/architecture/roadmap.md`;
improvement proposals: refactor-log, "Improvement proposals (P#)". Clock test of 2a.4 (2a.4.x) passed on
all three models; 2a.4 is closed. **Next step (owner's decision):** 2b, preference write and apply (plan
and decisions in refactor-log, "2b — plan"). 2b.0 (docs) done; next 2b.1, then a stop for the owner's clock
test and the detailed 2b.2 plan.
Existing NVS keys, namespaces and types never change. Any change to the preference contract
(new keys, BLE indexes, defaults) happens only as a documented contract-change step approved by
the owner and recorded in refactor-log. Otherwise behavior stays identical.

**New generation after the refactor (major 8; owner's decision, recorded in refactor-log):**
- App and firmware versions depend on each other, and the app forces updates. No BLE
  compatibility with old versions is needed.
- Frozen, must stay compatible: the advertisement (`LEDHAARETS` + version), authentication, OTA
  and the partition table. The build must fit the existing slot. There is no update by cable.
- NVS: on the first boot the firmware migrates. Texts, shita, location and eda carry over, the
  rest is reset, and a generation key is written. The migration is safe to run again. Before OTA
  the app keeps a backup of the same data, as a safety net, and guides the new setup with a wizard.
- The app checks `MAX_SUPPORTED_FW_MAJOR` and shows an "app update required" banner. This check
  goes into the app before the new generation is released. Downgrade is not offered to users,
  only as a support action.
- The legacy checks in the app and `docs/contracts/ble.md` §9 are removed when the new generation
  is released.
- Until then, the contract rule above applies.

- **0** — Baseline: contracts extraction, dependency map, preference classification.
  Read-only except for files under `docs/`.
- **1** — Reorganize into layer folders. File moves, file renames and include/path updates
  only. No logic changes, no signature changes, no reformatting. Same target layout in all
  three projects, so later diffs between them show only real differences. (Postponed.)
- **2a** — Per-layer preference structs (`DataPrefs` with place / zmanim / minhagim,
  `LogicPrefs`, `PresentationPrefs` with layout, `DisplayPrefs`, `UserContent`, `SystemPrefs`) and a
  central registry that owns the table (key, index, type, default, field) and load/save only.
  `PREF_OP_WRITE` range checks (`shita`, `ofek`, and the four zeet keys) and the immediate
  apply (`ofek`, `shita`) stay in `CoreService`; registering limits and actions from above
  belongs to the later "actions mechanism" stage, together with immediate apply of the other
  preferences. Classification: `docs/architecture/preferences.md`; decisions:
  `docs/architecture/notes.md`. Sub-steps:
  1. Structs as views onto the existing `userPreferences` fields. No consumer changes. (Done.)
  2. Move consumers to their layer's struct, one layer per step. (Done.)
  3. Move field ownership into the structs; `UserPreferences` becomes the registry. (Done:
     2a.3.0–2a.3.3; clock test 2a.3.5 passed.)
- **2** — Isolate the remaining layers behind interfaces and break couplings.
- **Later, each approved separately** — applying preference changes from BLE immediately
  (shared apply functions with the menu); NVS namespace per layer with migration; one
  project with `[env:<model>]` per model; per-module versioning; BLE layer identical in all
  models; the decision on texts moves to Logic (F3).

Only the user changes the current stage.

## Working rules

- One small, reviewable step at a time. Before a new task, present a plan or mapping (which
  files will change, what will change, and why) and stop for approval. Once a plan is approved
  for sequential execution, carry out its steps without stopping; stop only on a deviation or
  where the plan says to stop.
- Check every plan against `docs/architecture/roadmap.md` and state in the plan what in it
  prepares for a roadmap item or blocks one.
- Code is not sacred: much of it grew from quick fixes. While working, collect improvement
  proposals (P#) in refactor-log: inconsistencies between models/menu/app/comments, temporary
  solutions, special cases that block uniformity, logic worth reshaping. Each proposal: current
  behavior, why it matters, suggested change, impact on field clocks / app / contract. Never
  implement a proposal without the owner's explicit decision; refactor steps stay
  behavior-identical.
- Never mix a move and a content change in one step.
- Never touch anything outside this repository, especially the production projects and
  `ledhaarets-build`.
- Never flash, upload, or talk to hardware: no upload targets, no `esptool`, no serial
  monitor. The user flashes test clocks.
- Never run `build_tool.py` or anything that uploads to the server.
- Commit a step only after every verification check of that step passes, with a message in the
  existing style (subject `<step>: <what> (<model>)`, body with the counts and the verification
  result). If any check deviates from the expected result, stop before committing and report.
- In a step with several models, every commit is made with a path (`git commit -m … -- <model>/`),
  so only that path goes in, whatever else is in the index. Before each commit:
  `git diff --cached --name-only -- <model>/`.
- Never push, reset, checkout or switch branches; push is done by the owner only
  (`.claude/settings.json` blocks these commands).
- Show a diff before editing only when the owner asks for it, or when the step has no binary
  verification (documents only, tools). When a plan is approved for sequential execution, that
  approval replaces the diff before editing for every step in it, including document and tool
  steps.
- Do not change `PROJECT_VER` unless asked.
- No reformatting, no fixing unrelated warnings or typos, no "improving" code being moved.
  Anything that looks like a bug goes to `docs/refactor-log.md` → Findings, not into the code.
- A production hotfix reported by the user is ported as its own step, labeled "hotfix port".
- Edit files only with the editor tools, not `sed` or scripts. Keep the existing line endings
  (source files are CRLF in the working copy); check with `git ls-files --eol` after every step.
  Exception: `tools/gen_deps_doc.py write` may rewrite only the blocks between the
  `<!-- generated:<name>:begin/end -->` markers in `docs/architecture/dependencies.md`, with CRLF.
  Second exception: `tools/prefs_contract.py write` may (re)write only the preference contract
  baselines `docs/contracts/prefs/<model>.txt`, with LF.
  Everything else in that file, and every other file, is edited with the editor tools.

## Verification after every step

1. `pio run` succeeds for every env of every affected project, with no new warnings.
2. Stage 1 steps: compare the symbol table with the reference built at the start of the step
   (`xtensa-esp32-elf-nm -C --size-sort .pio/build/<env>/firmware.elf`, from PlatformIO's
   `toolchain-xtensa-esp32` package). Function symbols and sizes must match. Small
   `.rodata` differences from `__FILE__` strings are expected; report them. File moves change
   debug info, so the expectation in item 4 applies, except for those `__FILE__` differences,
   which also show as code/data in `compare_bins.py` and in the stripped ELF.
3. Stage 2a steps: confirm that the registry still lists every preference with the same key,
   index, type and default, in the same load order (compare against
   `docs/architecture/preferences.md`). For sub-step 1, `git diff` shows only additions under
   `Preferences/`.
4. Report the bin size delta per env. For comparison builds (the reference and the candidate),
   set `SOURCE_DATE_EPOCH` to the same fixed value in both, so `__DATE__` / `__TIME__` are
   identical. Never set it for a production build. The image embeds the SHA-256 of the full
   ELF, debug info included, so a step that adds or moves source lines changes it even when
   the code is identical. Expected result of a no-logic-change step (e.g. 2a.2):
   `tools/compare_bins.py` reports "Only metadata differs" (ELF SHA-256 and trailer only), the
   two ELFs after `xtensa-esp32-elf-objcopy --strip-debug` are byte-identical, and
   `nm -C --size-sort` is identical.
5. Confirm that contract strings are unchanged (UUIDs, NVS namespaces and keys,
   advertisement prefix).
6. Every step that changes edges (`count_deps.py --compare` against the tree before the step
   shows a changed edge), and every production sync: `python -I tools/gen_deps_doc.py check`
   (JC6416). If it fails, regenerate `dependencies.md` with `write` in the same step, or report
   why not.
7. Summarize what changed and what needs testing on a real clock.