# Refactor log

One entry per approved step. Anything that looks like a bug or a contract inconsistency is
recorded under **Findings** and is not fixed in code as part of a refactor step.

## Steps

### Stage 0 / A — contracts extraction

- **A.1** — Created `docs/refactor-log.md` (this file) with initial Findings.
  No code changes.
- **A.2** — Created `docs/contracts/ble.md`. Extended F3; added F4–F9. App-side facts
  (encoding, MTU, defaults) taken from the app source as reported by the user. No code changes.

## Findings

Line numbers refer to commit `e9c7639`.

### F1 — `metadata.cpp` includes `metadata.h`, file is `Metadata.h`

- **Where:** `JC6416/src/metadata.cpp:1`, `JC4020/src/metadata.cpp:1`, `JC6432/src/metadata.cpp:1`
- **What:** `#include "metadata.h"`, while the header on disk is `src/Metadata.h`. All other
  includers use `"Metadata.h"` (e.g. `src/BLE/Bluetooth.h:6`).
- **Impact:** builds only on a case-insensitive filesystem (Windows). Would break a
  Linux/CI build. No effect on the produced binary.
- **Models:** same in all three.

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
