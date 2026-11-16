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

## Planned for next production release

Decided by the owner. To be done in the production projects, then ported here with the next sync.

| Finding | Change |
|---|---|
| F11 | `PREF_OP_WRITE`: range check of `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom` against `numOfZeetCochavin`, like the 7.07 check of `shita` / `ofek` |
| F8 | Fix the factory-reset trigger comment in `Authmanager.h` / `Authmanager.cpp` |
| F13 | Fix the `stamp_app_desc.py` docstring (no `HARDWARE_VERSION` build flag) |

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
