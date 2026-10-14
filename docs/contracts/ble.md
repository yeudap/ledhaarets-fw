# BLE contract (Core service)

Snapshot of the BLE protocol as implemented at commit `e9c7639`. OTA service is documented
in [ota.md](ota.md), the advertisement in [advertisement.md](advertisement.md).

Line numbers refer to `JC6416/src/...` unless stated otherwise. Where a file is identical in
all three models (verified by md5), the same line numbers apply to JC4020 and JC6432.

## 0. Sources and per-model identity

| File | JC6416 | JC4020 | JC6432 |
|---|---|---|---|
| `BLE/CoreProtocol.h` | f2bfe8aa | same | same |
| `BLE/CoreService.cpp` / `.h` | 6ca084cf / 67202d74 | same | same |
| `BLE/Authmanager.cpp` / `.h` | b534df65 / b07d9c83 | same | same |
| `BLE/Bluetooth.cpp` | 9f1f249e | same | same |
| `BLE/Commands.h` | a8dccfc1 | same | same |
| `BLE/TextCommandHandler.h` | 6ceaa330 | same | same |
| `BLE/TextCommandHandler.cpp` | 7b672581 | **a34f9a84 (differs)** | 7b672581 |
| `Preferences/UserPreferences.h` | 1674adef | 1674adef | **b6fd22d0 (differs)** |
| `States/States.h` (`numOfNameSets`) | 50 (`:29`) | **0 (`:28`)** | 50 (`:28`) |

**Model differences in the BLE contract** (details in §6.3 and §8):
1. **JC6432:** preference indexes 72–76 differ (§8). See refactor-log F2.
2. **JC4020:** TEXT domain has 0 slots and write/display are disabled (§6.3). See refactor-log F3.

Everything else in this document is identical in all three models.

## 1. GATT layout

| Item | Value | Source |
|---|---|---|
| Core service UUID | `7b79e39b-488a-4f76-a60f-bd3a29da174f` | `BLE/CoreService.h:8` |
| CMD characteristic UUID | `326109ee-bcea-4809-88b1-5e0da1289a8c`, property `WRITE` | `CoreService.h:9`, `CoreService.cpp:900-903` |
| EVT characteristic UUID | `fbe9b56e-270b-4765-a3ec-b917bb0ad0e1`, property `NOTIFY` | `CoreService.h:10`, `CoreService.cpp:906-908` |
| OTA service UUID (details in ota.md) | `85564e0a-542d-4967-9d09-892b864a1b46` | `BLE/FirmwareService.h:12` |
| GAP init name | `"LedHaarets"` | `BLE/Bluetooth.cpp:63` |
| Preferred MTU | 517 (firmware preference only, see note) | `Bluetooth.cpp:64` |
| Device name (GAP + scan response) | NVS `sys`/`dev_name`, default `"LedHaarets"`, max 24 bytes | `CoreService.cpp:712-741`, `CoreProtocol.h:287-290` |

**MTU note.** 517 is only what the firmware offers. The negotiated MTU is the minimum of both
sides: the app requests **150** (`lib/services/ble_service/ble_connection.dart:64`, app source as
reported by the user). On Android the effective ATT payload is therefore ≤ 147 bytes
(MTU − 3). Any frame size assumption must hold at 147 bytes. This is directly relevant to the
OTA chunk size (`CHUNK_SIZE_DEFAULT` / `SAFE_CHUNK_SIZE` = 244), see [ota.md](ota.md).

Connection callbacks (`Bluetooth.cpp:17-47`): on connect and on disconnect the auth session is
reset (`authManager.resetSession()`); advertising restarts on disconnect.

## 2. Frame format

CMD (app → clock, written to CMD char) — `CoreProtocol.h:8-11`:

```
[0] DOMAIN  [1] OPCODE  [2..N] PAYLOAD
```

EVT (clock → app, Notify on EVT char) — `CoreProtocol.h:13-17`, built in `coreSendEvt` `CoreService.cpp:33-54`:

```
[0] DOMAIN (echo)  [1] OPCODE (echo)  [2] STATUS  [3..N] DATA
```

- `CORE_HEADER_SIZE` = 2, `CORE_EVT_HEADER_SIZE` = 3 (`CoreProtocol.h:32-33`).
- On error: `STATUS = 0x01`, `DATA[0]` = error code (`sendErr`, `CoreService.cpp:57-60`).
- CMD shorter than 2 bytes is dropped silently, no EVT (`CoreService.cpp:851-855`).
- An EVT is sent only if connected and the EVT char exists (`CoreService.cpp:36-37`).

### Byte order

| Field | Order | Source |
|---|---|---|
| PIN in `AUTH_OP_PIN` | **little-endian** u16 | `CoreService.cpp:129` |
| PIN via `PREF_OP_WRITE "pin"` | **little-endian** u16 | `CoreService.cpp:628` |
| Preference values I16/U16 (and I32/U32 in code, unused) | **little-endian** | `UserPreferences.h:231-258, 295-324` |
| Preference BLOB (double) | raw 8 bytes, IEEE 754, little-endian (ESP32 native) | `UserPreferences.h:259-267, 325-331` |
| **Year in TIME data** | **big-endian** (MSB first) | `CoreProtocol.h:182-183`, `CoreService.cpp:475, 510-511` |

## 3. Status and error codes

STATUS (`CoreProtocol.h:38-39`): `0x00` OK, `0x01` ERROR.

| Code | Name | Meaning | Source |
|---|---|---|---|
| 0x01 | `CORE_ERR_UNKNOWN_DOMAIN` | unknown domain | `CoreProtocol.h:44` |
| 0x02 | `CORE_ERR_UNKNOWN_OPCODE` | unknown opcode in domain | `:45` |
| 0x03 | `CORE_ERR_INVALID_PAYLOAD` | payload too short / malformed | `:46` |
| 0x04 | `CORE_ERR_INVALID_INDEX` | index out of range (TEXT) | `:47` |
| 0x05 | `CORE_ERR_INVALID_VALUE` | invalid value | `:48` |
| 0x06 | `CORE_ERR_EMPTY` | empty record | `:49` |
| 0x07 | `CORE_ERR_WRITE_FAILED` | storage write failed | `:50` |
| 0x08 | `CORE_ERR_TYPE_MISMATCH` | pref type ≠ key type | `:51` |
| 0x09 | `CORE_ERR_INVALID_KEY` | pref key not found | `:52` |
| 0x0A | `CORE_ERR_INVALID_TIME` | invalid date/time | `:53` |
| 0x0B | `CORE_ERR_AUTH_REQUIRED` | HELLO: unknown clientId | `:54` |
| 0x0C | `CORE_ERR_AUTH_FAILED` | wrong PIN (after 1 s) | `:55` |
| 0x0D | `CORE_ERR_NOT_AUTHENTICATED` | non-AUTH command before auth | `:56` |
| 0xFF | `CORE_ERR_INTERNAL` | internal | `:57` |

SYS-specific: `SYS_ERR_NAME_TOO_LONG` = 0x01 (`CoreProtocol.h:284`) is defined but **not sent**;
an over-long name returns `CORE_ERR_INVALID_VALUE` (`CoreService.cpp:795-798`). See refactor-log F7.

## 4. Domains

| Code | Domain | Handler | Source |
|---|---|---|---|
| 0x01 | NAV | `handleNav` | `CoreProtocol.h:62`, `CoreService.cpp:189` |
| 0x02 | MENU | `handleMenu` | `:63`, `:224` |
| 0x03 | TEXT | `handleText` | `:64`, `:259` |
| 0x04 | TIME | `handleTime` | `:65`, `:451` |
| 0x05 | PREF | `handlePref` | `:66`, `:532` |
| 0x06 | AUTH | `handleAuth` | `:67`, `:76` |
| 0x07 | SYS | `handleSys` | `:277`, `:767` |

Dispatch: `CoreCallbacks::onWrite`, `CoreService.cpp:844-889`. Unknown domain →
`UNKNOWN_DOMAIN`, echoing the received domain/opcode (`:884-887`).

## 5. Authentication

### 5.1 Gate

Single gate in `onWrite` (`CoreService.cpp:867-873`): any domain other than AUTH (0x06) while
the session is not authenticated → `ERROR + NOT_AUTHENTICATED`, echoing the received
domain/opcode. Session flag is per-connection and reset on connect/disconnect
(`Bluetooth.cpp:23, 37`).

**Scope:** the gate covers the Core service only. The OTA service (`FirmwareService.cpp`,
`OtaProtocol.cpp`) does not reference `authManager`. See refactor-log F5.

### 5.2 Opcodes (`CoreProtocol.h:231-246`, handler `CoreService.cpp:76-183`)

| Op | Name | CMD | EVT | Behavior |
|---|---|---|---|---|
| 0x01 | `AUTH_OP_HELLO` | `[06][01][clientId 16B]` (≥18 B) | `[06][01][STATUS]` | Trusted clientId → OK, `grantSession`, info message on screen. Otherwise `ERROR + AUTH_REQUIRED`. Short → `INVALID_PAYLOAD`. (`:85-115`) |
| 0x02 | `AUTH_OP_PIN` | `[06][02][pin u16 LE][clientId 16B]` (≥20 B) | `[06][02][STATUS]` | Correct → OK, `grantSession` (no NVS write). Wrong → `delay(1000)` then `ERROR + AUTH_FAILED`. (`:121-153`) |
| 0x03 | `AUTH_OP_REGISTER` | `[06][03]` | `[06][03][STATUS]` | Requires authenticated session, else `NOT_AUTHENTICATED`. Stores session clientId as trusted (FIFO). Idempotent. (`:159-177`) |
| 0x04 | — | reserved for future UNREGISTER, not implemented (`CoreProtocol.h:246`) → `UNKNOWN_OPCODE` | | |

### 5.3 PIN and trusted clients (`Authmanager.cpp`, `Authmanager.h`, `CoreProtocol.h:249-260`)

| Item | Value | Source |
|---|---|---|
| clientId size | 16 bytes (raw UUID v4) | `CoreProtocol.h:219, 249` |
| Trusted slots | 3, FIFO eviction (oldest = slot 0 dropped) | `CoreProtocol.h:254`, `Authmanager.cpp:102-120` |
| All-zero clientId | never trusted, never stored | `Authmanager.cpp:70-71, 104-105` |
| Default PIN / after factory reset | 1111 | `CoreProtocol.h:255`, `Authmanager.cpp:22, 141` |
| PIN range | 0–9999 (> 9999 → `INVALID_VALUE`) | `CoreProtocol.h:256`, `CoreService.cpp:630-634` |
| Wrong PIN delay | 1000 ms, blocking in NimBLE host task | `CoreProtocol.h:257`, `CoreService.cpp:149` |
| PIN change | `PREF_OP_WRITE`, key `"pin"`, type must be U16 (0x03), value ≥ 2 B LE; routed to `authManager.setPin`; requires authenticated session | `CoreService.cpp:615-639` |
| PIN readable? | No: `"pin"` is not in `simplePrefs`, so not readable via `PREF_OP_READ` and not in `PREF_OP_SYNC` | `CoreService.cpp:559-560`, `CoreProtocol.h:225-227` |
| NVS writes | PIN only on change; slots only when a *new* client registers | `Authmanager.cpp:127-131, 107-109` |
| Factory reset | PIN = 1111, all slots cleared, session reset; triggered from the menu `SetResetPassState::okAction` | `Authmanager.cpp:139-149`, `States/ConfigState.cpp:115-124` (same in all three) |

NVS keys (`sec` namespace) are listed in [nvs.md](nvs.md).

Note: the comment in `Authmanager.h:23-24` / `Authmanager.cpp:137` describes the factory reset
trigger as "BACK+OK for 10 seconds"; the caller in code is the reset menu state. See refactor-log F8.

## 6. Domain details

### 6.1 NAV (0x01) — `CoreProtocol.h:74-82`, `CoreService.cpp:189-218`

| Op | CMD | EVT |
|---|---|---|
| 0x01 `NAV_OP_PRESS` | `[01][01][key]` (≥3 B) | `[01][01][STATUS]` |

Key codes → internal `ButtonCommand` (`Commands.h:22-28`):

| Key | Wire | `ButtonCommand` |
|---|---|---|
| UP | 0x01 | `UP` = 2 |
| DOWN | 0x02 | `DOWN` = 3 |
| MENU | 0x03 | `MENU` = 1 |
| OK | 0x04 | `OK` = 4 |

Other key → `INVALID_VALUE`. Short packet → `INVALID_PAYLOAD` with opcode `NAV_OP_PRESS`
(not echoed). Wire codes and internal enum values differ; mapping is explicit (`:206-215`).

### 6.2 MENU (0x02) — `CoreProtocol.h:89-137`, `CoreService.cpp:224-254`

| Op | CMD | EVT |
|---|---|---|
| 0x01 `MENU_OP_GOTO` | `[02][01][menu_code]` (≥3 B) | `[02][01][STATUS]` |

`menu_code` is cast directly to `MenuCommand` (`Commands.h:34-79`); values are identical to
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
accepted with OK (`CoreService.cpp:242-253`), despite the comment at `:244`. See refactor-log F6.

### 6.3 TEXT (0x03) — `CoreProtocol.h:144-166`, `CoreService.cpp:259-446`, `TextCommandHandler.cpp`

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

Internal text error → CORE error mapping (`CoreService.cpp:435-446`; internal codes in
`TextCommandHandler.h:19-27`):

| Internal | → CORE |
|---|---|
| `ERR_INVALID_INDEX` 0x01 | `CORE_ERR_INVALID_INDEX` 0x04 |
| `ERR_INVALID_FORMAT` 0x03 | `CORE_ERR_INVALID_VALUE` 0x05 |
| `ERR_EMPTY_TEXT` 0x04 | `CORE_ERR_EMPTY` 0x06 |
| `ERR_WRITE_FAILED` 0x07 | `CORE_ERR_WRITE_FAILED` 0x07 |
| anything else (`ERR_MEMORY`, `ERR_UNSUPPORTED`, `ERR_READ_FAILED`, `ERR_DELETE_FAILED`, …) | `CORE_ERR_INTERNAL` 0xFF |

`TextCommandHandler.h:7-16` also defines legacy command codes (`READ_TEXT` 0xF8, `WRITE_TEXT`
0xF7, `DELETE_TEXT` 0xFD, `DISPLAY_TEXT` 0xFC, `SWAP_TEXTS` 0xFB, `GET_TEXT_COUNT` 0xF9,
`SUCCESS_CODE` 0x00, `ERROR_CODE` 0xFF). They are not used on the wire by the Core service.

> **⚠ Model difference — JC4020**
> - `numOfNameSets = 0` (`JC4020/src/States/States.h:28`): every index is invalid, so READ, WRITE,
>   DELETE, DISPLAY, SWAP always return `ERROR + INVALID_INDEX (0x04)`; COUNT returns OK with `0`.
> - Additionally `handleWriteText` (`JC4020/.../TextCommandHandler.cpp:63`) and
>   `handleDisplayText` (`:138`) have their bodies commented out and return
>   `ERR_INVALID_INDEX` unconditionally.
> - JC6416 / JC6432: 50 slots, full behavior.

### 6.4 TIME (0x04) — `CoreProtocol.h:173-189`, `CoreService.cpp:451-527`

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

### 6.5 PREF (0x05) — `CoreProtocol.h:196-214`, `CoreService.cpp:532-707`

| Op | CMD | EVT data on OK | Search scope |
|---|---|---|---|
| 0x01 `PREF_OP_READ` | `[05][01][key\0]` (≥4 B) | `[type][value bytes]` | `simplePrefs` only (`:561`) |
| 0x02 `PREF_OP_WRITE` | `[05][02][type][key\0][value bytes]` (≥5 B) | — | `allPrefs` (= `textPrefs` + `simplePrefs`, `:642`) + special key `"pin"` |
| 0x03 `PREF_OP_SYNC` | `[05][03]` | `[count][index…]` for every `simplePrefs` entry whose value ≠ default | `simplePrefs` (`:685`) |

WRITE errors: no `\0` after key → `INVALID_KEY`; type ≠ declared type → `TYPE_MISMATCH`;
`fromBytes` failure (too short) → `INVALID_VALUE`; unknown key → `INVALID_KEY`. On success the
value is saved to NVS immediately. Writing the key with index 6 (`ofek`) also shows an "update
succeeded" screen (`:660-668`).

Note: textInput keys are writable through `PREF_OP_WRITE` (they are in `allPrefs`) but are not
readable through `PREF_OP_READ` and all share index 0.

#### Type codes (`CoreProtocol.h:209-214`)

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
only for `UserPref<double_t>` (`UserPreferences.h:410-411, 566, 569`). Wire encoding is a raw
`memcpy` of the 8-byte double (`toBytes` `:259-267`, `fromBytes` `:325-331`, requires ≥ 8 B),
little-endian on ESP32. In NVS it is stored with `putDouble`, which the framework implements as
`putBytes` (8 B blob). This matches the app (`lib/config/core_protocol.dart:147`, 8-byte double,
as reported by the user). Naming note only, not a Finding.

**STR wire format:**

- **Encoding — not UTF-8.** The app transliterates Hebrew to Latin characters by keyboard
  layout (א→`t`, ש→`a`, …; `lib/utils/text_processor.dart`) and sends one byte per character
  (`codeUnits`). The same encoding is used for TEXT domain strings (§6.3). App-side maximum:
  70 characters. (App source as reported by the user.) The firmware stores and returns the bytes
  as-is; the display maps them to Hebrew glyphs. The default `textPlace` value `"whaht otwun"`
  is in this encoding. The comment "UTF-8" at `CoreProtocol.h:213` does not describe the actual
  encoding.
- **Only `SYS_OP_SET_NAME` is UTF-8** (app: `utf8.encode`; firmware validates length in bytes,
  §6.6).
- **Terminator:** `PREF_OP_READ` returns the string bytes **plus a trailing `0x00`**
  (`toBytes`, `UserPreferences.h:268-275`), contrary to the comment at `CoreProtocol.h:213`.
  `PREF_OP_WRITE` parses the value as a C string (`:334`), i.e. up to the first `\0`. The app
  truncates at the first `\0` on read and appends `\0` on write
  (`lib/services/user_preferences_service.dart`). **The format with `\0` is what both sides
  expect.** See refactor-log F4.

### 6.6 SYS (0x07) — `CoreProtocol.h:277-290`, `CoreService.cpp:767-837`

| Op | CMD | EVT | Behavior |
|---|---|---|---|
| 0x01 `SYS_OP_RESTART` | `[07][01]` | `[07][01][00]` | OK sent first; `esp_restart()` after 500 ms timer (`:751-762, 777-785`) |
| 0x02 `SYS_OP_SET_NAME` | `[07][02][UTF-8 name]` | `[07][02][STATUS]` | > 24 bytes → `INVALID_VALUE`. Empty → delete NVS key, use default `"LedHaarets"`. Applies to GAP name + scan response immediately (`:791-818`) |
| 0x03 `SYS_OP_GET_MAC` | `[07][03]` | `[07][03][00][MAC 6B]` | `esp_efuse_mac_get_default` (`:825-831`) |

## 7. Version gate

`MIN_FW_MAJOR_VERSION` = 7 (`CoreProtocol.h:27`): documented as "a clock with major < 7 must
be OTA-updated before use". The firmware itself does not reference it; it is a constant shared
with the app. Protocol version per comment: 1.1 (`CoreProtocol.h:19`).

## 8. Preference indexes

Indexes are returned by `PREF_OP_SYNC`, and the app maps them to keys. Keys and types are
also the NVS keys in namespace `clk` (see [nvs.md](nvs.md)).

**Firmware defaults are part of the contract.** `PREF_OP_SYNC` returns only the indexes whose
value differs from the *firmware* default (`CoreService.cpp:685-689`, `_hasChangedFromDefault`
`UserPreferences.h:214-217`). For every index not returned, the app assumes its *own* default.
Changing a firmware default therefore changes what the app displays for clocks still at
default, and any mismatch between app and firmware defaults is visible to the user.
Known mismatches: refactor-log F9 (marked † in the table below).

Declarations: `UserPreferences.h` constructor, JC6416/JC4020 lines 487–846, JC6432 lines 490–856.
JC4020 `UserPreferences.h` is byte-identical to JC6416. The `UserPref` template (lines 1–345,
load/save/toBytes/fromBytes) is identical in all three.

`textInput1`..`textInput50`: type STR, default `""`, index **0** (all), in `textPrefs` only, not
reported by SYNC (JC6416 `:488-537`).

| Idx | Key | Type | Default JC6416 / JC4020 | Default JC6432 | Line 6416/4020 | Line 6432 |
|---|---|---|---|---|---|---|
| 1 | forceWrite | U8 | 0 | 0 | 487 | 490 |
| 2 | textPlace | STR | `"whaht otwun"` | same | 539 | 542 |
| 3 | dayIndex | U8 | 1 | 1 | 542 | 545 |
| 4 | shita | U8 | 0 | 0 | 545 | 548 |
| 5 | eda | U8 | 1 | 1 | 555 | 558 |
| 6 | ofek | U8 | 1 | 1 | 563 | 566 |
| 7 | nz_lat | BLOB (double) | 32.07 | 32.07 | 566 | 569 |
| 8 | nz_long | BLOB (double) | 34.78 | 34.78 | 569 | 572 |
| 9 | nz_height | I16 | 20 | 20 | 572 | 575 |
| 10 | nz_hul | U8 | 0 | 0 | 575 | 578 |
| 11 | timezone | I8 | 2 | 2 | 580 | 583 |
| 12 | brightness | U8 | 200 | 200 | 583 | 586 |
| 13 | safa | U8 | 1 | 1 | 586 | 589 |
| 14 | colorUp | U8 | 1 | 1 | 590 | 593 |
| 15 | colorMiddle | U8 | 6 | 6 | 593 | 596 |
| 16 | colorBottomT | U8 | 1 | 1 | 596 | 599 |
| 17 | colorBottomV | U8 | 6 | 6 | 599 | 602 |
| 18 † | colorYom | U8 | 6 | **1** | 602 | 605 |
| 19 † | colorParasha | U8 | 6 | **1** | 605 | 608 |
| 20 | colorIluy | U8 | 6 | 6 | 608 | 611 |
| 21 | clockFont | U8 | 0 | 0 | 611 | 614 |
| 22 | dateFormat | U8 | 1 | 1 | 614 | 617 |
| 23 | clockFormat | U8 | 24 | 24 | 624 | 627 |
| 24 | timeFormat | U8 | 0 | 0 | 628 | 631 |
| 25 | shaonKaiz | U8 | 1 | 1 | 631 | 634 |
| 26 | middleLine | U8 | 1 | 1 | 636 | 639 |
| 27 | upLine | U8 | 1 | 1 | 640 | 643 |
| 28 | zriha | U8 | 2 | 2 | 645 | 648 |
| 29 | shkiaa | U8 | 3 | 3 | 650 | 653 |
| 30 | rabenuTam | U8 | 20 | 20 | 655 | 658 |
| 31 | zeetCochavim | U8 | 8 | 8 | 658 | 661 |
| 32 | zmanTalit | U8 | 4 | 4 | 661 | 664 |
| 33 | alotHashahar | U8 | 4 | 4 | 664 | 667 |
| 34 | minhaG | U8 | 3 | 3 | 667 | 670 |
| 35 | sozksMga | U8 | 1 | 1 | 670 | 673 |
| 36 | soztMga | U8 | 1 | 1 | 673 | 676 |
| 37 | plagMinha | U8 | 1 | 1 | 676 | 679 |
| 38 | zeetShabat | U8 | 16 | 16 | 679 | 682 |
| 39 | zeetZom | U8 | 14 | 14 | 682 | 685 |
| 40 | nerot | U8 | 22 | 22 | 685 | 688 |
| 41 | homa | U8 | 0 | 0 | 689 | 692 |
| 42 | hul | U8 | 0 | 0 | 693 | 696 |
| 43 † | leil | U8 | 0 | **1** | 697 | 700 |
| 44 | netzClk | U8 | 0 | 0 | 702 | 705 |
| 45 | shanim | U8 | 0 | 0 | 706 | 709 |
| 46 | interval | U16 | 30 | 30 | 711 | 714 |
| 47 | intervalIluy | U16 | 30 | 30 | 716 | 719 |
| 48 | intervalTime | U16 | 30 | 30 | 721 | 724 |
| 49 | intervalInfo | U16 | 30 | 30 | 726 | 729 |
| 50 | dafHayomi | U8 | 1 | 1 | 731 | 734 |
| 51 | yerushalmi | U8 | 0 | 0 | 734 | 737 |
| 52 | yom | U8 | 0 | 0 | 737 | 740 |
| 53 | parasha | U8 | 0 | 0 | 740 | 743 |
| 54 | haftara | U8 | 1 | 1 | 743 | 746 |
| 55 | ivri | U8 | 0 | 0 | 746 | 749 |
| 56 | loazi | U8 | 0 | 0 | 749 | 752 |
| 57 | tfila | U8 | 1 | 1 | 752 | 755 |
| 58 | moadim | U8 | 1 | 1 | 755 | 758 |
| 59 | showIsraeli | U8 | 1 | 1 | 758 | 761 |
| 60 | TAZ | U8 | 1 | 1 | 761 | 764 |
| 61 | SOZT | U8 | 1 | 1 | 767 | 770 |
| 62 | SOZKS | U8 | 1 | 1 | 773 | 776 |
| 63 | HMM | U8 | 1 | 1 | 779 | 782 |
| 64 | PSZ | U8 | 1 | 1 | 785 | 788 |
| 65 | HAZ | U8 | 1 | 1 | 791 | 794 |
| 66 | ZSZ1 | U8 | 1 | 1 | 797 | 800 |
| 67 | ZSZ | U8 | 1 | 1 | 804 | 807 |
| 68 | ZYS | U8 | 3 | 3 | 811 | 814 |
| 69 | ZPS | U8 | 0 | 0 | 818 | 821 |
| 70 | IPO | U8 | 2 | 2 | 825 | 828 |
| 71 | IORT | U8 | 2 | 2 | 832 | 835 |

† App default differs from the firmware default on some models (refactor-log F9; app values as
reported by the user):

| Idx | Key | App default | JC6416 / JC4020 | JC6432 | Mismatch on |
|---|---|---|---|---|---|
| 18 | colorYom | 6 | 6 | 1 | JC6432 |
| 19 | colorParasha | 6 | 6 | 1 | JC6432 |
| 43 | leil | 1 | 0 | 1 | JC6416, JC4020 |

### ⚠ Indexes 72–76 — BLE contract differs between models

| Idx | JC6416 / JC4020 | JC6432 |
|---|---|---|
| 72 | **`screen`** U8, default 1 (`:839`) | **`colorIvri`** U8, default 6 (`:842`) |
| 73 | **`bt`** U8, default 1 (`:846`) | **`colorLoazi`** U8, default 1 (`:845`) |
| 74 | — | **`colorParashaV`** U8, default 6 (`:848`) |
| 75 | — | **`screen`** U8, default **2** (`:851`) |
| 76 | — | **`bt`** U8, default 1 (`:856`) |

- The same index refers to a different key on JC6432. This is a **difference in the BLE
  contract between models**, not only in the NVS content.
- `screen` also has a different meaning: JC6416/JC4020 1–4 = layout variants
  (`:840-844`); JC6432 1 = Big, 2 = Small (`:852-854`).
- Per the app source (`lib/config/preference_constants.dart`, as reported by the user) the app's
  map ends at 71 (`IORT`), so 72–76 are not currently mapped by the app and there is no collision
  in practice. Any future app mapping of 72+ must be per-model.
- Simple-pref counts: JC6416/JC4020 73, JC6432 76 (`simplePrefs.push_back`).

Refactor-log: F2.
