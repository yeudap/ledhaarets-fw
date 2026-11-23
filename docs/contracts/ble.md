# BLE contract (Core service)

The BLE protocol as implemented in production **7.09**, and in **709A**, which does not change
`BLE/` or `Preferences/` (see [../baseline.md](../baseline.md)). This is the contract in the field.
How older firmware that the app may still meet behaves differently: §9. The OTA service contract
(not written yet) and the advertisement contract (not written yet) are separate.

References are to `JC6416/src/...` by file and symbol (function, class, macro, enum), unless
stated otherwise. Where a file is identical in all three models (verified by md5), the same
references apply to JC4020 and JC6432. The only line numbers are in references to the app source,
as reported by the user.

## 0. Sources and per-model identity

| File | JC6416 | JC4020 | JC6432 |
|---|---|---|---|
| `BLE/CoreProtocol.h` | f2bfe8aa | same | same |
| `BLE/CoreService.cpp` / `.h` | c565e9d1 / 67202d74 | same | same |
| `BLE/Authmanager.cpp` / `.h` | b534df65 / b07d9c83 | same | same |
| `BLE/Bluetooth.cpp` | 9f1f249e | same | same |
| `BLE/Commands.h` | a8dccfc1 | same | same |
| `BLE/TextCommandHandler.h` | 6ceaa330 | same | same |
| `BLE/TextCommandHandler.cpp` | 7b672581 | **a34f9a84 (differs)** | 7b672581 |
| `Preferences/UserPreferences.h` | 1674adef | 1674adef | **b6fd22d0 (differs)** |
| `States/States.h` (`numOfNameSets`) | 50 | **0** | 50 |
| `States/ShitaState.cpp` (called from PREF WRITE `shita`) | dd0d3011 | same | same |
| `States/OfekState.cpp` / `ConfigState.cpp` | files differ per model | | |

`OfekState.cpp` and `ConfigState.cpp` differ between models as files, but the functions the BLE
contract depends on (`SetCustomState::printUpdateSucc`, `SetOfekState::printUpdateSucc`,
`SetResetPassState::okAction`, `SetResetAllState::okAction`) are identical in all three
(compared function by function).

**Model differences in the BLE contract** (details in §6.3 and §8):
1. **JC6432:** the firmware defaults of `colorYom` and `colorParasha` differ (§8). See
   refactor-log F9. Keys, indexes and types are the same in all three models.
2. **JC4020:** TEXT domain has 0 slots and write/display are disabled (§6.3). See refactor-log F3.

Everything else in this document is identical in all three models.

## 1. GATT layout

| Item | Value | Source |
|---|---|---|
| Core service UUID | `7b79e39b-488a-4f76-a60f-bd3a29da174f` | `CORE_SERVICE_UUID` in `BLE/CoreService.h` |
| CMD characteristic UUID | `326109ee-bcea-4809-88b1-5e0da1289a8c`, property `WRITE` | `CORE_CMD_CHAR_UUID` in `CoreService.h`; `initCoreService` in `CoreService.cpp` |
| EVT characteristic UUID | `fbe9b56e-270b-4765-a3ec-b917bb0ad0e1`, property `NOTIFY` | `CORE_EVT_CHAR_UUID` in `CoreService.h`; `initCoreService` in `CoreService.cpp` |
| OTA service UUID (OTA contract: not written yet) | `85564e0a-542d-4967-9d09-892b864a1b46` | `OTA_SERVICE_UUID` in `BLE/FirmwareService.h` |
| GAP init name | `"LedHaarets"` | `NimBLEDevice::init` in `initBluetooth`, `BLE/Bluetooth.cpp` |
| Preferred MTU | 517 (firmware preference only, see note) | `NimBLEDevice::setMTU` in `initBluetooth`, `Bluetooth.cpp` |
| Device name (GAP + scan response) | NVS `sys`/`dev_name`, default `"LedHaarets"`, max 24 bytes | `loadDeviceName`, `applyDeviceName` in `CoreService.cpp`; `SYS_NAME_MAX_BYTES`, `SYS_DEFAULT_DEVICE_NAME` in `CoreProtocol.h` |

**MTU note.** 517 is only what the firmware offers. The negotiated MTU is the minimum of both
sides: the app requests **150** (`lib/services/ble_service/ble_connection.dart:64`, app source as
reported by the user). On Android the effective ATT payload is therefore ≤ 147 bytes
(MTU − 3). Any frame size assumption must hold at 147 bytes. This is directly relevant to the
OTA chunk size (`CHUNK_SIZE_DEFAULT` / `SAFE_CHUNK_SIZE` = 244; OTA contract: not written yet).

Connection callbacks (`MyServerCallbacks::onConnect` / `onDisconnect`, `Bluetooth.cpp`): on connect and on disconnect the auth session is
reset (`authManager.resetSession()`); advertising restarts on disconnect.

## 2. Frame format

CMD (app → clock, written to CMD char) — header comment of `CoreProtocol.h`:

```
[0] DOMAIN  [1] OPCODE  [2..N] PAYLOAD
```

EVT (clock → app, Notify on EVT char) — header comment of `CoreProtocol.h`, built in `coreSendEvt` (`CoreService.cpp`):

```
[0] DOMAIN (echo)  [1] OPCODE (echo)  [2] STATUS  [3..N] DATA
```

- `CORE_HEADER_SIZE` = 2, `CORE_EVT_HEADER_SIZE` = 3 (`CoreProtocol.h`).
- On error: `STATUS = 0x01`, `DATA[0]` = error code (`sendErr`, `CoreService.cpp`).
- CMD shorter than 2 bytes is dropped silently, no EVT (`CoreCallbacks::onWrite`, `CoreService.cpp`).
- An EVT is sent only if connected and the EVT char exists (`coreSendEvt`).

### Byte order

| Field | Order | Source |
|---|---|---|
| PIN in `AUTH_OP_PIN` | **little-endian** u16 | `handleAuth`, `CoreService.cpp` |
| PIN via `PREF_OP_WRITE "pin"` | **little-endian** u16 | `handlePref`, `CoreService.cpp` |
| Preference values I16/U16 (and I32/U32 in code, unused) | **little-endian** | `UserPref<T>::toBytes` / `fromBytes`, `UserPreferences.h` |
| Preference BLOB (double) | raw 8 bytes, IEEE 754, little-endian (ESP32 native) | `UserPref<T>::toBytes` / `fromBytes`, `UserPreferences.h` |
| **Year in TIME data** | **big-endian** (MSB first) | time data comment in `CoreProtocol.h`; `handleTime` (SET and GET), `CoreService.cpp` |

## 3. Status and error codes

STATUS (`CORE_STATUS_OK`, `CORE_STATUS_ERROR` in `CoreProtocol.h`): `0x00` OK, `0x01` ERROR.

Error codes: the `CORE_ERR_*` macros in `CoreProtocol.h`.

| Code | Name | Meaning |
|---|---|---|
| 0x01 | `CORE_ERR_UNKNOWN_DOMAIN` | unknown domain |
| 0x02 | `CORE_ERR_UNKNOWN_OPCODE` | unknown opcode in domain |
| 0x03 | `CORE_ERR_INVALID_PAYLOAD` | payload too short / malformed |
| 0x04 | `CORE_ERR_INVALID_INDEX` | index out of range (TEXT) |
| 0x05 | `CORE_ERR_INVALID_VALUE` | invalid value |
| 0x06 | `CORE_ERR_EMPTY` | empty record |
| 0x07 | `CORE_ERR_WRITE_FAILED` | storage write failed |
| 0x08 | `CORE_ERR_TYPE_MISMATCH` | pref type ≠ key type |
| 0x09 | `CORE_ERR_INVALID_KEY` | pref key not found |
| 0x0A | `CORE_ERR_INVALID_TIME` | invalid date/time |
| 0x0B | `CORE_ERR_AUTH_REQUIRED` | HELLO: unknown clientId |
| 0x0C | `CORE_ERR_AUTH_FAILED` | wrong PIN (after 1 s) |
| 0x0D | `CORE_ERR_NOT_AUTHENTICATED` | non-AUTH command before auth |
| 0xFF | `CORE_ERR_INTERNAL` | internal |

SYS-specific: `SYS_ERR_NAME_TOO_LONG` = 0x01 (`CoreProtocol.h`) is defined but **not sent**;
an over-long name returns `CORE_ERR_INVALID_VALUE` (`handleSys`, `SYS_OP_SET_NAME`,
`CoreService.cpp`). See refactor-log F7.

## 4. Domains

Domain codes: the `DOMAIN_*` macros in `CoreProtocol.h`. Handlers: `CoreService.cpp`.

| Code | Domain | Handler |
|---|---|---|
| 0x01 | NAV | `handleNav` |
| 0x02 | MENU | `handleMenu` |
| 0x03 | TEXT | `handleText` |
| 0x04 | TIME | `handleTime` |
| 0x05 | PREF | `handlePref` |
| 0x06 | AUTH | `handleAuth` |
| 0x07 | SYS | `handleSys` |

Dispatch: `CoreCallbacks::onWrite`, `CoreService.cpp`. Unknown domain → `UNKNOWN_DOMAIN`,
echoing the received domain/opcode (the `default` case of the dispatch `switch`).

## 5. Authentication

### 5.1 Gate

Single gate in `CoreCallbacks::onWrite` (`CoreService.cpp`), before the dispatch: any domain other
than AUTH (0x06) while the session is not authenticated → `ERROR + NOT_AUTHENTICATED`, echoing the
received domain/opcode. Session flag is per-connection and reset on connect/disconnect
(`MyServerCallbacks::onConnect` / `onDisconnect`, `Bluetooth.cpp`).

**Scope:** the gate covers the Core service only. The OTA service (`FirmwareService.cpp`,
`OtaProtocol.cpp`) does not reference `authManager`. See refactor-log F5.

### 5.2 Opcodes (`AUTH_OP_*` in `CoreProtocol.h`, handler `handleAuth` in `CoreService.cpp`)

| Op | Name | CMD | EVT | Behavior |
|---|---|---|---|---|
| 0x01 | `AUTH_OP_HELLO` | `[06][01][clientId 16B]` (≥18 B) | `[06][01][STATUS]` | Trusted clientId → OK, `grantSession`, info message on screen. Otherwise `ERROR + AUTH_REQUIRED`. Short → `INVALID_PAYLOAD`. (`case AUTH_OP_HELLO`) |
| 0x02 | `AUTH_OP_PIN` | `[06][02][pin u16 LE][clientId 16B]` (≥20 B) | `[06][02][STATUS]` | Correct → OK, `grantSession` (no NVS write). Wrong → `delay(1000)` then `ERROR + AUTH_FAILED`. (`case AUTH_OP_PIN`) |
| 0x03 | `AUTH_OP_REGISTER` | `[06][03]` | `[06][03][STATUS]` | Requires authenticated session, else `NOT_AUTHENTICATED`. Stores session clientId as trusted (FIFO). Idempotent. (`case AUTH_OP_REGISTER`) |
| 0x04 | — | reserved for future UNREGISTER, not implemented (comment after `AUTH_OP_REGISTER` in `CoreProtocol.h`) → `UNKNOWN_OPCODE` | | |

### 5.3 PIN and trusted clients (`Authmanager.cpp`, `Authmanager.h`, the `AUTH_*` macros in `CoreProtocol.h`)

| Item | Value | Source |
|---|---|---|
| clientId size | 16 bytes (raw UUID v4) | `AUTH_CLIENT_ID_SIZE` and the AUTH flow comment in `CoreProtocol.h` |
| Trusted slots | 3, FIFO eviction (oldest = slot 0 dropped) | `AUTH_TRUSTED_SLOTS` in `CoreProtocol.h`; `AuthManager::addTrusted` in `Authmanager.cpp` |
| All-zero clientId | never trusted, never stored | `AuthManager::isTrusted`, `AuthManager::addTrusted` (`_isAllZero`) |
| Default PIN / after factory reset | 1111 | `AUTH_DEFAULT_PIN` in `CoreProtocol.h`; `AuthManager::init`, `AuthManager::factoryReset` |
| PIN range | 0–9999 (> 9999 → `INVALID_VALUE`) | `AUTH_MAX_PIN` in `CoreProtocol.h`; `handlePref`, `"pin"` special case, `CoreService.cpp` |
| Wrong PIN delay | 1000 ms, blocking in NimBLE host task | `AUTH_FAIL_DELAY_MS` in `CoreProtocol.h`; `handleAuth`, `case AUTH_OP_PIN` |
| PIN change | `PREF_OP_WRITE`, key `"pin"`, type must be U16 (0x03), value ≥ 2 B LE; routed to `authManager.setPin`; requires authenticated session | `handlePref`, `"pin"` special case (`AUTH_PIN_PREF_KEY`), `CoreService.cpp` |
| PIN readable? | No: `"pin"` is not in `simplePrefs`, so not readable via `PREF_OP_READ` and not in `PREF_OP_SYNC` | `handlePref`, `case PREF_OP_READ` (comment on the `simplePrefs` search); `AUTH_PIN_PREF_KEY` comment in `CoreProtocol.h` |
| NVS writes | PIN only on change; slots only when a *new* client registers | `AuthManager::setPin`, `AuthManager::addTrusted` |
| Factory reset | PIN = 1111, all slots cleared, session reset. Two menu callers (below) | `AuthManager::factoryReset` |

`factoryReset()` callers (`States/ConfigState.cpp`, the same functions in all three models):

| Caller | What it does |
|---|---|
| `SetResetPassState::okAction` | `factoryReset()`, then `ESP.restart()` |
| `SetResetAllState::okAction` (menu "שחזור הגדרות יצרן") | `factoryReset()`, `resetTextPrefs()`, `resetSimplePrefs()`, then `ESP.restart()`. Clears `sec` and resets all of `clk` to firmware defaults; `sys/dev_name` is not touched |

There is no BLE opcode for factory reset. Both are menu states, reachable from the buttons (or
by simulating them over NAV/MENU).

NVS keys (`sec` namespace): `pin` (u16) and `trusted` (blob); the NVS contract is not written yet.

## 6. Domain details

### 6.1 NAV (0x01) — `NAV_*` in `CoreProtocol.h`, `handleNav` in `CoreService.cpp`

| Op | CMD | EVT |
|---|---|---|
| 0x01 `NAV_OP_PRESS` | `[01][01][key]` (≥3 B) | `[01][01][STATUS]` |

Key codes → internal `ButtonCommand` (`enum class ButtonCommand`, `Commands.h`):

| Key | Wire | `ButtonCommand` |
|---|---|---|
| UP | 0x01 | `UP` = 2 |
| DOWN | 0x02 | `DOWN` = 3 |
| MENU | 0x03 | `MENU` = 1 |
| OK | 0x04 | `OK` = 4 |

Other key → `INVALID_VALUE`. Short packet → `INVALID_PAYLOAD` with opcode `NAV_OP_PRESS`
(not echoed). Wire codes and internal enum values differ; mapping is explicit (the key `switch`
in `handleNav`).

### 6.2 MENU (0x02) — `MENU_*` in `CoreProtocol.h`, `handleMenu` in `CoreService.cpp`

| Op | CMD | EVT |
|---|---|---|
| 0x01 `MENU_OP_GOTO` | `[02][01][menu_code]` (≥3 B) | `[02][01][STATUS]` |

`menu_code` is cast directly to `MenuCommand` (`enum class MenuCommand`, `Commands.h`); values are identical to
`MENU_CODE_*`:

| Group | Codes |
|---|---|
| Main | 1 MAIN, 2 CLOCK, 3 DISPLAY, 4 MINHAG, 5 MAKOM, 6 ZMANIM, 7 CONTENT, 8 RESET, 9 ABOUT, 10 MAIN_SCREEN, 11 CONFIG |
| Clock | 20 SET_DATETIME, 21 TIME_FORMAT, 22 DATE_FORMAT, 23 TIME_STRUCT, 24 NEXT_DAY, 25 DST |
| Display | 40 COLOR, 41 BRIGHTNESS, 42 FONT |
| Location | 50 SELECT_HORIZON, 51 SET_LOCATION, 52 WALLED_CITY |
| Time settings | 60 SELECT_METHOD, 61 DESTRUCTION_YEARS, 62 EARLY_CANDLES, 63 SHOW_ALL_TIMES |
| Content | 70 EDIT, 71 ENTER_TEXT, 72 CHANGE_RATE |
| Customs | 80 DIASPORA |

Validation: only `0` (NONE) is rejected (`INVALID_VALUE`). Undefined values (e.g. 12, 99) are
accepted with OK (`handleMenu`, the validity check after the cast to `MenuCommand`), despite the
comment above that check. See refactor-log F6.

### 6.3 TEXT (0x03) — `TEXT_*` in `CoreProtocol.h`, `handleText` in `CoreService.cpp`, `TextCommandHandler.cpp`

| Op | CMD | EVT data on OK |
|---|---|---|
| 0x01 `TEXT_OP_READ` | `[03][01][index]` (≥3 B) | text bytes, no terminator (empty slot → OK with 0 bytes) |
| 0x02 `TEXT_OP_WRITE` | `[03][02][index][text bytes]` (≥4 B; text = bytes 3..end) | `[index]` |
| 0x03 `TEXT_OP_DELETE` | `[03][03][index]` (≥3 B) — compacts the list | `[index]` |
| 0x04 `TEXT_OP_DISPLAY` | `[03][04][index]` (≥3 B) | `[index]` |
| 0x05 `TEXT_OP_SWAP` | `[03][05][idx_a][idx_b]` (≥4 B) | `[idx_a][idx_b]` |
| 0x06 `TEXT_OP_COUNT` | `[03][06]` | `[count]` (non-empty slots) |

Slots: `numOfNameSets` (index 0..N-1), backed by NVS keys `textInput1`..`textInput50`.

Text encoding: **not UTF-8**. Same single-byte keyboard-layout encoding as PREF STR, see
"STR wire format" in §6.5.

Internal text error → CORE error mapping (`_mapTextError`, `CoreService.cpp`; internal codes:
the `ERR_*` macros in `TextCommandHandler.h`):

| Internal | → CORE |
|---|---|
| `ERR_INVALID_INDEX` 0x01 | `CORE_ERR_INVALID_INDEX` 0x04 |
| `ERR_INVALID_FORMAT` 0x03 | `CORE_ERR_INVALID_VALUE` 0x05 |
| `ERR_EMPTY_TEXT` 0x04 | `CORE_ERR_EMPTY` 0x06 |
| `ERR_WRITE_FAILED` 0x07 | `CORE_ERR_WRITE_FAILED` 0x07 |
| anything else (`ERR_MEMORY`, `ERR_UNSUPPORTED`, `ERR_READ_FAILED`, `ERR_DELETE_FAILED`, …) | `CORE_ERR_INTERNAL` 0xFF |

`TextCommandHandler.h` also defines legacy command codes (`READ_TEXT` 0xF8, `WRITE_TEXT`
0xF7, `DELETE_TEXT` 0xFD, `DISPLAY_TEXT` 0xFC, `SWAP_TEXTS` 0xFB, `GET_TEXT_COUNT` 0xF9,
`SUCCESS_CODE` 0x00, `ERROR_CODE` 0xFF). They are not used on the wire by the Core service.

> **⚠ Model difference — JC4020**
> - `numOfNameSets = 0` (`JC4020/src/States/States.h`): every index is invalid, so READ, WRITE,
>   DELETE, DISPLAY, SWAP always return `ERROR + INVALID_INDEX (0x04)`; COUNT returns OK with `0`.
> - Additionally `handleWriteText` and `handleDisplayText` (`JC4020/src/BLE/TextCommandHandler.cpp`)
>   have their bodies commented out and return
>   `ERR_INVALID_INDEX` unconditionally.
> - JC6416 / JC6432: 50 slots, full behavior.

### 6.4 TIME (0x04) — `TIME_*` in `CoreProtocol.h`, `handleTime` in `CoreService.cpp`

| Op | CMD | EVT |
|---|---|---|
| 0x01 `TIME_OP_SET` | `[04][01][7B time]` (≥9 B) | `[04][01][STATUS]`; invalid date → `INVALID_TIME` |
| 0x02 `TIME_OP_GET` | `[04][02]` | `[04][02][00][7B time]` |

Time data (7 B, `TIME_DATA_SIZE`):

| Byte | Field |
|---|---|
| 0 | year MSB |
| 1 | year LSB |
| 2 | month 1–12 |
| 3 | day 1–31 |
| 4 | hour 0–23 |
| 5 | minute 0–59 |
| 6 | second 0–59 |

**Year is big-endian**, unlike every other multi-byte field in this protocol.

### 6.5 PREF (0x05) — `PREF_*` in `CoreProtocol.h`, `handlePref` in `CoreService.cpp`

| Op | CMD | EVT data on OK | Search scope (`handlePref`, case of the op) |
|---|---|---|---|
| 0x01 `PREF_OP_READ` | `[05][01][key\0]` (≥4 B) | `[type][value bytes]` | `simplePrefs` only |
| 0x02 `PREF_OP_WRITE` | `[05][02][type][key\0][value bytes]` (≥5 B) | — | `allPrefs` (= `textPrefs` + `simplePrefs`) + special key `"pin"` |
| 0x03 `PREF_OP_SYNC` | `[05][03]` | `[count][index…]` for every `simplePrefs` entry whose value ≠ default. Never contains index 3 (§8) | `simplePrefs` |

WRITE checks, in order:

1. No `\0` after key → `INVALID_KEY`.
2. Type ≠ declared type → `TYPE_MISMATCH`.
3. **Range check (`handlePref`, `case PREF_OP_WRITE`)**,
   before `fromBytes`, on the first value byte:
   - index 4 (`shita`): value ≥ `numOfShitot` (`DB/Shita.h`, 10) → `INVALID_VALUE`;
   - index 6 (`ofek`): value ≥ `numOfOfeks` (size of `places[]`, set in `initPlace`,
     `DB/Place.cpp`) → `INVALID_VALUE`;
   - `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom`: value >
     `numOfZeetCochavin` (`DB/Shita.h`, 28) → `INVALID_VALUE`. The upper bound is allowed
     (`>`, not `≥`), because `zeetMethod` has `numOfZeetCochavin + 1` rows (0–28). These four keys
     are identified by preference address, not by index;
   - empty value for any of these keys → `INVALID_VALUE`.

   Nothing is written. The accepted range follows the firmware tables, so adding a shita, a city
   or a zeet method widens it. Older firmware without these checks: §9.
4. `fromBytes` failure (too short) → `INVALID_VALUE`.
5. Unknown key → `INVALID_KEY`.

On success the value is saved to NVS (`save()`), the OK EVT is sent, and then for two keys an
apply step runs.

#### Apply after WRITE

Both apply steps run inside the write callback, after the OK EVT. Each one temporarily switches
`currentState` to `bl.updateState`, calls the same function the menu uses, then restores
`currentState` and redraws it. Every other key is saved only, with no apply step.

The apply steps are in `handlePref`, `case PREF_OP_WRITE`, after `sendOk` (`CoreService.cpp`).

| Idx | Key | Value | Function called | Defined in |
|---|---|---|---|---|
| 6 | `ofek` | 0 | `SetCustomState::printUpdateSucc` | `OfekState.cpp` |
| 6 | `ofek` | ≠ 0 | `SetOfekState::printUpdateSucc` | `OfekState.cpp` |
| 4 | `shita` | any (after range check) | `ShitaState::setShita` | `ShitaState.cpp` |

- **`ofek` = 0 ("apply custom place"):** success message, `globalchangedOfek = false`,
  `initPlace()`, `ofek = 0`, `bl.configStates()`, `updateDayValues = true`. This is the last
  write of the app's custom-place sequence.
- **`ofek` ≠ 0 (city from the list):** success message; writes `nerot` and `homa` from
  `places[ofek]`; `initPlace()`; writes `nz_hul` and `hul` = `places[ofek].hul`;
  `bl.configStates()`; `updateDayValues = true`. Same function as the menu
  (`SetOfekState::okAction`).
  **Side effects on the wire:** a city write changes four more keys in NVS (`nerot`, `homa`,
  `nz_hul`, `hul`), and they show up in the next `PREF_OP_SYNC`. `nz_hul` is the Israel/abroad
  flag of the active place, so a city overwrites the value the custom place had (refactor-log F10).
- **`shita`:** `shitot[shita]->setDefault()` (writes the calculation-method preferences of that
  shita), `bl.configStates()`, `updateDayValues = true`, success message. Same function as the
  menu (`ShitaState::okAction`). The app writes `shita` and then reads the method preferences
  back, so it reads the values set by `setDefault`.

Note: textInput keys are writable through `PREF_OP_WRITE` (they are in `allPrefs`) but are not
readable through `PREF_OP_READ` and all share index 0.

#### Type codes (`PREF_TYPE_*` in `CoreProtocol.h`)

The wire value is `static_cast<uint8_t>(PreferenceType)` from Arduino `Preferences.h`
(`PT_I8, PT_U8, PT_I16, PT_U16, PT_I32, PT_U32, PT_I64, PT_U64, PT_STR, PT_BLOB` = 0..9).

| Code | Name | Firmware C++ type | Wire value | Used by |
|---|---|---|---|---|
| 0x00 | `PREF_TYPE_I8` | `int8_t` | 1 B | `timezone` |
| 0x01 | `PREF_TYPE_U8` | `uint8_t` | 1 B | most keys |
| 0x02 | `PREF_TYPE_I16` | `int16_t` | 2 B LE | `nz_height` |
| 0x03 | `PREF_TYPE_U16` | `uint16_t` | 2 B LE | `interval*`, `"pin"` |
| 0x08 | `PREF_TYPE_STR` | `String` | 1 byte per character, keyboard-layout encoding, trailing `0x00` (see note) | `textPlace`, `textInput*` |
| 0x09 | `PREF_TYPE_BLOB` | `double` (`double_t`) | 8 B IEEE 754 LE | `nz_lat`, `nz_long` |

**BLOB = double (verified).** `PT_BLOB` is the Arduino enum name; in this firmware it is used
only for `UserPref<double_t>` (`nz_lat`, `nz_long`; production: members and constructor of
`UserPreferences` in `UserPreferences.h`; in this repo since 2a.3.3: `PlacePrefs` in
`DataPrefs.h` / `DataPrefs.cpp`). Wire encoding is a raw `memcpy` of the 8-byte double
(`UserPref<T>::toBytes` / `fromBytes`, `double` branch, in `UserPreferences.h`; requires ≥ 8 B),
little-endian on ESP32. In NVS it is stored with `putDouble`, which the framework implements as
`putBytes` (8 B blob). This matches the app (`lib/config/core_protocol.dart:147`, 8-byte double,
as reported by the user). Naming note only, not a Finding.

**STR wire format:**

- **Encoding — not UTF-8.** The app transliterates Hebrew to Latin characters by keyboard
  layout (א→`t`, ש→`a`, …; `lib/utils/text_processor.dart`) and sends one byte per character
  (`codeUnits`). The same encoding is used for TEXT domain strings (§6.3). App-side maximum:
  70 characters. (App source as reported by the user.) The firmware stores and returns the bytes
  as-is; the display maps them to Hebrew glyphs. The default `textPlace` value `"whaht otwun"`
  is in this encoding. The comment "UTF-8" on `PREF_TYPE_STR` in `CoreProtocol.h` does not describe
  the actual encoding.
- **Only `SYS_OP_SET_NAME` is UTF-8** (app: `utf8.encode`; firmware validates length in bytes,
  §6.6).
- **Terminator:** `PREF_OP_READ` returns the string bytes **plus a trailing `0x00`**
  (`UserPref<T>::toBytes`, `String` branch, `UserPreferences.h`), contrary to the comment on
  `PREF_TYPE_STR` in `CoreProtocol.h`. `PREF_OP_WRITE` parses the value as a C string
  (`UserPref<T>::fromBytes`, `String` branch), i.e. up to the first `\0`. The app
  truncates at the first `\0` on read and appends `\0` on write
  (`lib/services/user_preferences_service.dart`). **The format with `\0` is what both sides
  expect.** See refactor-log F4.

### 6.6 SYS (0x07) — `SYS_*` in `CoreProtocol.h`, `handleSys` in `CoreService.cpp`

| Op | CMD | EVT | Behavior |
|---|---|---|---|
| 0x01 `SYS_OP_RESTART` | `[07][01]` | `[07][01][00]` | OK sent first; `esp_restart()` after 500 ms timer (`scheduleRestart`, `restartTimerCallback`; `case SYS_OP_RESTART`) |
| 0x02 `SYS_OP_SET_NAME` | `[07][02][UTF-8 name]` | `[07][02][STATUS]` | > 24 bytes → `INVALID_VALUE`. Empty → delete NVS key, use default `"LedHaarets"`. Applies to GAP name + scan response immediately (`case SYS_OP_SET_NAME`, `applyDeviceName`) |
| 0x03 `SYS_OP_GET_MAC` | `[07][03]` | `[07][03][00][MAC 6B]` | `esp_efuse_mac_get_default` (`case SYS_OP_GET_MAC`) |

## 7. Version gate

`MIN_FW_MAJOR_VERSION` = 7 (`CoreProtocol.h`): documented as "a clock with major < 7 must
be OTA-updated before use". The firmware itself does not reference it; it is a constant shared
with the app. Protocol version per comment: 1.1 (header comment of `CoreProtocol.h`).

## 8. Preference indexes

Indexes are returned by `PREF_OP_SYNC`, and the app maps them to keys. Keys and types are
also the NVS keys in namespace `clk` (NVS contract: not written yet).

**Firmware defaults are part of the contract.** `PREF_OP_SYNC` returns only the indexes whose
value differs from the *firmware* default (`handlePref`, `case PREF_OP_SYNC`, `CoreService.cpp`;
`UserPref<T>::_hasChangedFromDefault`, `UserPreferences.h`). For every index not returned, the app assumes its *own* default.
Changing a firmware default therefore changes what the app displays for clocks still at
default, and any mismatch between app and firmware defaults is visible to the user.
Known mismatches: refactor-log F9 (marked † in the table below).

**Index 3 is not used.** SYNC returns `getIndex()` of the `simplePrefs` entries only
(`handlePref`, `case PREF_OP_SYNC`), and no entry has index 3. Clocks may still hold the key
`dayIndex` in `clk` as an orphan NVS entry; nothing reads it.

Declarations: one initializer per key (key, default, type, index). Production: the
`UserPreferences` constructor in `UserPreferences.h`. In this repo since 2a.3.3: the constructors
of the layer objects (`Preferences/<Layer>.cpp`, e.g. `PlacePrefs::PlacePrefs` in
`DataPrefs.cpp`); the list order is the registry constructor (`UserPreferences::UserPreferences`
in `UserPreferences.cpp`). JC4020 `UserPreferences.h` is byte-identical to JC6416. The `UserPref`
template (load/save/toBytes/fromBytes) is identical in all three.

`textInput1`..`textInput50`: type STR, default `""`, index **0** (all), in `textPrefs` only, not
reported by SYNC.

| Idx | Key | Type | Default JC6416 / JC4020 | Default JC6432 |
|---|---|---|---|---|
| 1 | forceWrite | U8 | 0 | 0 |
| 2 | textPlace | STR | `"whaht otwun"` | same |
| 3 | — (gap. READ / WRITE look up by key, and the key `dayIndex` is in neither `simplePrefs` nor `allPrefs`, so both return `INVALID_KEY` (`handlePref`, `case PREF_OP_READ` / `case PREF_OP_WRITE`). SYNC never returns 3, see above) | — | — | — |
| 4 | shita | U8 | 0 | 0 |
| 5 | eda | U8 | 1 | 1 |
| 6 | ofek | U8 | 1 | 1 |
| 7 | nz_lat | BLOB (double) | 32.07 | 32.07 |
| 8 | nz_long | BLOB (double) | 34.78 | 34.78 |
| 9 | nz_height | I16 | 20 | 20 |
| 10 | nz_hul | U8 | 0 | 0 |
| 11 | timezone | I8 | 2 | 2 |
| 12 | brightness | U8 | 200 | 200 |
| 13 | safa | U8 | 1 | 1 |
| 14 | colorUp | U8 | 1 | 1 |
| 15 | colorMiddle | U8 | 6 | 6 |
| 16 | colorBottomT | U8 | 1 | 1 |
| 17 | colorBottomV | U8 | 6 | 6 |
| 18 † | colorYom | U8 | 6 | **1** |
| 19 † | colorParasha | U8 | 6 | **1** |
| 20 | colorIluy | U8 | 6 | 6 |
| 21 | clockFont | U8 | 0 | 0 |
| 22 | dateFormat | U8 | 1 | 1 |
| 23 | clockFormat | U8 | 24 | 24 |
| 24 | timeFormat | U8 | 0 | 0 |
| 25 | shaonKaiz | U8 | 1 | 1 |
| 26 | middleLine | U8 | 1 | 1 |
| 27 | upLine | U8 | 1 | 1 |
| 28 | zriha | U8 | 2 | 2 |
| 29 | shkiaa | U8 | 3 | 3 |
| 30 | rabenuTam | U8 | 20 | 20 |
| 31 | zeetCochavim | U8 | 8 | 8 |
| 32 | zmanTalit | U8 | 4 | 4 |
| 33 | alotHashahar | U8 | 4 | 4 |
| 34 | minhaG | U8 | 3 | 3 |
| 35 | sozksMga | U8 | 1 | 1 |
| 36 | soztMga | U8 | 1 | 1 |
| 37 | plagMinha | U8 | 1 | 1 |
| 38 | zeetShabat | U8 | 16 | 16 |
| 39 | zeetZom | U8 | 14 | 14 |
| 40 | nerot | U8 | 22 | 22 |
| 41 | homa | U8 | 0 | 0 |
| 42 | hul | U8 | 0 | 0 |
| 43 | leil | U8 | 1 | 1 |
| 44 | netzClk | U8 | 0 | 0 |
| 45 | shanim | U8 | 0 | 0 |
| 46 | interval | U16 | 30 | 30 |
| 47 | intervalIluy | U16 | 30 | 30 |
| 48 | intervalTime | U16 | 30 | 30 |
| 49 | intervalInfo | U16 | 30 | 30 |
| 50 | dafHayomi | U8 | 1 | 1 |
| 51 | yerushalmi | U8 | 0 | 0 |
| 52 | yom | U8 | 0 | 0 |
| 53 | parasha | U8 | 0 | 0 |
| 54 | haftara | U8 | 1 | 1 |
| 55 | ivri | U8 | 0 | 0 |
| 56 | loazi | U8 | 0 | 0 |
| 57 | tfila | U8 | 1 | 1 |
| 58 | moadim | U8 | 1 | 1 |
| 59 | showIsraeli | U8 | 1 | 1 |
| 60 | TAZ | U8 | 1 | 1 |
| 61 | SOZT | U8 | 1 | 1 |
| 62 | SOZKS | U8 | 1 | 1 |
| 63 | HMM | U8 | 1 | 1 |
| 64 | PSZ | U8 | 1 | 1 |
| 65 | HAZ | U8 | 1 | 1 |
| 66 | ZSZ1 | U8 | 1 | 1 |
| 67 | ZSZ | U8 | 1 | 1 |
| 68 | ZYS | U8 | 3 | 3 |
| 69 | ZPS | U8 | 0 | 0 |
| 70 | IPO | U8 | 2 | 2 |
| 71 | IORT | U8 | 2 | 2 |
| 72 | colorIvri | U8 | 6 | 6 |
| 73 | colorLoazi | U8 | 1 | 1 |
| 74 | colorParashaV | U8 | 6 | 6 |
| 75 | screen | U8 | 2 | 2 |
| 76 | bt | U8 | 1 | 1 |

- Same keys, types and indexes in all three models: 75 `simplePrefs` entries, highest index 76.
  Snapshot of the table as the code builds it: `docs/contracts/prefs/<model>.txt`
  (`tools/prefs_contract.py`).
- Defaults that differ per model: `colorYom` and `colorParasha` (†).
- `screen` has a different meaning per model: JC6416 / JC4020 1–4 = layout variants (no consumer on
  these models); JC6432 1 = Big, 2 = Small (comment after the `screen` initializer).
- `colorIvri`, `colorLoazi`, `colorParashaV` are read only on JC6432; on JC6416 / JC4020 they are in
  the table, with no consumer.
- Per the app source (`lib/config/preference_constants.dart`, as reported by the user) the app's
  map ends at 71 (`IORT`), and `getDefinitionByIndex` returns null for an unknown index, so 72–76
  are not mapped by the app. Any future app mapping of 72+ must allow for the per-model meaning of
  `screen`.

† App default differs from the firmware default on some models (refactor-log F9; app values as
reported by the user):

| Idx | Key | App default | JC6416 / JC4020 | JC6432 | Mismatch on |
|---|---|---|---|---|---|
| 18 | colorYom | 6 | 6 | 1 | JC6432 |
| 19 | colorParasha | 6 | 6 | 1 | JC6432 |

**Stored values versus defaults.** SYNC compares the stored value with the current firmware
default. A JC6416 / JC4020 clock that holds `leil` 0 or `screen` 1 in NVS (values that a menu reset
on older firmware saved as defaults) reports 43 or 75. The index is not stored in NVS. A key never
written reads as its default; a menu reset (`resetSimplePrefs`) saves every `simplePrefs` entry.
That 72–76 have no effect on the app is to be confirmed on a clock (2a.3.5).

## 9. Older firmware in the field

Clocks in the field may still run older firmware. This section lists where it behaves differently
from §1–§8. The "App" column gives only what is known about the app from its source, as reported
by the owner.

| Firmware | Clock behavior | App |
|---|---|---|
| below 7.05 | `shita` 0–7 only (`numOfShitot` 8) and zeet methods 0–27, with no range check: a write of `shita` 8–9 or of zeet 28 is stored, and `shitot[]` / `zeetMethod[]` are read past their end (for `shita`: in `DayInfo::initDay` at every boot) | No handling in the app. A rollback to such firmware is not blocked in the app (refactor-log F11) |
| below 7.05 | A write of `ofek` ≠ 0 runs `SetCustomState::printUpdateSucc`, which sets `ofek` to 0: a city chosen from the app is not kept | City selection is blocked in the app for firmware below `MIN_SW_VERSION_CITY_LIST` (7.05), through `ManufacturerData.supportsCityList`, in `settings_location_tab` |
| below 7.07 | No range check for `shita` and `ofek`: any value is stored | No handling in the app |
| below 7.07 | A write of `shita` is only saved; `Shita::setDefault` does not run | The app writes `shita`, waits 500 ms and reads back the 12 method preferences and `nerot`; on these clocks it reads back the previous values. No handling in the app |
| below 7.08 | No range check for `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom`: any value is stored | No handling in the app |
| below 7.08 | Key `dayIndex` at index 3 (U8, default 1). READ / WRITE accept it; JC6416 writes it every day, so SYNC almost always returns 3 | The app defines `dayIndex` at index 3 and does not use it beyond the definition |
| below 7.09, JC6416 / JC4020 | Table ends at 73: `screen` = 72 (default 1), `bt` = 73; `colorIvri`, `colorLoazi`, `colorParashaV` → `INVALID_KEY`. `leil` default 0 | The app's map ends at 71 and `getDefinitionByIndex` returns null for 72–73, so they are ignored. App default of `leil` is 1: a clock at its `leil` default 0 is not reported by SYNC, and the app shows 1 |
