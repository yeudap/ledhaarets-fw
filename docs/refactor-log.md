# Refactor log

One entry per approved step. Anything that looks like a bug or a contract inconsistency is
recorded under **Findings** and is not fixed in code as part of a refactor step.

## Steps

### Stage 0 / A — contracts extraction

- **A.1** — Created `docs/refactor-log.md` (this file) with initial Findings.
  No code changes.
- **A.2** — Created `docs/contracts/ble.md`. Extended F3; added F4–F9. App-side facts
  (encoding, MTU, defaults) taken from the app source as reported by the user. No code changes.

### Production ports

- **Production port 7.06** (commit `3c67caa`) — full copy of production `v7.06` into all three
  models; covers 7.05 and 7.06. Details in `docs/baseline.md`, Sync 2. `Preferences/` unchanged
  (keys, indexes, types, defaults). Resolved F1. Recorded F10–F14.
- **Production port 7.07** (commit `dcd2cdb`) — full copy of production `v7.07` into all three
  models. Details in `docs/baseline.md`, Sync 3. `Preferences/` unchanged. Closed F12 and F14;
  F11 partly resolved; F8 updated; recorded F15.

### Stage 2a — per-layer preference structs

- **2a.1** (JC6416) — Added per-layer preference views under `JC6416/src/Preferences/`:
  `DataPrefs.h` (`dataPrefs.place` / `.zmanim` / `.minhagim`), `LogicPrefs.h`,
  `PresentationPrefs.h` (`presentationPrefs`, with `layout`: `middleLine`, `upLine`, `screen`),
  `DisplayPrefs.h`, `SystemPrefs.h` (`bt` only), `UserContent.h` (`textPrefs`), and
  `PrefsViews.cpp` (`static_assert` per field, no code or data).
  - **Form:** structs of `UserPref<T> &` references, instances `inline constexpr` in the
    headers, bound to the existing `userPreferences` fields. Field names equal the keys; fields
    in each struct are in BLE index order. Classification as in `docs/architecture/preferences.md`
    §3/§4 (commit `e76aaea`).
  - **Not in any struct:** `forceWrite` (central registry), `dayIndex` (to be removed, F16).
  - **No consumer changes.** No existing file changed; JC4020 and JC6432 untouched.
  - **Verification** (reference: clean build of `e76aaea`, built before editing):
    - `pio run -d JC6416` (env `esp32dev`) succeeds; full build, 0 compiler warnings before
      and after. GCC 8.4 accepts `inline constexpr`; no fallback needed.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `tools/compare_bins.py`: 66 bytes differ — 32 in `app_desc` (ELF SHA-256), 33 in the
      trailer, and 1 in code: a `__TIME__` digit in `getDate` (`src/Screens/VersionScreen.cpp:33`,
      `movi.n a15, 52` vs `54`), from recompiling at a different time.
    - Coverage script against `preferences.md` §4: 71 fields, each exactly once, each in the
      struct matching its layer column, index order kept; `forceWrite` and `dayIndex` absent;
      `UserContent` refers to `textPrefs`.
    - `git status`: only the 7 new files. Keys, indexes, types, defaults, load order and
      contract strings (UUIDs, `clk` / `sec` / `sys`, `LEDHAARETS`) unchanged.
  - **Clock test:** not needed; compiled code identical except the build time.
- **2a.2** (JC6416) — move consumers to their layer's struct, one layer per sub-step:
  `userPreferences.X` → `<view>.X` and one `#include "Preferences/<Layer>.h"` after the last
  existing include; no other change on the line. Order: System, UserContent, Presentation,
  Display, Logic, Data. Commented-out lines that mention a field are replaced in their layer's
  sub-step too, so that at the end of 2a.2 no `userPreferences.<field>` remains outside
  `Preferences/`. Registry members (`allPrefs`, `simplePrefs`, `resetSimplePrefs`,
  `resetTextPrefs`, `printAllValues`) and `dayIndex` stay on `userPreferences`. After the last
  layer, a separate sub-step updates `tools/count_prefs.py` to count access through the structs
  (until then it does not see moved accesses).
- **2a.2.1** (JC6416) — System: `userPreferences.bt` → `systemPrefs.bt`, 6 places:
  `src/States/ConfigState.cpp` (5) and `src/main.cpp` (1), plus `#include "Preferences/SystemPrefs.h"`
  in both files.
  - **Verification** (reference: clean build of `c6882d6`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `SOURCE_DATE_EPOCH` reaches the compiler: the `__DATE__` / `__TIME__` immediates in
      `getDate` (`src/Screens/VersionScreen.cpp`) are `Nov 17 2025 00:00:00`, the fixed value.
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `tools/compare_bins.py`: not `identical`; "Only metadata differs": 31 bytes in `app_desc`
      (ELF SHA-256) and 33 in the trailer. Code and data identical.
    - Cause: the ELF differs only in debug sections (`.debug_info`, `.debug_abbrev`,
      `.debug_line`, `.debug_str`), from the line shift of the new include and the new header's
      type info. After `objcopy --strip-debug` the two ELFs are byte-identical. The image embeds
      the SHA-256 of the full ELF, so any step that adds or moves a source line changes it.
    - `grep`: no `userPreferences.bt` outside `Preferences/`; `systemPrefs.bt` 6 times.
    - `git status`: only the two source files. Nothing under `Preferences/` changed, so keys,
      indexes, types, defaults and load order are unchanged; contract strings untouched.
    - Line endings of the edited files kept as CRLF in the working copy.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.2** (JC6416) — UserContent and Presentation:
  - `userPreferences.textPrefs` → `userContent.textPrefs`, 7 places in
    `src/BLE/TextCommandHandler.cpp`, plus `#include "Preferences/UserContent.h"`.
  - `userPreferences.<field>` → `presentationPrefs.<field>` for `leil`, `dateFormat`,
    `clockFormat`, `timeFormat`, and → `presentationPrefs.layout.<field>` for `middleLine`,
    `upLine`: 50 occurrences on 39 lines in `src/DB/DayInfo.cpp`, `src/DB/Zman.cpp`,
    `src/Logic.cpp`, `src/Screens/RegsScreen.cpp`, `src/States/MainState.cpp`,
    `src/States/MainStateTimeSets.cpp`, `src/States/SetClockState.cpp`, plus
    `#include "Preferences/PresentationPrefs.h"` in each. `safa` and `layout.screen` have no
    consumers in JC6416. No commented-out lines in these two layers.
  - Edits made with the editor tool only.
  - **Verification** (reference: clean build of `05834a2`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `objcopy --strip-debug`: the two ELFs are byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256)
      and 33 in the trailer. As expected.
    - `grep`: none of the 9 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `userContent.textPrefs` 7 times, `presentationPrefs.` 50 times (4 of them `.layout.`).
    - `git diff --stat`: 8 files, 54 insertions, 46 deletions (46 replaced lines + 8 includes).
      Nothing under `Preferences/` changed; contract strings untouched.
    - `git ls-files --eol`: all 8 files `w/crlf`, as before.
  - **Recorded:** F17, F18 (documentation only, no code change).
  - **Clock test:** not needed; compiled code identical.
- **2a.2.3** (JC6416) — Display:
  - `userPreferences.<field>` → `displayPrefs.<field>` for `brightness`, `colorUp`, `colorMiddle`,
    `colorBottomT`, `colorBottomV`, `colorYom`, `colorParasha`, `colorIluy`, `clockFont`:
    55 occurrences on 47 lines in `src/Logic.cpp` (17), `src/main.cpp` (2),
    `src/States/ColorState.cpp` (14), `src/States/BrightnessState.cpp` (2),
    `src/States/FontState.cpp` (3), `src/States/MainState.cpp` (1), `src/Screens/RegsScreen.cpp`
    (16), plus `#include "Preferences/DisplayPrefs.h"` in each. All 9 fields have consumers in
    JC6416. No commented-out lines in this layer. `mainScreen.colorUp` etc. are screen members,
    not preferences, and are unchanged.
  - Edits made with the editor tool only.
  - **Verification** (reference: clean build of `935fb04`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `objcopy --strip-debug`: the two ELFs are byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256)
      and 33 in the trailer. As expected.
    - `grep`: none of the 9 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `displayPrefs.` 55 times.
    - `git diff --stat`: 7 files, 54 insertions, 47 deletions (47 replaced lines + 7 includes).
      Nothing under `Preferences/` changed; contract strings untouched.
    - `git ls-files --eol`: all 7 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.

## Planned for next production release

Decided by the owner. To be done in the production projects, then ported here with the next sync.

| Finding | Change |
|---|---|
| F11 | `PREF_OP_WRITE`: range check of `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom` against `numOfZeetCochavin`, like the 7.07 check of `shita` / `ofek` |
| F8 | Fix the factory-reset trigger comment in `Authmanager.h` / `Authmanager.cpp` |
| F13 | Fix the `stamp_app_desc.py` docstring (no `HARDWARE_VERSION` build flag) |
| F16 | Remove `dayIndex`: stop the write in `MainState::setNewDay` (JC6416), and remove the preference from `UserPreferences` in all three models, since indexes are explicit (index 3 stays a gap). Had indexes been positional, only the write would be stopped and the definition kept, to preserve index 3 |

## Findings

Line numbers in F1–F9 refer to commit `e9c7639`; from F10 on, to commit `dcd2cdb` (7.07).
Status updates inside older findings use `dcd2cdb` line numbers and say so.

### F1 — `metadata.cpp` includes `metadata.h`, file is `Metadata.h`

- **Where:** `JC6416/src/metadata.cpp:1`, `JC4020/src/metadata.cpp:1`, `JC6432/src/metadata.cpp:1`
- **What:** `#include "metadata.h"`, while the header on disk is `src/Metadata.h`. All other
  includers use `"Metadata.h"` (e.g. `src/BLE/Bluetooth.h:6`).
- **Impact:** builds only on a case-insensitive filesystem (Windows). Would break a
  Linux/CI build. No effect on the produced binary.
- **Models:** same in all three.
- **Status:** resolved in production 7.06. `metadata.cpp:2` now includes `"Metadata.h"` in all
  three models (`dcd2cdb`). Closed.

### F2 — JC6432 preference indexes differ from JC6416/JC4020 (BLE contract divergence)

- **Where:** `src/Preferences/UserPreferences.h`
  - JC6416 / JC4020: `screen` = 72 (`:839`), `bt` = 73 (`:846`)
  - JC6432: `colorIvri` = 72 (`:842`), `colorLoazi` = 73 (`:845`), `colorParashaV` = 74 (`:848`),
    `screen` = 75 (`:851`), `bt` = 76 (`:856`)
- **What:** the same preference index refers to a different key depending on the model.
  Preference indexes are part of the BLE contract (`PREF_OP_SYNC` returns indices).
  JC6432 also has different defaults for `colorYom`, `colorParasha`, `leil`, `screen`, and
  a different meaning for `screen` (1 = Big, 2 = Small vs. 1–4 layout variants).
- **Impact:** an app that maps indexes to keys without checking the model will
  misinterpret preferences 72–76 on JC6432 or on the other two models. Must be preserved as-is;
  any future unification needs an approved migration.
  Per the app source (`lib/config/preference_constants.dart`, outside this repo, as reported
  by the user): the app's preference map ends at index 71 (`IORT`) and does not define 72–76,
  so there is currently no actual collision between models.
- **Models:** JC6432 differs; JC6416 and JC4020 are identical.

### F3 — JC4020: TEXT write and display over BLE are disabled

- **Where:** `JC4020/src/BLE/TextCommandHandler.cpp`
  - `handleWriteText` (`:63`) — body commented out; always returns `ERR_INVALID_INDEX`.
  - `handleDisplayText` (`:138`) — the `if` is commented out but its block remains, so it
    always returns `ERR_INVALID_INDEX`; the `try` block below is unreachable.
  - `handleDeleteText` — `numOfValidTexts` refresh commented out (`:126`).
- **What:** JC6416 and JC6432 (identical file) implement these operations; JC4020 rejects them.
  Likely intentional (no free-text display on JC4020), but it is a per-model difference in
  BLE behavior for `TEXT_OP_WRITE` / `TEXT_OP_DISPLAY`.
- **Also:** `JC4020/src/States/States.h:28` sets `numOfNameSets = 0` (JC6416/JC6432: 50), so on
  JC4020 every index is invalid: READ, DELETE and SWAP also return `INVALID_INDEX`, and COUNT
  returns 0. The commented-out code in write/display does not change the wire behavior.
- **Models:** JC4020 differs; JC6416 and JC6432 are identical.

### F4 — PREF STR value: READ returns a trailing `\0`, protocol comment says it doesn't

- **Where:** `src/Preferences/UserPreferences.h:268-275` (`toBytes` appends `0`) vs.
  `src/BLE/CoreProtocol.h:213` ("ללא null terminator בערך עצמו").
- **What:** `PREF_OP_READ` for a STR key (`textPlace`) returns `[type][bytes…][0x00]`.
  `PREF_OP_WRITE` parses the value as a C string (`UserPreferences.h:334`).
- **Impact:** comment-only mismatch. The app truncates at the first `\0` on read and appends
  `\0` on write (`lib/services/user_preferences_service.dart`, as reported by the user), so the
  format with `\0` is what both sides expect and is the contract. The same comment also says
  "UTF-8", while the actual encoding is single-byte keyboard-layout transliteration
  (see `docs/contracts/ble.md` §6.5).
- **Models:** same in all three.

### F5 — OTA service is not behind the Core auth gate

- **Where:** gate in `src/BLE/CoreService.cpp:867-873` applies only to the Core CMD
  characteristic. `src/BLE/FirmwareService.cpp` and `src/BLE/OtaProtocol.cpp` do not reference
  `authManager`.
- **Impact:** a connected client can run OTA without HELLO/PIN. Recorded for awareness;
  behavior is part of the current field contract.
- **Models:** same in all three.

### F6 — `MENU_OP_GOTO` accepts undefined menu codes

- **Where:** `src/BLE/CoreService.cpp:242-253`
- **What:** the comment at `:244` says undefined values are rejected, but only `0` (NONE) is.
  Any other byte is cast to `MenuCommand` and acknowledged with OK.
- **Models:** same in all three.

### F7 — `SYS_ERR_NAME_TOO_LONG` is defined but never sent

- **Where:** defined `src/BLE/CoreProtocol.h:284`; `SYS_OP_SET_NAME` over 24 bytes returns
  `CORE_ERR_INVALID_VALUE` (0x05) instead (`src/BLE/CoreService.cpp:795-798`).
- **Impact:** the app sees 0x05. The 0x05 response is the contract.
- **Models:** same in all three.

### F8 — Factory-reset trigger described differently in comments and code

- **Where:** `src/BLE/Authmanager.h:23-24` and `src/BLE/Authmanager.cpp:137` say "BACK+OK for 10
  seconds"; the only caller is `SetResetPassState::okAction`
  (`src/States/ConfigState.cpp:115-124`), a menu confirmation.
- **Impact:** comment-only mismatch. `factoryReset()` has no other caller (grep, all three
  models). Whether a button combination leads into that menu state was not checked here.
- **Models:** same in all three.
- **Update (7.06, `dcd2cdb` line numbers):** a second caller was added:
  `SetResetAllState::okAction` (`src/States/ConfigState.cpp:136-147`, menu "שחזור הגדרות יצרן").
  It calls `factoryReset()`, then `resetTextPrefs()` and `resetSimplePrefs()`, then `ESP.restart()`.
  `SetResetPassState::okAction` is now at `:116-125`. Both callers are menu states, same lines in
  all three models. The comment in `Authmanager.h` / `Authmanager.cpp` is unchanged, so the
  mismatch remains.
- **Status:** comment fix planned for the next production release. Open, planned.

### F9 — Preference defaults differ between the app and the firmware

- **Where (firmware):** `src/Preferences/UserPreferences.h`
  - `leil` (43): JC6416 / JC4020 default `0` (`:697`); JC6432 `1` (`:700`).
  - `colorYom` (18): JC6416 / JC4020 `6` (`:602`); JC6432 `1` (`:605`).
  - `colorParasha` (19): JC6416 / JC4020 `6` (`:605`); JC6432 `1` (`:608`).
- **Where (app, as reported by the user):** `lib/config/preference_constants.dart`,
  `lib/models/app_preferences.dart:90`, `lib/providers/ble/ble_preferences_mixin.dart:154-180`.
  App defaults: `leil` = 1, `colorYom` = 6, `colorParasha` = 6.
- **Mismatches:** `leil` on JC6416 and JC4020; `colorYom` and `colorParasha` on JC6432.
- **Impact:** `PREF_OP_SYNC` reports only values that differ from the *firmware* default. When
  the clock holds its firmware default, the index is not reported, and the app shows its own
  default, which is a different value. E.g. JC6416 with `leil` = 0 → app shows 1.
- **Action:** documentation only, no fix. Firmware defaults are part of the BLE contract
  (`docs/contracts/ble.md` §8).
- **Status:** won't fix (owner's decision). After the refactor the issue will no longer be
  relevant. Closed.

### F10 — Selecting a city overwrites `nz_hul`

- **Where:** `src/States/OfekState.cpp:510-519` (`SetOfekState::printUpdateSucc`), lines `:516-517`.
  Called from the menu (`SetOfekState::okAction`, `:67-71`) and from BLE `PREF_OP_WRITE ofek` ≠ 0
  (`src/BLE/CoreService.cpp:669-678`).
- **What:** since 7.05 (together with the fix for `PREF_OP_WRITE ofek` ≠ 0 from BLE) the
  function writes `nz_hul` (and `hul`) from `places[ofek].hul` of the selected city. Afterwards, when the user goes back to the custom place (`ofek` = 0), `nz_hul`
  still holds the city's value, not the value the custom place had before.
- **Impact:** `nz_hul` selects the DST rules (`src/DB/DayInfo.cpp:198-199`, Israel or USA) and,
  since 7.06, `zmanTalit` in `myZmanim::setDefault` (F15).
- **Status:** intentional. `nz_hul` is the Israel/abroad flag of the *active* place, and selecting
  a city sets it, even at the expense of the custom place's value. Closed.
- **Models:** same in all three.

### F11 — Value range of `shita` and of `zeetCochavim` grew in 7.05

- **What:** `numOfShitot` 8 → 10 (`shita` 0–9: `myZmanim` = 8, `pozen` = 9) and
  `numOfZeetCochavin` 27 → 28 (`zeetMethod` rows 0–28; new method 28 = 7.08°)
  (`src/DB/Shita.h:10, 12-13`). The arrays are indexed with the stored value without a check:
  - `shitot[userPreferences.shita]` in `DayInfo::initDay` (`src/DB/DayInfo.cpp:20`), at every day
    calculation including boot. Also `src/States/MinhagState.cpp:309-642` (10 places) and
    `src/States/ShitaState.cpp:38`.
  - `zeetMethod[...]` in the menu states (`src/States/MinhagState.cpp:310, 347, 606, 643`), with
    the temp value taken from `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom`
    (`src/Logic.cpp:68-77`, `MinhagState.cpp:315, 352, 611, 648`). The same list serves all four
    keys. The calculation itself (`Shita::zeetCochavimMethods`) is a `switch` with a `default`
    branch and does not index the array.
- **Risks:**
  - Rolling back to a firmware before 7.05 with a new value already stored: `shitot[8]` / `[9]`
    reads past the array of 8 → most likely a crash in `initDay` at every boot (boot loop).
    A stored 28 in one of the four `zeetMethod` keys reads past the array when that menu is
    shown. NVS is not reset by an install. `pozen::setDefault` itself stores 28 in
    `zeetCochavim` and `zeetZom`, so selecting that shita is enough.
  - Out-of-range writes from BLE.
- **Status:**
  - Since 7.07, out-of-range BLE writes of `shita` and `ofek` are rejected with
    `CORE_ERR_INVALID_VALUE` before `fromBytes` (`src/BLE/CoreService.cpp:653-659`,
    `docs/contracts/ble.md` §6.5).
  - `zeetCochavim` (and `rabenuTam`, `zeetShabat`, `zeetZom`) are not yet checked against
    `numOfZeetCochavin`. A BLE range check for all four is planned for the next production
    release. Open, planned.
  - Rollback to an older version: decided not to block it in the app, because it is a manual,
    deliberate install.
- **Models:** same in all three (`Shita.h`, `Shita.cpp` identical).

### F12 — `hardwareModel` defined `inline` only in JC6432

- **Where:** `src/Hardware.h:14`.
- **What:** in 7.06, JC6432 declared `inline HardwareModel hardwareModel;`, while JC6416 and
  JC4020 had a plain definition in a header.
- **Status:** `inline` in all three models since 7.07. Closed.

### F13 — Stale references to `HARDWARE_VERSION` (removed in 7.06)

- **What:** the `HARDWARE_VERSION` build flag was removed in 7.06. The hardware version is now
  detected at runtime (`detectHardwareModel` → `configureHardware` →
  `Metadata::setHardwareVersion`, `src/Hardware.h`). References that are no longer true:
  - `stamp_app_desc.py:20-21` docstring (all three models): says the script reads
    `HARDWARE_VERSION` from the build flags.
  - `CLAUDE.md`, hard contract 5.
  - `docs/environment.md:95` (snapshot).
  - Also `docs/architecture/dependencies.md:419` and `docs/architecture/notes.md:65`.
- **Status:**
  - `CLAUDE.md` is being updated now.
  - The docstring fix is planned for the next production release.
  - `environment.md` is a snapshot and is updated only at the next export.
  - `dependencies.md` and `notes.md` are being updated with the 7.07 docs sync.
  - Open, planned.

### F14 — `DayInfo` changes that existed only in JC6432

- **Where:** `src/DB/DayInfo.cpp`, `DayInfo::setAfterShkiaa` (JC6416 / JC4020 `:946-1009`,
  JC6432 `:964-1027`) and `DayInfo::setBooleanValues` (JC6416 / JC4020 `:742`, JC6432 `:760`).
- **What:**
  - `setAfterShkiaa` calls `setHul()` when `hul` is on (JC6416 / JC4020 `:1005`, JC6432 `:1023`),
    and sets `afterShkiaa = true` at the end instead of the start.
  - The assignment `parasha_hul = tomorrow->parasha_hul` is commented out.
  - `this->isShaonKaiz = _shaonKaiz` is commented out.
- **Status:** ported to JC6416 and JC4020 in 7.07; the three models are aligned. Closed.

### F15 — `zmanTalit` is not updated after selecting a city

- **Where:** `src/DB/Shita.cpp:421-422` (`myZmanim::setDefault`, `:415-430`);
  `src/States/OfekState.cpp:510-519` (`SetOfekState::printUpdateSucc`).
- **What:** `myZmanim::setDefault` picks `zmanTalit` by `nz_hul`: 7 (10.2°) when `nz_hul` = 1,
  otherwise 10 (11.5°). Selecting a city changes `nz_hul` (F10), but the shita's `setDefault`
  does not run again, so `zmanTalit` stays as computed for the previous `nz_hul`.
- **Order in `SetOfekState::printUpdateSucc`:** success message; `nerot`, `homa` from the city
  (`:513-514`); `initPlace()` (`:515`); `nz_hul`, `hul` from the city (`:516-517`);
  `configStates()`; `updateDayValues`. So `initPlace()` builds `places[0]` with the *previous*
  `nz_hul`. No effect when `ofek` ≠ 0, because `place` points to `places[ofek]`, not `places[0]`.
- **Status:** won't fix (owner's decision: keep as is). Closed.
- **Models:** same in all three.

### F16 — `dayIndex` is written every day and never read

- **Where:** `src/States/MainState.cpp:285` (`MainState::setNewDay`): `userPreferences.dayIndex= idx;`.
  Definition: `src/Preferences/UserPreferences.h:542` (key `dayIndex`, U8, default 1, index 3).
- **What:** the preference was meant to keep the day index, to restore the date after a power
  and battery loss. No code reads it. On JC6416 it is written at every day calculation, so its
  value almost always differs from the default.
- **Indexes are explicit:** every `UserPref` gets its BLE index as a constructor argument, and
  `getIndex()` returns that value. Nothing derives an index from the position in `allPrefs` or
  `simplePrefs`. The only uses of the index are in `src/BLE/CoreService.cpp`: `PREF_OP_SYNC`
  (`:707`) and `getIndex() == 4` / `== 6` (`:654-655`, `:670`, `:680`). `PREF_OP_READ` and
  `PREF_OP_WRITE` look up by key. Removing the definition does not move any other index; index 3
  becomes a gap.
- **Effect of removal on the firmware:**
  - `PREF_OP_SYNC` no longer reports index 3. Today it reports it almost always on JC6416.
  - `PREF_OP_READ` / `PREF_OP_WRITE` with key `dayIndex` return `CORE_ERR_INVALID_KEY`.
  - The NVS key `dayIndex` stays in `clk` as an orphan entry (no active erase). No behavior
    effect.
  - Stopping the write alone would freeze the last value, and `SYNC` would keep reporting
    index 3 on existing clocks until a reset.
- **Effect on the app** (per the app source, as reported by the user:
  `lib/config/preference_constants.dart`, `lib/models/app_preferences.dart:13`): `dayIndex` is
  defined at index 3 and not used beyond the definition. After the removal `SYNC` does not
  return 3, and the app does not request it. Safe with existing app versions.
- **Status:** removal planned for the next production release. Open, planned.
- **Models:** only JC6416 writes it. JC6432 has the write commented out
  (`src/States/MainState.cpp:310`); JC4020 does not write it. The definition (index 3) is the
  same in all three.

### F17 — Diagnostics screen shows `leil` twice

- **Where:** `src/Screens/RegsScreen.cpp:242-243` (`dcd2cdb`; `:243-244` after 2a.2.2).
- **What:** areas `d1` and `d2` both print `leil` and its default. Probably a copy-paste error;
  `d2` was likely meant for another preference.
- **Impact:** diagnostics screen only. No effect on preferences, NVS or BLE.
- **Status:** documentation only (owner's decision). No code change.
- **Models:** same in all three (JC6416 / JC4020 `:242-243`, JC6432 `:242-243`).

### F18 — `SetTimeFormat`: self-assignment in `menuAction` and `okAction`

- **Where:** `src/States/SetClockState.cpp:229` (`menuAction`) and `:243` (`okAction`)
  (`dcd2cdb`; `:230` / `:244` after 2a.2.2).
- **What:** both assign `timeFormat` to itself. Both sides are `UserPref<uint8_t>`, so overload
  resolution picks the implicit copy assignment, not `UserPref::operator=(T)`
  (`src/Preferences/UserPreferences.h:86`): a memberwise self-copy, no write to NVS. The actual
  write happens immediately in `upAction` / `downAction` (`:233-234`, `:238-239`), unlike the
  other menu states, which edit a temp value and write it in `okAction`.
- **Impact:** none on behavior. The two lines are no-ops; the menu writes `timeFormat` to NVS on
  every up/down press.
- **Status:** documentation only (owner's decision). No code change.
- **Models:** same in all three (JC6416 / JC4020 `:229`, `:243`; JC6432 `:230`, `:244`).
