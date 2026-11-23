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
- **Production port 7.08** (commit `d01dd0f`) — mixed method: full copy of production `v7.08`
  into JC4020 and JC6432, patch `v7.07..v7.08` into JC6416 (on top of 2a.1–2a.2). Details in
  `docs/baseline.md`, Sync 4. `Preferences/`: `dayIndex` removed (index 3 is a gap); all other
  keys, indexes, types and defaults unchanged. Closed F8, F11 and F16; F13 stays open (fix in
  `ledhaarets-build`); recorded F19. The adaptation of the new JC6416 code to 2a is step 2a.2.7.
  - **Build at `d01dd0f`** (clean `pio run`, env `esp32dev`; build only, no device operations):

    | Model | `firmware.bin` | RAM | Flash | Compiler warnings |
    |---|---|---|---|---|
    | JC6416 | 2,060,800 | 61,100 | 2,054,229 | 0 |
    | JC4020 | 2,055,536 | 61,132 | 2,048,961 | 0 |
    | JC6432 | 2,072,400 | 61,924 | 2,065,825 | 0 |

    JC6416 figures are from the 2a.2.7 reference build (same tree, `SOURCE_DATE_EPOCH` set);
    JC4020 and JC6432 were built without it.
- **Production port 7.09** (Sync 5; commits `7066e4e` JC6416, `da5a349` JC4020, `1cb790a`
  JC6432) — patch `v7.08..v7.09` in all three models, on top of 2a.1–2a.3.2;
  `UserPreferences.h` copied from production as is (one-time exception to the editing rule,
  approved by the owner). Details in `docs/baseline.md`, Sync 5.
  - **Before the sync:** `672aa78` — the host stub of `tools/prefs_contract.py` had no
    `String(const char*, length)`, so the registry with F22 did not compile on the PC. Added the
    Arduino-ESP32 2.0.17 signature; on the tree of `32f2b10` the output of all three models is
    identical to the baselines.
  - **Closed in 7.09:** F17, F19, F21, F22. F2 is closed in production too (one table in all
    three models). F9 updated (`leil` is 1 in all models).
  - **`printAll`** is back (production keeps it for debug): documented deviation from 2a.3.1.
  - **Contract change (owner's decision, in production since 7.09):** JC6416 / JC4020 defaults
    `leil` 0 → 1 and `screen` 1 → 2 (aligned with JC6432; `screen` has no consumer on these
    models, so no display change). `PREF_OP_READ` of a key never written returns the new default;
    `PREF_OP_SYNC` now reports 43 for a stored `leil` 0 and 75 for a stored `screen` 1 (e.g. after
    "reset settings" on 7.08, which saves every default), and no longer reports a stored `leil` 1.
    `docs/contracts/prefs/JC6416.txt` and `JC4020.txt` rewritten (only these two defaults);
    JC6432 baseline unchanged.
  - **Out of scope:** F22 also exists in BA6416, BC6416, JC8040 and Laniado (owner's report).
  - **Verification** (reference: clean builds of `672aa78`; `SOURCE_DATE_EPOCH=1763337600` in
    every build; comparison procedure in `docs/baseline.md`, Sync 5):

    | Model | `firmware.bin` | RAM | Flash | Compiler warnings |
    |---|---|---|---|---|
    | JC6416 | 2,061,040 → 2,061,072 (+32) | 61,172 | 2,054,457 → 2,054,501 | 0 |
    | JC4020 | 2,055,760 → 2,055,680 (−80) | 61,204 | 2,049,181 → 2,049,097 | 0 |
    | JC6432 | 2,072,400 → 2,072,448 (+48) | 61,924 | 2,065,825 → 2,065,869 | 0 |

    - Versus production v7.09, all three: `compare_bins.py` "Only metadata differs"; ELF after
      `--strip-debug` byte-identical; `nm -C --size-sort` identical.
    - `prefs_contract.py check`: JC6416 / JC4020 differ from the old baseline only in the `leil`
      and `screen` defaults; JC6432 OK. Against JC6432 only `colorYom` / `colorParasha` differ.
    - `count_deps.py --compare` (tree of `32f2b10`): `RegsScreen -> PresentationPrefs` 12 → 12
      (`leil` −2, `timeFormat` +2) in all three; JC4020 also `MainStateTimeSets -> DB/Dict` 2 → 1
      (`daysOfTheWeek` in a removed F21 print). Preference consumer sums unchanged: 784 / 820 / 847.
    - `UserPreferences.h` `cmp`-equal to production, `w/crlf`. UUIDs, NVS namespaces and keys,
      advertisement prefix unchanged.
    - `diff -r` of the production tree and the repo: only the 2a views and their consumers, and
      helper files that exist only in production.
  - **Docs:** `dependencies.md` regenerated (`gen_deps_doc.py write`). It had not been regenerated
    after 2a.3.2 (`check` failed already at `32f2b10`: three view edges, +12 references);
    `CLAUDE.md` now runs `check` in every step that changes edges.
- **Production port 709A** (Sync 6; commits `2b1617c` JC6416, `c53643a` JC4020, `cdc789c` JC6432)
  — patch `v7.09..709A` in all three models, on top of 2a.1–2a.3.2. `709A` is a temporary
  production tag (not a release; to be replaced by `v7.10` and deleted), so the production commits
  are recorded by hash: JC6416 `8d4db32dc3000fc33d533aa5423c92ff79b4187b`, JC4020
  `225ed5ed5fa4fdae77f0cdf8763cc2ddb49d2e78`, JC6432 `7da8a8aaf6e6ed7f405bfeab928dd4c7bc653699`.
  Details in `docs/baseline.md`, Sync 6.
  - **Before the sync:** `63f997c` — `CLAUDE.md`: the contract-change rule replaces "No change to
    NVS keys …", and approval of a sequential plan replaces the diff before editing (step 1 of
    the saved 2a.3.3 plan, not applied until then).
  - **Synced:** F13, S4, S6, S8; `PROJECT_VER` 7.09 → 7.10.
  - **One-time exception** (owner's decision): `src/States/ConfigState.cpp:30-31` in JC4020 and
    JC6432 (S8: `showStateScreen()` with 4 trailing spaces, `{ ` with 1). The editor tool strips
    trailing whitespace, so the two lines were written by an exact byte replacement script.
  - **Commit fix:** the staged `git rm` of the JC6432 `or_*` files first went into the JC6416
    commit; restored to the index and the commit amended before anything else (local, not
    pushed). Each model commit holds only its own files.
  - **Recorded:** F23.
  - **Verification** (reference: clean builds of `63f997c`, and clean builds of `git archive
    <commit>` of production; `SOURCE_DATE_EPOCH=1763337600` in every build):

    | Model | `firmware.bin` | RAM | Flash | Compiler warnings |
    |---|---|---|---|---|
    | JC6416 | 2,061,072 → 2,061,072 (0) | 61,172 | 2,054,501 | 0 |
    | JC4020 | 2,055,680 → 2,055,680 (0) | 61,204 | 2,049,097 | 0 |
    | JC6432 | 2,072,448 → 2,072,448 (0) | 61,924 | 2,065,869 | 0 |

    - Versus production 709A, all three: `compare_bins.py` "Only metadata differs"; ELF after
      `--strip-debug` byte-identical; `nm -C --size-sort` identical.
    - Files: `platformio.ini`, `Area.cpp`, `Display.cpp`, `Clock.h`, `stamp_app_desc.py` have the
      709A blob in all three. `ConfigState.cpp`, `OfekState.cpp`: the diff to 709A equals the diff
      to v7.09 before the sync (2a lines only). `diff -r` of 709A and the repo: the same files and
      content as before the sync, plus helper files that exist only in production.
    - `prefs_contract.py check`: OK for all three. `count_prefs.py`: identical.
    - `count_deps.py --compare` (tree of `63f997c`): JC6416 and JC4020 `Area -> Display` 29 → 28
      (the definition of `display` moved to `Display.cpp`); JC6432 no change. Preference consumer
      sums unchanged: 784 / 820 / 847.
    - UUIDs, NVS namespaces and keys, advertisement prefix unchanged. `git ls-files --eol`: edited
      files `w/crlf`, as before.
  - **Docs:** `dependencies.md` regenerated (`gen_deps_doc.py write`; by hand: §1 and §4 totals
    7,176 → 7,175, the §3 and §5.1 graphs 29 → 28, §5 "28 references from `Area`", §8 `display`
    defined in `Display.cpp:3`); `check` OK (421 edges, 7,175 references). `notes.md` (`display` in
    `Display.cpp`), `environment.md:95` (`HARDWARE_VERSION` removed, F13).

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
- **2a.2.4** (JC6416) — Logic:
  - `userPreferences.<field>` → `logicPrefs.<field>` for the 27 Logic fields (`netzClk`,
    `interval`, `intervalIluy`, `intervalTime`, `intervalInfo`, `dafHayomi`, `yerushalmi`, `yom`,
    `parasha`, `haftara`, `ivri`, `loazi`, `tfila`, `moadim`, `showIsraeli`, `TAZ`, `SOZT`,
    `SOZKS`, `HMM`, `PSZ`, `HAZ`, `ZSZ1`, `ZSZ`, `ZYS`, `ZPS`, `IPO`, `IORT`): 223 occurrences on
    190 lines in `src/States/SetTochenTimeState.cpp` (51), `src/Screens/RegsScreen.cpp` (48),
    `src/States/SetTochenInfoState.cpp` (46), `src/States/MainStateInfoSets.cpp` (41),
    `src/States/MainStateTimeSets.cpp` (22), `src/Logic.cpp` (8), `src/States/SetInputText.cpp`
    (4), `src/DB/DayInfo.cpp` (2), `src/States/MainState.cpp` (1), plus
    `#include "Preferences/LogicPrefs.h"` in each. All 27 fields have consumers in JC6416. No
    commented-out lines in this layer. No line mixes a Logic field with another layer's field;
    `eda`, `hul`, `homa` in `MainStateInfoSets.cpp` / `MainStateTimeSets.cpp` stay on
    `userPreferences` (Data layer).
  - Edits made with the editor tool only (whole-file `replace_all` only in `SetTochenTimeState.cpp`,
    `SetTochenInfoState.cpp` and `SetInputText.cpp`, where every `userPreferences.` access is a
    Logic field; per-field or per-line elsewhere).
  - **Verification** (reference: clean build of `7983099`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `objcopy --strip-debug`: the two ELFs are byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256)
      and 32 in the trailer (two ranges; one trailer byte happens to match). As expected.
    - `grep`: none of the 27 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `logicPrefs.` 223 times.
    - `git diff --stat`: 9 files, 199 insertions, 190 deletions (190 replaced lines + 9 includes).
      Nothing under `Preferences/` changed; contract strings untouched.
    - `git ls-files --eol`: all 9 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.5** (JC6416) — Data (last layer):
  - `userPreferences.<field>` → `dataPrefs.place.<field>` (`textPlace`, `ofek`, `nz_lat`,
    `nz_long`, `nz_height`, `nz_hul`, `timezone`, `shaonKaiz`, `homa`), `dataPrefs.zmanim.<field>`
    (`shita`, `zriha`, `shkiaa`, `rabenuTam`, `zeetCochavim`, `zmanTalit`, `alotHashahar`,
    `minhaG`, `sozksMga`, `soztMga`, `plagMinha`, `zeetShabat`, `zeetZom`, `nerot`) and
    `dataPrefs.minhagim.<field>` (`eda`, `hul`, `shanim`): 397 occurrences (place 89, zmanim 241,
    minhagim 67) on 344 lines in 15 files: `src/DB/Shita.cpp` (144), `src/States/MinhagState.cpp`
    (48), `src/Screens/RegsScreen.cpp` (42), `src/States/OfekState.cpp` (41), `src/DB/DayInfo.cpp`
    (40), `src/DB/Place.cpp` (19), `src/Logic.cpp` (19), `src/States/EdotState.cpp` (17),
    `src/States/MainStateInfoSets.cpp` (7), `src/DB/Omer.cpp` (6), `src/States/SetClockState.cpp`
    (5), `src/DB/DB.cpp` (3), `src/States/ShitaState.cpp` (3), `src/States/MainStateTimeSets.cpp`
    (2), `src/States/MainState.cpp` (1), plus `#include "Preferences/DataPrefs.h"` in each. All 26
    fields have consumers in JC6416.
  - Commented-out debug lines replaced too, including the label text inside their string
    literals (owner's decision): `src/DB/Place.cpp:127-132` and `src/States/OfekState.cpp:258-259`,
    e.g. `// Serial.print("dataPrefs.place.nz_long: ");`.
  - Edits made with the editor tool only (per-field `replace_all` per file).
  - After this sub-step, the only `userPreferences.` accesses outside `Preferences/` are the
    registry members (`simplePrefs`, `allPrefs` in `src/BLE/CoreService.cpp`; `resetSimplePrefs`,
    `resetTextPrefs` in `src/States/ConfigState.cpp`; `printAllValues`, commented out, in
    `src/main.cpp:63`) and `dayIndex` (`src/States/MainState.cpp:289`, F16), as planned for 2a.2.
  - **Verification** (reference: clean build of `9235817`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,912 bytes before and after (delta 0). RAM 61,124 and Flash
      2,054,341 bytes, unchanged.
    - `nm -C --size-sort`: identical.
    - `objcopy --strip-debug`: the two ELFs are byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256)
      and 33 in the trailer. As expected.
    - `grep`: none of the 26 fields remains as `userPreferences.<field>` outside `Preferences/`,
      comments included; every `dataPrefs.<group>.<field>` uses the group the field belongs to.
    - `git diff --stat`: 15 files, 359 insertions, 344 deletions (344 replaced lines + 15
      includes). Nothing under `Preferences/` changed; contract strings untouched.
    - `git ls-files --eol`: all 15 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.6** — `tools/count_prefs.py` counts access through the layer views:
  - `REF` also matches `systemPrefs.`, `userContent.`, `presentationPrefs.` (with or without
    `layout.`), `displayPrefs.`, `logicPrefs.`, `dataPrefs.place|zmanim|minhagim.`; the captured
    name is still the preference name. A leading `\b` was added.
  - The units that define the views (`SystemPrefs`, `UserContent`, `PresentationPrefs`,
    `DisplayPrefs`, `LogicPrefs`, `DataPrefs`, `PrefsViews`) are skipped: they bind, they do not
    use. Write detection and output format unchanged. Docstring updated.
  - `docs/architecture/preferences.md` §1: the counting rule now includes access through the views.
  - No firmware source changed; no build.
  - **Verification** (output has no line numbers: `<pref>: <unit> r<reads>/w<writes>`, so a
    byte comparison is a comparison of the counts; text and `--json` both checked):
    - New tool vs old tool, on trees without views: identical on JC6416 at `e76aaea` and at
      `dcd2cdb`, and on JC4020 and JC6432 at HEAD.
    - New tool on JC6416 at `73cb81c` vs old tool on JC6416 at `e76aaea` (before the views):
      identical. Same consumers, units, reads and writes for every preference.
  - **End of 2a.2 (JC6416):** `grep "userPreferences\s*\."` outside `Preferences/` finds only:
    - `dayIndex` — `src/States/MainState.cpp:289` (write; F16, removal planned).
    - Registry members:
      - `resetSimplePrefs` — `src/States/ConfigState.cpp:81`, `:143`.
      - `resetTextPrefs` — `src/States/ConfigState.cpp:101`, `:142`.
      - `simplePrefs` — `src/BLE/CoreService.cpp:562`, `:704`.
      - `allPrefs` — `src/BLE/CoreService.cpp:643`.
      - `printAllValues` — `src/main.cpp:63`, commented out.
    - A Hebrew comment that ends with the word: `src/BLE/CoreService.cpp:613`
      ("ולא ל-userPreferences."). Not an access.
    - All other preference accesses go through the layer views.
- **2a.2.7** (JC6416) — 2a.2 rule applied to the code that came with production 7.08 (F11):
  - `src/BLE/CoreService.cpp:655-658`: `&userPreferences.<field>` → `&dataPrefs.zmanim.<field>` for
    `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom` (the `isZeetPref` range check), plus
    `#include "Preferences/DataPrefs.h"` after the last existing include. No other change on the
    lines. The views are references to the same objects (`PrefsViews.cpp:27-36` asserts the
    addresses), so the pointer comparison is unchanged.
  - Edits made with the editor tool only.
  - Before editing, the 7.08 port was checked for new direct accesses (JC6416 at `d01dd0f` vs
    `18e4bf2`, end of 2a.2):
    - `grep "userPreferences\s*\."` outside `Preferences/`: the only additions are the four
      `isZeetPref` lines; the `dayIndex` write is gone. Registry members and the comment unchanged.
    - `tools/count_prefs.py`: two differences only. `dayIndex` is gone, and `CoreService r1/w0`
      is new for the four zeet keys. The tool counts access through the views too, so the
      `CoreService` reads remain after this step (identical output before and after the edit).
  - **Verification** (reference: clean build of `d01dd0f`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6416` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,060,800 bytes before and after (delta 0). RAM 61,100 and Flash
      2,054,229 bytes, unchanged. (7.08 vs 2a.2.5: −112 bytes bin, −24 RAM, −112 Flash.)
    - `nm -C --size-sort`: identical.
    - `objcopy --strip-debug`: the two ELFs are byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256)
      and 33 in the trailer. As expected.
    - `grep`: none of the four fields remains as `userPreferences.<field>` outside `Preferences/`;
      `dataPrefs.zmanim.` 4 times in `CoreService.cpp`.
    - `git diff --stat`: 1 file, 5 insertions, 4 deletions (4 replaced lines + 1 include).
      Nothing under `Preferences/` changed; contract strings untouched.
    - `git ls-files --eol`: `w/crlf`, as before.
  - `CoreService.cpp` was identical in the three models after the 7.08 port; it now differs in
    JC6416 by this step, until 2a is replicated to JC4020 and JC6432.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.8** — `tools/count_deps.py` counts access through the layer views (step (a) before
  regenerating `dependencies.md`):
  - **Cause:** a view global (`inline constexpr DataPrefs dataPrefs{...};`) was not recognized: the
    parser took the brace initializer for a block it does not handle, so `dataPrefs` was unknown
    and the whole chain was dropped. After 2a.2 the consumer edges to `Preferences/UserPreferences`
    shrank (e.g. `DB/Shita` 146 → 2) with no edge in their place.
  - **Rule:** access through a view counts once toward the view unit (e.g. `DB/Shita →
    Preferences/DataPrefs`), with the chain up to the preference as identifier
    (`dataPrefs.zmanim.shita`), so the identifier count is still per preference. It does not
    count toward `Preferences/UserPreferences`, also when a method is called on the preference.
    `userContent.textPrefs` counts as `userContent` (namespace-qualified member types are not
    followed, as for `userPreferences.textPrefs`).
  - **Change:** `VIEW_UNITS` (the six view units); in `handle_block`, a brace-initialized global in
    a view unit is registered as a global of its struct type (not as a const, so the chain is
    followed); in `count_refs`, a chain rooted in a view stops at the first member whose type is
    outside the view units. Output format and options unchanged. Docstring updated.
  - No firmware source changed; no build. The counting rule in `dependencies.md` §1 is updated in
    step (b), together with the regenerated sections.
  - **Verification** (previous version of the tool from `e7f8efb`; scripts in the scratchpad):
    - Only production sync in `c6882d6..HEAD` for JC6416: port 7.08 (`git log --oneline
      c6882d6..HEAD -- JC6416`; the rest are 2a.2 steps). So the only allowed differences are the
      7.08 ones.
    - Trees without views, new vs previous tool, text and `--json`: identical on JC4020 and JC6432
      at `e7f8efb`, and on JC6416 at `e76aaea`.
    - JC6416 at `e7f8efb`, new vs previous tool: 47 edges differ, all of them new edges into a
      view unit (41 from consumers, 6 from `PrefsViews`). Every other edge is identical,
      including the view units' own edges (e.g. `DataPrefs → UserPreferences` 54).
    - JC6416, new tool at `e7f8efb` vs previous tool at `c6882d6` (before 2a.2), consumers only
      (units other than `UserPreferences`, `PrefsViews` and the views), references to
      `UserPreferences` plus the views: 27 consumers; equal for each consumer except
      `BLE/CoreService` 5 → 9 (+4) and `States/MainState` 8 → 7 (−1); 781 → 784 in total. Per
      (consumer, preference), with view identifiers normalized to `userPreferences.<name>`, the
      only differences are `CoreService` `rabenuTam`, `zeetCochavim`, `zeetShabat`, `zeetZom`
      0 → 1 (F11) and `MainState` `dayIndex` 1 → 0 (F16). Both from 7.08, as expected.
    - `PrefsViews` → views: 72 references (`DataPrefs` 26, `LogicPrefs` 27, `DisplayPrefs` 9,
      `PresentationPrefs` 8, `SystemPrefs` 1, `UserContent` 1), one per `static_assert` in
      `PrefsViews.cpp` (72: 71 preference fields + `userContent.textPrefs`). The message strings
      are not counted.
- **2a.2.9** — `docs/architecture/dependencies.md` regenerated on the current JC6416 tree (step (b)):
  - **Base:** `JC6416/src` at `7b60c79` (7.08 + 2a.2; unchanged since), counted with the tool of
    2a.2.8 (`bc7d1b5`). 423 edges, 7,166 references (previous map: 374 / 6,747 at `9c75812`).
  - **Generated blocks** (between `<!-- generated:<name>:begin/end -->` markers, written by a
    scratchpad script with explicit CRLF; one-time approval for this file only, not a change to the
    editing rule): §4 edge table, §9 include list, appendix A.1–A.4. The §4 "mark" column is
    computed from the §2 level table, the two intended edges in `notes.md`, and the two name
    collisions below.
  - **Hand-written parts, kept and updated:** §1 (base; view units; view counting rule from 2a.2.8;
    known tool limitations; spot checks re-run; "updating the document", with the rule moved from
    §11.4 and reworded: regenerate at every production sync and at the end of every sub-stage that
    changes dependencies, no delta section); §2 (views and `PrefsViews` at level 4 with
    `UserPreferences`, owner's decision); §3, §5, §6 (structure kept, numbers from the tool; §5.6
    rewritten with the views and a per-view table); §7 (line references re-checked on the base;
    `ConfigState → Authmanager` = 2); §8 (locations re-checked; view globals, `hardwareModel` and
    `Metadata::HWversion` added; `shitot[]` is `Shita*[10]`); §9 (hand notes under the generated
    list); §10 (re-checked against JC4020 / JC6432 at HEAD; per-model preference table added).
  - **§11 deleted.** §11.1 / §11.2 (new and stronger edges) are covered by the regenerated §4; the
    "version" column is history and stays in git (`e7f8efb`). §11.3 items: §4, §7.1 and §9 parts
    covered by regeneration; the §8 parts (`shitot[10]`, `hardwareModel`, `HWversion`) moved by
    hand into §8. §11.4 moved into §1.
  - **Generator validated** on `9c75812` with the previous tool against the previous document:
    §4 identical in #, from, to, references and mark for all 374 rows; differences only in the
    tie order of the examples (the tool sorts ties alphabetically) and in the identifier count of
    the two edges already noted in §1. A.2 and A.3 identical. A.1 and A.4: the angle-bracket
    include `Prefs → States` is missing (see below); A.4 shows the unit name whenever a topic has
    a single intra target (uniform rule; the previous document did it in some rows only).
  - **Found while regenerating (documented in §1, no code or tool change):**
    - `Preferences/DataPrefs → DB/Place` and `→ DB/Shita` (1 each) are name collisions: the struct
      fields `place` and `zmanim` (`DataPrefs.h:56-57`) match the global `place` and the array
      `zmanim`. Marked "name collision" in §4, excluded from §7.1. A one-line addition to the
      tool's `EXCLUDE` would remove them (separate step, if approved).
    - The tool does not see `#include <...>`. Two such includes are relevant
      (`UserPreferences.cpp:3`, `Bluetooth.cpp:5`); listed by hand in §9.
    - §10, JC4020: `TextCommandHandler → Logic` (2, only `extern Logic bl;`) and `→ States/States.h`
      (3, `numOfNameSets`) exist; the previous map said these edges do not exist. Corrected.
    - §10, JC6432: `MainState` also calls `mainScreen.setScreen` (added).
    - §5.8 referred to "appendix B"; corrected to appendix A, table A.2.
  - **Verification** (scripts in the scratchpad):
    - §4 rows identical to `count_deps.py` output except the mark column (423 rows); totals 423 /
      7,166.
    - §3: every topic edge ≥ 10 in the graph and every edge < 10 in the threshold table equals
      A.1 (28 graph edges, 18 rows). A.2 totals equal the A.1 `States` row.
    - The numbers of §5 and §6 come from a group script, validated on `9c75812`: it reproduces the
      previous document's numbers (one exception kept as is: §6 "dayInfo/DayInfo 270" is the total
      from all of `States`, unchanged).
    - Line references in §7.1, §7.3 and §8 checked by grep on the base.
    - Relative links resolve. `git ls-files --eol`: `w/crlf`, no bare LF or CR.
  - **Per-model preference table (§10)**, the baseline for replicating 2a (references / consumers;
    consumers only):

    | Target | JC6416 (views) | JC4020 (direct) | JC6432 (direct) |
    |---|---|---|---|
    | `DataPrefs` | 393 / 15 | 393 / 15 | 400 / 16 |
    | `LogicPrefs` | 223 / 9 | 254 / 7 | 258 / 10 |
    | `PresentationPrefs` | 50 / 7 | 74 / 8 | 62 / 8 |
    | `DisplayPrefs` | 55 / 7 | 43 / 7 | 55 / 7 |
    | JC6432 extra colors | — | — | 9 / 3 |
    | `SystemPrefs` | 6 / 2 | 6 / 2 | 6 / 2 |
    | `UserContent` | 7 / 1 | in direct | in direct |
    | `UserPreferences` direct | 50 | 50 | 57 |
    | Total | 784 | 820 | 847 |

    Replication progress is logged per step: before, after, and the sum per consumer.
- **2a.2.10** — tools before the replication:
  - **`tools/count_deps.py`:** `EXCLUDE['Preferences/DataPrefs'] = {'place', 'zmanim'}`: the struct
    fields `PlacePrefs place;` / `ZmanimPrefs zmanim;` (`DataPrefs.h:56-57`) are no longer counted
    as references to the global `place` (`DB/Place`) and the array `zmanim` (`DB/Shita`). Same
    mechanism as the existing manual exclusions. Docstring: the "374 edges" validation now names
    the previous map (`dependencies.md` at `e7f8efb`); the exclusion and its validation added.
    - **Regression** (tool at `72b432e` vs new): text and `--json` identical on JC4020 and JC6432
      at `72b432e` and on JC6416 at `e76aaea`. On JC6416 at `72b432e`: 423 → 421 edges,
      7,166 → 7,164 references; the only differences are the two edges gone
      (`DataPrefs → DB/Place` `place` 1, `→ DB/Shita` `zmanim` 1).
  - **`tools/gen_deps_doc.py`** (new): the generator of the marked blocks of `dependencies.md`
    (§4 edges, §9 includes, A.1–A.4), from the scratchpad script of 2a.2.9, without the
    name-collision override (no longer needed). Modes: `print <block>`, `write` (marked blocks only,
    CRLF; refuses a document that is not pure CRLF or lacks a marker), `check` (blocks up to date,
    §4 total line, §3 graph / threshold table and the A.2 total row against A.1; exit code 1 on a
    mismatch), `report` (helper data for the hand-written sections), `selfcheck`. Writes no
    bytecode. JC6416 only (fixed row lists, stops on a missing or new unit). The script for the
    §5 / §6 group numbers stays out of `tools/` (owner's decision).
    - **`selfcheck`:** `git archive 9c75812` into a temporary folder, compared with the document at
      `e7f8efb`: OK. §4: 374 rows, #, from, to, references and mark identical; examples in another
      tie order in 94 rows; identifier count differs only in the two known edges. A.2, A.3
      identical; A.1 only `Prefs → States` `inc`; A.4 only the unit names in parentheses and the
      `UserPreferences` include. These are the allowed differences, encoded in the script.
  - **`CLAUDE.md`:** working rules — exception to "edit only with the editor tools": `gen_deps_doc.py
    write` may rewrite only the marked blocks of `dependencies.md`, with CRLF. Stage line: next is
    replicating 2a.1 to JC4020.
  - **`dependencies.md` regenerated** (`write`, then hand edits with the editor tool; `check` OK,
    421 edges, 7,164 references). Diff against `72b432e`, classified by a scratchpad script:
    - Generated blocks: §4 rows 369–370 (`DataPrefs → DB/Place`, `→ DB/Shita`) removed, rows
      371–423 renumbered (53 rows, `#` column only); A.1 `Prefs → DB` 2 → empty; A.4
      `Preferences/DataPrefs` loses `DB 2`. §9, A.2, A.3 identical.
    - Hand-written, all following from the exclusion: §1 the collision moves from "known tool
      limitations" to "manual exclusions"; §1 totals 421 / 7,164; §1 "updating the document" names
      the generator and drops the collisions from the mark rule; §3 threshold row `Prefs → DB`
      removed; §4 legend "name collision" removed and total line 421 / 7,164; §5.5 "from
      Preferences 2" removed; §5.6 graph edge `DATA → DB` and its note removed; §7.1 "not
      included" note removed; A.1 note on `Prefs → DB` removed.
    - One more hand edit, not in the approved list: §1 base line, "counted with the tool of 2a.2.8
      (`bc7d1b5`)" → "the tool of 2a.2.10", which the exclusion made wrong.
    - `git ls-files --eol`: `w/crlf`, no bare LF or CR.
  - No firmware source changed; no build.
- **2a.1** (JC4020) — per-layer preference views, replicated from JC6416 (first replication step):
  - **Mapping (before editing):** `JC4020/src/Preferences/UserPreferences.h` is byte-identical to
    the JC6416 file at `d01dd0f` (port 7.08; same blob), and so is `UserPreferences.cpp`. No field
    exists in only one model; no shared field differs in name, type, index or default. `textPrefs`
    exists and is used (`TextCommandHandler` r6; F3 does not change this), so `UserContent` is the same.
  - **Files:** `DataPrefs.h`, `LogicPrefs.h`, `PresentationPrefs.h`, `DisplayPrefs.h`,
    `SystemPrefs.h`, `UserContent.h`, `PrefsViews.cpp` under `JC4020/src/Preferences/`,
    byte-identical to the JC6416 files (`cmp`): same form, structs, field names and order as
    2a.1 (JC6416); 72 `static_assert`s (71 fields + `userContent.textPrefs`). Written with the
    editor tool; LF in the working copy, like the JC6416 views (owner's decision; F20).
  - **Not in any struct:** `forceWrite` (central registry).
  - **No consumer changes.** No existing file changed.
  - **Verification rule for replicating 2a.1** (approved; applies to JC6432 too):
    - Binary: `firmware.bin`, RAM and Flash unchanged; `nm -C --size-sort` identical; ELFs
      identical after `objcopy --strip-debug`; `tools/compare_bins.py` reports "identical" or
      "Only metadata differs".
    - `tools/count_prefs.py`: output byte-identical (text and `--json`); it skips the view units.
    - `tools/count_deps.py`: not identical, because the view units are new units. Every edge of
      the base is unchanged (refs, ids, labels); the new edges start only in the view units and
      `PrefsViews`, and each equals the matching edge in JC6416; the consumer sum (consumers →
      `UserPreferences` and the views, `dependencies.md` §10) is unchanged, per consumer.
    - Coverage: every `UserPreferences` field except `forceWrite` and `textInputN` is in exactly
      one struct, the one of its layer in `preferences.md` §4, in BLE index order; `textInputN`
      through `userContent.textPrefs`.
  - **Verification** (reference: clean build of `3c02e13`, built before editing; both builds
    clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC4020` (env `esp32dev`) succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "identical" (same md5). The full ELF is byte-identical too, so the
      ELF SHA-256 in `app_desc` does not change. Cause: `PrefsViews.cpp.o` has no code or data,
      and the linker (`--gc-sections`) drops it entirely, debug sections included (listed only
      under "Discarded input sections" in `firmware.map`); no other file includes the view headers.
    - `count_prefs.py`: identical (text and `--json`).
    - `count_deps.py`: 350 → 363 edges, 6,825 → 7,125 references. No edge removed or changed. The
      13 new edges (300 references) all equal JC6416: `PrefsViews → UserPreferences` 72,
      `→ LogicPrefs` 27, `→ DataPrefs` 26, `→ DisplayPrefs` 9, `→ PresentationPrefs` 8,
      `→ SystemPrefs` 1, `→ UserContent` 1; `→ UserPreferences` from `LogicPrefs` 56, `DataPrefs`
      54, `DisplayPrefs` 20, `PresentationPrefs` 18, `SystemPrefs` 4, `UserContent` 4.
    - Replication progress (`dependencies.md` §10): consumer sum 820 before and after, equal for
      each of the 25 consumers; all accesses still direct (moved in 2a.2).
    - Coverage script against `preferences.md` §4: 71 fields, each exactly once, each in the
      struct of its layer, index order kept; `forceWrite` absent; `UserContent` refers to `textPrefs`.
    - `git status`: only the 7 new files and the two documents. Keys, indexes, types, defaults,
      load order and contract strings unchanged (`UserPreferences.*` untouched).
    - `git ls-files --eol`: new files `w/lf`, like the JC6416 views.
  - `preferences.md` §8: one line on the JC4020 views. `dependencies.md` is regenerated after
    the replication to JC6432.
  - **Clock test:** not needed; the image is byte-identical.
- **2a.2** (JC4020) — replication of 2a.2: same rule and layer order as 2a.2 (JC6416). Sub-steps
  2a.2.1–2a.2.5; there is no 2a.2.6 (the tools already count access through the views), and the
  four `isZeetPref` lines of `CoreService` (2a.2.7 in JC6416) move with Data in 2a.2.5.
  Verification of every sub-step (approved plan): reference = clean build of the previous
  sub-step's tree, candidate = clean build after editing, both with `SOURCE_DATE_EPOCH=1763337600`;
  `nm -C --size-sort` identical; ELFs identical after `objcopy --strip-debug`; `compare_bins.py`
  only `app_desc` (ELF SHA-256) and trailer; `count_prefs.py` byte-identical to the output after
  2a.1 (JC4020); `count_deps.py`: every consumer's sum (`dependencies.md` §10) unchanged, only its
  split moves from `UserPreferences` to the view, all other edges unchanged; occurrence counts as
  in the plan; 0 compiler warnings; edited files keep `w/crlf`. Scripts in the scratchpad.
- **2a.2.1** (JC4020) — System: `userPreferences.bt` → `systemPrefs.bt`, 6 places:
  `src/States/ConfigState.cpp` (5) and `src/main.cpp` (1), plus `#include "Preferences/SystemPrefs.h"`
  after the last existing include in both files. Same as 2a.2.1 (JC6416).
  - Edits made with the editor tool only (per-field `replace_all`).
  - **Verification** (reference: the 2a.1 (JC4020) candidate build, same source tree as `109bbb2`):
    - `pio run -d JC4020` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 820 → 820; `ConfigState` 9
      (`SystemPrefs` 5, `UserPreferences` 4), `main` 4 (`SystemPrefs` 1, `UserPreferences` 3);
      all other edges unchanged.
    - `grep`: no `userPreferences.bt` outside `Preferences/`; `systemPrefs.bt` 6 times.
    - `git diff --stat`: 2 files, 8 insertions, 6 deletions (6 replaced lines + 2 includes).
      `git ls-files --eol`: both `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.2** (JC4020) — UserContent and Presentation:
  - `userPreferences.textPrefs` → `userContent.textPrefs`, 7 places in
    `src/BLE/TextCommandHandler.cpp`, plus `#include "Preferences/UserContent.h"`. One of them
    (`:79`) is inside the commented-out TEXT write block (F3); replaced by the 2a.2 rule. JC6416
    has no commented-out line here.
  - `userPreferences.<field>` → `presentationPrefs.<field>` for `leil`, `dateFormat`,
    `clockFormat`, `timeFormat`, and → `presentationPrefs.layout.<field>` for `middleLine`,
    `upLine`: 74 occurrences (27 of them `.layout.`) on 62 lines in 8 files: `src/States/SetClockState.cpp`
    (26), `src/States/SetTochenState.cpp` (17, JC4020 only), `src/Screens/RegsScreen.cpp` (12),
    `src/States/MainState.cpp` (9), `src/Logic.cpp` (4), `src/DB/Zman.cpp` (3), `src/DB/DayInfo.cpp`
    (2), `src/States/MainStateTimeSets.cpp` (1), plus `#include "Preferences/PresentationPrefs.h"`
    in each. `safa` and `layout.screen` have no consumers in JC4020.
  - Differences from 2a.2.2 (JC6416; 50 occurrences, 4 `.layout.`, 7 files): `middleLine` and
    `upLine` are used (`Logic`, `MainState`, `SetTochenState`); `dateFormat` is read twice in
    `MainState` and not in `MainStateTimeSets`; `leil` is also read in `MainStateTimeSets`.
  - Not changed: the string literal in `src/States/MainStateTimeSets.cpp:53`
    (`Serial.println("if(userPreferences.leil==1 && dayInfo.afterShkiaa)");`, live code, owner's
    decision; F21). Only the condition on the line above was replaced (single-line edit, no
    `replace_all` in this file).
  - Edits made with the editor tool only (per-field `replace_all` elsewhere).
  - **Verification** (reference: the 2a.2.1 (JC4020) candidate build, source tree of `70b1b3b`):
    - `pio run -d JC4020` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 820 → 820, each consumer's sum
      unchanged. Moved: `TextCommandHandler` → `UserContent` 6 (the commented line is not
      counted); → `PresentationPrefs`: `SetClockState` 26, `SetTochenState` 17, `RegsScreen` 12,
      `MainState` 9, `Logic` 4, `Zman` 3, `DayInfo` 2, `MainStateTimeSets` 1 (74). All other edges
      unchanged.
    - `grep`: none of the 7 fields remains as `userPreferences.<field>` outside `Preferences/`,
      except the string literal above; `userContent.textPrefs` 7, `presentationPrefs.` 74.
    - `git diff --stat`: 9 files, 78 insertions, 69 deletions (69 replaced lines + 9 includes).
      `git ls-files --eol`: all 9 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.3** (JC4020) — Display:
  - `userPreferences.<field>` → `displayPrefs.<field>` for `brightness`, `colorUp`, `colorMiddle`,
    `colorBottomT`, `colorBottomV`, `colorYom`, `colorParasha`, `colorIluy`, `clockFont`:
    43 occurrences on 35 lines in `src/Screens/RegsScreen.cpp` (16), `src/Logic.cpp` (11),
    `src/States/ColorState.cpp` (8), `src/States/FontState.cpp` (3), `src/main.cpp` (2),
    `src/States/BrightnessState.cpp` (2), `src/States/MainState.cpp` (1), plus
    `#include "Preferences/DisplayPrefs.h"` in each. Same files as 2a.2.3 (JC6416; 55 occurrences):
    in JC4020 `colorYom`, `colorParasha` and `colorIluy` are read only by `RegsScreen` (not by
    `Logic` or `ColorState`). No commented-out lines in this layer.
  - Edits made with the editor tool only (whole-file `replace_all` in `ColorState.cpp`,
    `FontState.cpp` and `BrightnessState.cpp`, where every `userPreferences.` access is a Display
    field; per-field elsewhere).
  - **Verification** (reference: the 2a.2.2 (JC4020) candidate build, source tree of `52654a8`):
    - `pio run -d JC4020` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 820 → 820, each consumer's sum
      unchanged. Moved to `DisplayPrefs`: `RegsScreen` 16, `Logic` 11, `ColorState` 8, `FontState`
      3, `main` 2, `BrightnessState` 2, `MainState` 1 (43); `ColorState`, `FontState` and
      `BrightnessState` no longer reference `UserPreferences`. All other edges unchanged.
    - `grep`: none of the 9 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `displayPrefs.` 43 times.
    - `git diff --stat`: 7 files, 42 insertions, 35 deletions (35 replaced lines + 7 includes).
      `git ls-files --eol`: all 7 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.4** (JC4020) — Logic:
  - `userPreferences.<field>` → `logicPrefs.<field>` for the Logic fields used in JC4020: 254
    occurrences on 220 lines in `src/States/SetTochenState.cpp` (120, JC4020 only),
    `src/Screens/RegsScreen.cpp` (48), `src/States/MainStateInfoSets.cpp` (43),
    `src/States/MainStateTimeSets.cpp` (36), `src/Logic.cpp` (4), `src/DB/DayInfo.cpp` (2),
    `src/States/MainState.cpp` (1), plus `#include "Preferences/LogicPrefs.h"` in each. `loazi` has no
    consumer in JC4020; `intervalIluy`, `intervalTime` and `intervalInfo` are read only by `RegsScreen`.
    No commented-out lines in this layer. No line mixes a Logic field with another layer's field;
    `eda`, `hul`, `homa` in `MainStateInfoSets.cpp` / `MainStateTimeSets.cpp` stay on
    `userPreferences` (Data layer, next sub-step).
  - Differences from 2a.2.4 (JC6416; 223 occurrences, 9 files): `SetTochenState` replaces
    `SetTochenInfoState`, `SetTochenTimeState` and `SetInputText`; `interval` is used instead of the
    three `interval*`; `yom`, `parasha`, `ZSZ`, `ZYS`, `ZPS` and `IPO` are used
    (`MainStateInfoSets`, `MainStateTimeSets`); `Logic.cpp` reads 4 (JC6416: 8).
  - Edits made with the editor tool only: whole-file `replace_all` in `SetTochenState.cpp`, where
    after 2a.2.2 every `userPreferences.` access is a Logic field; per-field `replace_all` elsewhere
    (`interval*` before `interval`, `ZSZ1` before `ZSZ`). The string literal in
    `MainStateTimeSets.cpp` (now `:54`) is untouched (F21).
  - **`SetTochenState.cpp` check** (approved addition): before the edit, 120 `userPreferences.`
    accesses, all of them one of 23 fields that are all Logic in `preferences.md` §4; after the edit,
    120 `logicPrefs.` and 0 `userPreferences.`. The parameter name `userPreferencesZman` (no dot) is
    not affected.
  - **Verification** (reference: the 2a.2.3 (JC4020) candidate build, source tree of `b7498c6`):
    - `pio run -d JC4020` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 820 → 820, each consumer's sum
      unchanged. Moved to `LogicPrefs`: `SetTochenState` 120, `RegsScreen` 48, `MainStateInfoSets`
      43, `MainStateTimeSets` 36, `Logic` 4, `DayInfo` 2, `MainState` 1 (254). All other edges
      unchanged.
    - `grep`: no Logic field remains as `userPreferences.<field>` outside `Preferences/`;
      `logicPrefs.` 254 times.
    - `git diff --stat`: 7 files, 227 insertions, 220 deletions (220 replaced lines + 7 includes).
      `git ls-files --eol`: all 7 files `w/crlf`, as before.
  - **Recorded:** F21 (documentation only, no code change).
  - **Clock test:** not needed; compiled code identical.
- **2a.2.5** (JC4020) — Data (last layer; includes the 2a.2.7 (JC6416) `CoreService` lines):
  - `userPreferences.<field>` → `dataPrefs.place.<field>` (`textPlace`, `ofek`, `nz_lat`,
    `nz_long`, `nz_height`, `nz_hul`, `timezone`, `shaonKaiz`, `homa`), `dataPrefs.zmanim.<field>`
    (`shita`, `zriha`, `shkiaa`, `rabenuTam`, `zeetCochavim`, `zmanTalit`, `alotHashahar`,
    `minhaG`, `sozksMga`, `soztMga`, `plagMinha`, `zeetShabat`, `zeetZom`, `nerot`) and
    `dataPrefs.minhagim.<field>` (`eda`, `hul`, `shanim`): 401 occurrences (place 89, zmanim 245,
    minhagim 67) on 348 lines in 16 files: `src/DB/Shita.cpp` (144), `src/States/MinhagState.cpp`
    (48), `src/Screens/RegsScreen.cpp` (42), `src/States/OfekState.cpp` (41), `src/DB/DayInfo.cpp`
    (40), `src/DB/Place.cpp` (19), `src/Logic.cpp` (19), `src/States/EdotState.cpp` (17),
    `src/States/MainStateInfoSets.cpp` (7), `src/DB/Omer.cpp` (6), `src/States/SetClockState.cpp`
    (5), `src/BLE/CoreService.cpp` (4, the `isZeetPref` range check), `src/DB/DB.cpp` (3),
    `src/States/ShitaState.cpp` (3), `src/States/MainStateTimeSets.cpp` (2), `src/States/MainState.cpp`
    (1), plus `#include "Preferences/DataPrefs.h"` after the last existing include in each.
  - Same files and the same count per file as 2a.2.5 + 2a.2.7 (JC6416) (397 + 4). The views are
    references to the same objects (`PrefsViews.cpp` asserts the addresses), so the pointer
    comparison in `isZeetPref` is unchanged.
  - Commented-out debug lines replaced too, including the label text inside their string literals
    (owner's decision in 2a.2.5 (JC6416)): `src/DB/Place.cpp:127-132` and
    `src/States/OfekState.cpp:259-260`.
  - Edits made with the editor tool only. Files whose remaining `userPreferences.` accesses are all
    Data fields: whole-file `replace_all` into the file's main group, then per-field `replace_all`
    to move the fields of the other groups (`DB.cpp`, `Omer.cpp`, `ShitaState.cpp`,
    `SetClockState.cpp`: one group, no correction; `DayInfo.cpp`, `Place.cpp`, `Shita.cpp`,
    `Logic.cpp`, `RegsScreen.cpp`, `EdotState.cpp`, `MinhagState.cpp`, `OfekState.cpp`: with
    corrections). Per-field `replace_all` in `CoreService.cpp` (registry members and a comment),
    `MainStateTimeSets.cpp` (F21 string), `MainStateInfoSets.cpp` and `MainState.cpp`.
  - **Verification** (reference: the 2a.2.4 (JC4020) candidate build, source tree of `1cc19bf`):
    - `pio run -d JC4020` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,055,536 bytes before and after (delta 0). RAM 61,132 and Flash 2,048,961
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 820 → 820, each consumer's sum
      unchanged; 393 references moved to `DataPrefs` (the 8 in commented-out lines are not
      counted). All other edges unchanged.
    - `grep`: none of the 26 fields remains as `userPreferences.<field>` outside `Preferences/`,
      comments included; every `dataPrefs.<group>.<field>` uses the group the field belongs to
      (script check).
    - `git diff --stat`: 16 files, 364 insertions, 348 deletions (348 replaced lines + 16
      includes). `git ls-files --eol`: all 16 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
  - **End of 2a.2 (JC4020):** `grep "userPreferences\s*\."` outside `Preferences/` finds only:
    - Registry members:
      - `resetSimplePrefs` — `src/States/ConfigState.cpp:81`, `:143`.
      - `resetTextPrefs` — `src/States/ConfigState.cpp:101`, `:142`.
      - `simplePrefs` — `src/BLE/CoreService.cpp:563`, `:710`.
      - `allPrefs` — `src/BLE/CoreService.cpp:644`.
      - `printAllValues` — `src/main.cpp:63`, commented out.
    - Not accesses:
      - A Hebrew comment that ends with the word: `src/BLE/CoreService.cpp:614`.
      - The debug string `src/States/MainStateTimeSets.cpp:55` (F21; documented exception).
    - Same list as at the end of 2a.2 (JC6416), plus the F21 string; `dayIndex` was removed in 7.08.
    - **Replication progress** (`dependencies.md` §10, consumers only): 820 references before 2a.2
      and after it, equal for each of the 25 consumers. By target after 2a.2: `DataPrefs` 393,
      `LogicPrefs` 254, `PresentationPrefs` 74, `DisplayPrefs` 43, `SystemPrefs` 6, `UserContent`
      6, `UserPreferences` direct 44 (registry members, `extern UserPreferences userPreferences;`
      declarations, `UserPref` types in `TextCommandHandler`, `setupPreferences`). The §10 baseline
      for JC4020 was Data 393, Logic 254, Presentation 74, Display 43, System 6, `UserPreferences`
      direct 50; the 6 `textPrefs` references of `TextCommandHandler` moved from the direct row to
      `UserContent`.
  - `CLAUDE.md`: stage line — 2a.1–2a.2 done in JC6416 and JC4020; next JC6432, after deciding
    the classification of `colorIvri`, `colorLoazi` and `colorParashaV`.
- **2a.1** (JC6432) — per-layer preference views, replicated from JC6416:
  - **Mapping (before editing):** `JC6432/src/Preferences/UserPreferences.cpp` is identical to
    JC6416. `UserPreferences.h` differs only as documented (`preferences.md` §8, F2, F9): three
    fields only in JC6432, `colorIvri` (72, default 6), `colorLoazi` (73, 1), `colorParashaV` (74, 6),
    all U8, read by `Logic` (→ `mainScreen.colorX` → `Text` color in `MainScreen`) and edited by
    `ColorState`, colors only; `screen` 75 (default 2) and `bt` 76; defaults `colorYom` 1,
    `colorParasha` 1, `leil` 1. No name or type difference. `textPrefs` exists and is used
    (`TextCommandHandler`, 7, no commented-out line), so `UserContent` is the same.
  - **Classification (owner's decision):** the three colors in `DisplayPrefs`, in BLE index order
    like every field, so they come after `clockFont` at the end of the struct; `screen` in
    `presentationPrefs.layout`.
  - **Files:** `DataPrefs.h`, `LogicPrefs.h`, `PresentationPrefs.h`, `SystemPrefs.h`,
    `UserContent.h` byte-identical to JC6416 (`cmp`). `DisplayPrefs.h`: JC6416 plus 3 fields and 3
    initializers at the end. `PrefsViews.cpp`: JC6416 plus 3 `static_assert`s after `clockFont`
    (75 in total: 74 fields + `userContent.textPrefs`). LF in the working copy, like the other views
    (F20). No existing file changed; no consumer changes.
  - **Verification** (2a.1 replication rule, with the exact expectations for the three colors
    approved before the step; reference: clean build of `39840bf`, built before editing; both
    builds clean, with `SOURCE_DATE_EPOCH=1763337600`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical. The full ELF is
      byte-identical too; `tools/compare_bins.py`: "identical" (`PrefsViews.cpp.o` is discarded by
      `--gc-sections`, as in JC4020).
    - `count_prefs.py`: identical (text and `--json`).
    - `count_deps.py`: 395 → 408 edges, 7,579 → 7,891 references. No base edge removed or changed.
      13 new edges, all from view units: `PrefsViews → UserPreferences` 75, `PrefsViews →
      DisplayPrefs` 12 and `DisplayPrefs → UserPreferences` 26, exactly as expected (JC6416: 72, 9,
      20; +3 fields); the other 10 equal JC6416. Consumer sum 847 → 847, equal for each of the 29
      consumers.
    - Coverage script (layers from `preferences.md` §4, the three colors from §8; indexes from
      JC6432's `UserPreferences.h`): 74 fields, each exactly once, each in the struct of its layer,
      index order kept in every struct (`DisplayPrefs` ends with 72, 73, 74; `layout.screen` 75;
      `bt` 76); `forceWrite` absent; `UserContent` refers to `textPrefs`.
    - `git status`: only the 7 new files and the documents. Keys, indexes, types, defaults, load
      order and contract strings unchanged (`UserPreferences.*` untouched).
  - **Found while mapping (no change):** `dependencies.md` §10 gives the three JC6432 colors as
    "9 / 3" (references / consumers). The 9 references are right, but only 2 consumers reference
    them (`Logic` 3, `ColorState` 6). To be corrected when `dependencies.md` is regenerated.
  - `preferences.md` §8 JC6432: one line on the views. `notes.md` (main-screen layout direction):
    `Screens/MainScreen.cpp:91` reads `dateFormat` from Presentation, a cross-layer coupling for the
    layer-isolation stage; not changed in 2a.
  - **Clock test:** not needed; the image is byte-identical.
- **2a.2** (JC6432) — replication of 2a.2, same rule, layer order and verification as 2a.2 (JC4020)
  (sub-steps 2a.2.1–2a.2.5; the `CoreService` `isZeetPref` lines move with Data). Expected
  `TextCommandHandler → UserContent`: 7 (no commented-out `textPrefs` line in JC6432). The three
  JC6432 colors move with Display, `screen` with Presentation (`layout`).
- **2a.2.1** (JC6432) — System: `userPreferences.bt` → `systemPrefs.bt`, 6 places:
  `src/States/ConfigState.cpp` (5) and `src/main.cpp` (1), plus `#include "Preferences/SystemPrefs.h"`
  after the last existing include in both files. Same as 2a.2.1 (JC6416).
  - Edits made with the editor tool only (per-field `replace_all`).
  - **Verification** (reference: the 2a.1 (JC6432) candidate build, source tree of `2dcc4e5`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 31 bytes in `app_desc` (ELF SHA-256; one
      byte happens to match) and 33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 847 → 847; `ConfigState` 9
      (`SystemPrefs` 5, `UserPreferences` 4), `main` 4 (`SystemPrefs` 1, `UserPreferences` 3);
      all other edges unchanged.
    - `grep`: no `userPreferences.bt` outside `Preferences/`; `systemPrefs.bt` 6 times.
    - `git diff --stat`: 2 files, 8 insertions, 6 deletions (6 replaced lines + 2 includes).
      `git ls-files --eol`: both `w/crlf`, as before.
  - F20: "Models" line updated (JC6432 has views since 2a.1).
  - **Clock test:** not needed; compiled code identical.
- **2a.2.2** (JC6432) — UserContent and Presentation:
  - `userPreferences.textPrefs` → `userContent.textPrefs`, 7 places in
    `src/BLE/TextCommandHandler.cpp`, plus `#include "Preferences/UserContent.h"`. Same as JC6416.
  - `userPreferences.<field>` → `presentationPrefs.<field>` for `leil`, `dateFormat`,
    `clockFormat`, `timeFormat`, and → `presentationPrefs.layout.<field>` for `middleLine`,
    `upLine`, `screen`: 62 occurrences (14 of them `.layout.`) on 50 lines in 8 files:
    `src/States/SetClockState.cpp` (27), `src/States/MainState.cpp` (13), `src/Screens/RegsScreen.cpp`
    (12), `src/Logic.cpp` (3), `src/DB/Zman.cpp` (3), `src/DB/DayInfo.cpp` (2),
    `src/Screens/MainScreen.cpp` (1), `src/States/FontState.cpp` (1), plus
    `#include "Preferences/PresentationPrefs.h"` in each. `safa` has no consumer.
  - Differences from 2a.2.2 (JC6416; 50 occurrences, 4 `.layout.`, 7 files): `layout.screen` is used
    (10: `MainState` 7, two of them writes on up/down in the main screen; `Logic`, `FontState`,
    `SetClockState` 1 each); `Screens/MainScreen.cpp:91` reads `dateFormat` (display layer reading
    a preference, `notes.md`); `MainState` reads `dateFormat` 3 times and `MainStateTimeSets` not
    at all. No commented-out lines in these two layers.
  - Edits made with the editor tool only (per-field `replace_all`).
  - **Verification** (reference: the 2a.2.1 (JC6432) candidate build, source tree of `fe1a932`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 847 → 847, each consumer's sum
      unchanged. Moved: `TextCommandHandler` → `UserContent` 7; → `PresentationPrefs`:
      `SetClockState` 27, `MainState` 13, `RegsScreen` 12, `Logic` 3, `Zman` 3, `DayInfo` 2,
      `MainScreen` 1, `FontState` 1 (62). All other edges unchanged.
    - `grep`: none of the 8 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `userContent.textPrefs` 7, `presentationPrefs.` 62.
    - `git diff --stat`: 9 files, 66 insertions, 57 deletions (57 replaced lines + 9 includes).
      `git ls-files --eol`: all 9 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.3** (JC6432) — Display:
  - `userPreferences.<field>` → `displayPrefs.<field>` for `brightness`, `colorUp`, `colorMiddle`,
    `colorBottomT`, `colorBottomV`, `colorYom`, `colorParasha`, `colorIluy`, `clockFont` and the
    JC6432 colors `colorIvri`, `colorLoazi`, `colorParashaV`: 64 occurrences on 56 lines in
    `src/Logic.cpp` (20), `src/States/ColorState.cpp` (20), `src/Screens/RegsScreen.cpp` (16),
    `src/States/FontState.cpp` (3), `src/main.cpp` (2), `src/States/BrightnessState.cpp` (2),
    `src/States/MainState.cpp` (1), plus `#include "Preferences/DisplayPrefs.h"` in each. Same
    files as 2a.2.3 (JC6416; 55 occurrences); the three JC6432 colors add 9 (`Logic` 3,
    `ColorState` 6). No commented-out lines in this layer.
  - Edits made with the editor tool only: whole-file `replace_all` in `ColorState.cpp`,
    `FontState.cpp` and `BrightnessState.cpp` (every remaining `userPreferences.` access is a Display
    field); per-field elsewhere, `colorParashaV` before `colorParasha` (prefix).
  - **Verification** (reference: the 2a.2.2 (JC6432) candidate build, source tree of `abda574`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      32 in the trailer (two ranges; one byte happens to match).
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 847 → 847, each consumer's sum
      unchanged. Moved to `DisplayPrefs`: `Logic` 20, `ColorState` 20, `RegsScreen` 16, `FontState`
      3, `main` 2, `BrightnessState` 2, `MainState` 1 (64); `ColorState` and `BrightnessState` no
      longer reference `UserPreferences`. All other edges unchanged.
    - `grep`: none of the 12 fields remains as `userPreferences.<field>` outside `Preferences/`;
      `displayPrefs.` 64 times; `displayPrefs.colorParashaV` 3 (`Logic` 1, `ColorState` 2).
    - `git diff --stat`: 7 files, 63 insertions, 56 deletions (56 replaced lines + 7 includes).
      `git ls-files --eol`: all 7 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.4** (JC6432) — Logic:
  - `userPreferences.<field>` → `logicPrefs.<field>` for the 27 Logic fields: 259 occurrences on
    226 lines in `src/States/SetTochenTimeState.cpp` (52), `src/Screens/RegsScreen.cpp` (48),
    `src/States/SetTochenInfoState.cpp` (46), `src/States/MainStateInfoSets2.cpp` (41),
    `src/States/MainStateInfoSets1.cpp` (36), `src/States/MainStateTimeSets.cpp` (21),
    `src/Logic.cpp` (8), `src/States/SetInputText.cpp` (4), `src/DB/DayInfo.cpp` (2),
    `src/States/MainState.cpp` (1), plus `#include "Preferences/LogicPrefs.h"` in each.
    No line mixes a Logic field with another layer's field; `eda`, `hul`, `homa` in
    `MainStateInfoSets1/2.cpp` / `MainStateTimeSets.cpp` stay on `userPreferences` (Data, next
    sub-step).
  - Differences from 2a.2.4 (JC6416; 223 occurrences, 9 files): `MainStateInfoSets` is split into
    `…1.cpp` and `…2.cpp` (77 together, JC6416 41); `loazi` is not read in `MainStateTimeSets`
    (21, JC6416 22); `SetTochenTimeState` reads `intervalTime` once more (52, JC6416 51).
  - One commented-out line, replaced by the 2a.2 rule: `src/States/MainStateInfoSets2.cpp:240`
    (`:239` before the include), `// if(!((dayInfo.anenu||dayInfo.yaaleVeyavo) && logicPrefs.tfila==1)) …`.
    JC6416 has no commented-out line in this layer.
  - Edits made with the editor tool only: whole-file `replace_all` in `SetTochenTimeState.cpp`,
    `SetTochenInfoState.cpp` and `SetInputText.cpp` (every `userPreferences.` access is a Logic
    field; the parameter `userPreferencesZman` has no dot and is not affected); per-field
    elsewhere (`interval*` before `interval`, `ZSZ1` before `ZSZ`).
  - **Verification** (reference: the 2a.2.3 (JC6432) candidate build, source tree of `17a1b2a`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 847 → 847, each consumer's sum
      unchanged. Moved to `LogicPrefs`: `SetTochenTimeState` 52, `RegsScreen` 48,
      `SetTochenInfoState` 46, `MainStateInfoSets2` 40 (the commented line is not counted),
      `MainStateInfoSets1` 36, `MainStateTimeSets` 21, `Logic` 8, `SetInputText` 4, `DayInfo` 2,
      `MainState` 1 (258). All other edges unchanged.
    - `grep`: no Logic field remains as `userPreferences.<field>` outside `Preferences/`, comments
      included; `logicPrefs.` 259 times.
    - `git diff --stat`: 10 files, 236 insertions, 226 deletions (226 replaced lines + 10
      includes). `git ls-files --eol`: all 10 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
- **2a.2.5** (JC6432) — Data (last layer; includes the `CoreService` `isZeetPref` lines):
  - `userPreferences.<field>` → `dataPrefs.place.<field>`, `dataPrefs.zmanim.<field>` and
    `dataPrefs.minhagim.<field>` for the 26 Data fields: 408 occurrences (place 91, zmanim 245,
    minhagim 72) on 352 lines in 17 files: `src/DB/Shita.cpp` (144), `src/States/MinhagState.cpp`
    (48), `src/Screens/RegsScreen.cpp` (42), `src/States/OfekState.cpp` (41), `src/DB/DayInfo.cpp`
    (40), `src/Logic.cpp` (19), `src/DB/Place.cpp` (19), `src/States/EdotState.cpp` (17),
    `src/States/MainStateInfoSets1.cpp` (7), `src/States/MainStateInfoSets2.cpp` (7),
    `src/DB/Omer.cpp` (6), `src/States/SetClockState.cpp` (5), `src/BLE/CoreService.cpp` (4),
    `src/DB/DB.cpp` (3), `src/States/ShitaState.cpp` (3), `src/States/MainStateTimeSets.cpp` (2),
    `src/States/MainState.cpp` (1), plus `#include "Preferences/DataPrefs.h"` after the last
    existing include in each.
  - Same files and per-file counts as 2a.2.5 + 2a.2.7 (JC6416), except the `MainStateInfoSets`
    split (7 + 7; JC6416 7). Commented-out debug lines replaced, label text included, as in JC6416:
    `src/DB/Place.cpp:127-132` and `src/States/OfekState.cpp:260-261`.
  - Edits made with the editor tool only, as in 2a.2.5 (JC4020): whole-file `replace_all` into the
    file's main group, then per-field `replace_all` for the fields of the other groups, in files
    whose remaining accesses are all Data (`DB.cpp`, `Omer.cpp`, `ShitaState.cpp`,
    `SetClockState.cpp`, `MainStateTimeSets.cpp`: one group; `DayInfo.cpp`, `Place.cpp`,
    `Shita.cpp`, `Logic.cpp`, `RegsScreen.cpp`, `EdotState.cpp`, `MinhagState.cpp`,
    `OfekState.cpp`, `MainStateInfoSets1/2.cpp`: with corrections); per-field in `CoreService.cpp`
    (registry members and a comment) and `MainState.cpp`.
  - **Verification** (reference: the 2a.2.4 (JC6432) candidate build, source tree of `bb40ec0`):
    - `pio run -d JC6432` succeeds; 0 compiler warnings before and after.
    - `firmware.bin` 2,072,400 bytes before and after (delta 0). RAM 61,924 and Flash 2,065,825
      bytes, unchanged.
    - `nm -C --size-sort`: identical. `objcopy --strip-debug`: byte-identical.
    - `tools/compare_bins.py`: "Only metadata differs" — 32 bytes in `app_desc` (ELF SHA-256) and
      33 in the trailer.
    - `count_prefs.py`: identical. `count_deps.py`: consumer sum 847 → 847, each consumer's sum
      unchanged; 400 references moved to `DataPrefs` (the 8 in commented-out lines are not
      counted). All other edges unchanged.
    - `grep`: none of the 26 fields remains as `userPreferences.<field>` outside `Preferences/`,
      comments included; every `dataPrefs.<group>.<field>` uses the group the field belongs to
      (script check).
    - `git diff --stat`: 17 files, 369 insertions, 352 deletions (352 replaced lines + 17
      includes). `git ls-files --eol`: all 17 files `w/crlf`, as before.
  - **Clock test:** not needed; compiled code identical.
  - **End of 2a.2 (JC6432):** `grep "userPreferences\s*\."` outside `Preferences/` finds only:
    - Registry members:
      - `resetSimplePrefs` — `src/States/ConfigState.cpp:81`, `:143`.
      - `resetTextPrefs` — `src/States/ConfigState.cpp:101`, `:142`.
      - `simplePrefs` — `src/BLE/CoreService.cpp:563`, `:710`.
      - `allPrefs` — `src/BLE/CoreService.cpp:644`.
      - `printAllValues` — `src/main.cpp:63`, commented out.
    - A Hebrew comment that ends with the word: `src/BLE/CoreService.cpp:614`. Not an access.
    - Same list as at the end of 2a.2 (JC6416).
    - **Replication progress** (`dependencies.md` §10, consumers only): 847 references before 2a.2
      and after it, equal for each of the 29 consumers. By target after 2a.2: `DataPrefs` 400,
      `LogicPrefs` 258, `DisplayPrefs` 64 (the §10 rows `DisplayPrefs` 55 + JC6432 colors 9),
      `PresentationPrefs` 62, `UserContent` 7, `SystemPrefs` 6, `UserPreferences` direct 50
      (§10: 57; the 7 `textPrefs` references of `TextCommandHandler` moved to `UserContent`).
  - **End of 2a.1–2a.2:** all three models access preferences through the layer views; only the
    registry members remain on `userPreferences`.
  - `CLAUDE.md`: stage line — 2a.1–2a.2 done in all three models; next: regenerate
    `dependencies.md`, then 2a.3.
- **docs: `dependencies.md` regenerated after 2a.2 in all models** (rule in its §1):
  - `JC6416/src` unchanged since `7b60c79` (`git diff --stat` empty), so §2–§9 and the generated
    blocks stay as they are: `gen_deps_doc.py check` OK (421 edges, 7,164 references).
  - §1: base line notes the regeneration at `ae29f42`.
  - §10 re-checked against JC4020 and JC6432 at `ae29f42`: the model notes still hold (missing /
    extra units, the JC4020 `TextCommandHandler` edges 2 and 3, edges 2–4 of §7.1 absent in
    JC4020); added `Screens/MainScreen → Preferences/PresentationPrefs` 1 in JC6432. The table
    "before the replication of 2a" replaced by "after 2a.1–2a.2", from `count_deps.py`.
  - **Checks:** consumer sums JC6416 784, JC4020 820, JC6432 847, unchanged. Direct access to
    `UserPreferences` in every model only from registry members, `extern` declarations, the
    `UserPref` / `UserPreferences` types and `setupPreferences` (script over the edge labels). The
    §10 discrepancy is gone: the JC6432 colors are inside `DisplayPrefs`, 9 references from 2
    consumers. A second run of `count_deps.py`, `count_prefs.py` (all three models) and
    `gen_deps_doc.py print` gives identical output. `git ls-files --eol`: `w/crlf`, no bare LF
    or CR.
  - Not changed: `notes.md` cites `MainScreen.cpp:91` (correct at 2a.1 (JC6432)); the line is
    `:92` since the include of 2a.2.2.
  - `CLAUDE.md`: stage line — next is 2a.3, planned with the owner before any execution.
- **2a.3 — plan** (decided with the owner): each layer owns its global object in its own file
  (`DataPrefs dataPrefs;` in `DataPrefs.cpp`, etc.); `UserPreferences` keeps only pointers, the
  lists, load, reset and the `clk` namespace. The definitions (key, type, index, default) move with
  the objects as they are; a table in flash is a separate step (2a.4). The `CoreService` index
  checks and actions (`== 4`, `== 6`, the zeet keys) do not change until the actions mechanism. One
  table for all three models (same preferences and indexes, as in JC6432); defaults stay per model.
  The image changes from 2a.3.3 on, so the reference is a contract tool on the PC, and in the end a
  test on a clock.
- **2a.3.0** (all models) — preference contract tool, `tools/prefs_contract.py`:
  - **What it does:** compiles the real registry code of a model on the PC, against minimal host
    stubs (`tools/prefs_contract/stub/`: `String`, `Serial`, `Preferences`, an empty
    `States/States.h`), runs `setupPreferences()` and prints what was built: every entry of
    `simplePrefs`, `textPrefs` and `allPrefs` in list order (key, type tag and code, index,
    default, C++ type, namespace), every `Preferences` call made during the load (`[nvs-load]`:
    namespace, function, key, default passed), and every line printed to Serial meanwhile
    (`[serial]`). The firmware sources are only read.
  - **Compiler:** MSVC (VS 2022 Community, cl 14.44, found with `vswhere` / `vcvars64.bat`); g++
    if it is on PATH or with `--cxx g++`. There is no g++ on the PC (owner's decision: MSVC).
  - **Source file rule:** `<src>/Preferences/*.cpp` except `PrefsViews.cpp` (compile-time checks
    only), plus `tools/prefs_contract/main.cpp`. **To maintain:** when the preference files move
    (layer folders, or the per-layer owner files of 2a.3.3), `source_files()` must be updated so
    that every file that defines a preference object or the registry is compiled. Also in the
    tool's docstring.
  - **NVS name limit:** the tool fails if any key or namespace name, in the lists or in the NVS
    log, is longer than 15 characters (`NVS_KEY_NAME_MAX_SIZE` 16 with the terminator).
  - **Baselines:** `docs/contracts/prefs/JC6416.txt`, `JC4020.txt`, `JC6432.txt` (LF), written
    only by `prefs_contract.py write`. `check` compares, prints a unified diff and exits 1 on any
    difference.
  - **Verification:**
    - Counts: JC6416 and JC4020 `simplePrefs` 72, `textPrefs` 50, `allPrefs` 122, `nvs-load` 123
      (`begin` + 122 gets); JC6432 75 / 50 / 125 / 126. `[serial] 0` in all three. JC6416 and
      JC4020 outputs identical except the model name.
    - Script against the documents: key, index, type and default of every `simplePrefs` entry
      equal `docs/contracts/ble.md` §8 (main table, the † defaults and the 72–76 table) for all
      three models, and `preferences.md` §4 for JC6416: 0 differences. All 50 texts: `PT_STR`,
      index 0, default `""`, `clk`.
    - Sensitivity (scratch copies of `JC6416/src/Preferences`, one change each): `eda` default
      1 → 2 changes exactly its `simplePrefs`, `allPrefs` and `nvs-load` lines; `ofek` index
      6 → 16 changes exactly its `simplePrefs` and `allPrefs` lines; swapping the `push_back` of
      `eda` and `ofek` swaps exactly those lines in the three sections. A 16-character key fails
      the tool (exit 2) in all three sections.
    - Second run byte-identical for every model. `check` passes after `write`; with one baseline
      value changed by hand it reports the line and exits 1 (restored, identical to the first run).
    - The stub's `PreferenceType` matches
      `framework-arduinoespressif32/libraries/Preferences/src/Preferences.h:19-21` (2.0.17), same
      enumerators in the same order.
    - `git status`: only the new tool files and baselines, and the rule in `CLAUDE.md`. No firmware
      file changed, so no build is needed.
  - `CLAUDE.md`: second exception to the editor-tools rule (`prefs_contract.py write`, baselines
    only).
  - Follow-up commit: `check` reads a CRLF baseline as LF (`core.autocrlf=true` makes the
    baselines CRLF in the working copy after a checkout). Tested on a CRLF copy of `JC6432.txt`.
- **2a.3.1** (all models) — unused registry functions removed from
  `src/Preferences/UserPreferences.h`:
  - `saveAll`, `resetAll` (between `loadAll` and `resetSimplePrefs`), and `printAll`,
    `setValueByKey`, `getValueByKey`, `getPref` (after `resetTextPrefs`, up to the end of the
    class): 93 lines deleted per model, nothing else. `grep` in `src/`: none of them is called
    (`setValueByKey` called `p->set`, which does not exist; the template was never instantiated).
    `main.cpp:63` keeps the commented-out `printAllValues` (another name, never existed).
  - Not touched (owner's decision): `UserPrefBase` / `UserPref`, including `isModified()` and
    `print()` (`UserPreferences` itself has neither), the includes, and the remaining comments.
    Remaining registry functions: `init`, `loadAll`, `resetSimplePrefs`, `resetTextPrefs`.
  - **Since Sync 5:** `printAll` is back (production 7.09 keeps it for debug). Documented
    deviation from this step.
  - **Verification** (reference: clean builds of `bd8f222`, `SOURCE_DATE_EPOCH=1763337600` in
    reference and candidate):
    - `pio run -d <model>` succeeds for all three; 0 compiler warnings before and after.
    - `firmware.bin`: JC6416 2,060,800, JC4020 2,055,536, JC6432 2,072,400 bytes, before and after
      (delta 0). RAM 61,100 / 61,132 / 61,924 and Flash 2,054,229 / 2,048,961 / 2,065,825 bytes,
      unchanged.
    - `tools/compare_bins.py`: "Only metadata differs" for all three. `objcopy --strip-debug`:
      byte-identical. `nm -C --size-sort`: identical.
    - `prefs_contract.py check`: OK for all three.
    - `count_prefs.py`: identical. `count_deps.py --compare` (tree of `bd8f222` vs working tree):
      no edge changed in any model, so every consumer sum is unchanged (784 / 820 / 847).
    - `git diff --stat`: 3 files, 279 deletions. `git ls-files --eol`: `w/crlf`, as before.
  - **Docs:** `preferences.md` §7 (remaining registry functions) and §9.1 (`forceWrite` is not read
    anywhere; it stays because of the BLE contract, index 1). Recorded F22.
  - **Clock test:** not needed; compiled code identical.
- **2a.3.2** (JC6416, JC4020) — one preference table for all models. **Contract change**
  (owner's decision; documented, not in the field until this repo becomes production):
  - `src/Preferences/UserPreferences.h`: members `colorIvri`, `colorLoazi`, `colorParashaV` after
    `IORT`; in the constructor, between `IORT` and `screen`, `colorIvri` (72, default 6),
    `colorLoazi` (73, 1), `colorParashaV` (74, 6), each with `// 1-7`, as in JC6432; `screen`
    72 → 75, `bt` 73 → 76; the three `simplePrefs.push_back` after `IORT`. Existing defaults and
    the `screen` comment (per-model meaning) unchanged.
  - `src/Preferences/DisplayPrefs.h` (3 fields, 3 initializers) and `PrefsViews.cpp`
    (3 `static_assert`): now byte-identical to JC6432 in both models. JC6432 not changed.
  - **Whitespace (cosmetic):** the two separator lines after `colorIvri` / `colorLoazi` are empty
    in JC6416 / JC4020; in JC6432 they hold 24 spaces. The editor tool does not write
    whitespace-only lines, and a script edit is not allowed. Two whitespace-only differences that
    already existed are left as they are (the line before `colorIvri`: 24 vs 48 spaces; a blank
    line before `{` in JC6416 / JC4020).
  - **Verification** (reference: the 2a.3.1 candidate builds, clean builds of the tree of
    `9f9c39e`, `SOURCE_DATE_EPOCH=1763337600` in both):
    - `pio run -d JC6416` / `-d JC4020` succeed; 0 compiler warnings before and after.
    - JC6416: `firmware.bin` 2,060,800 → 2,061,040 (+240); RAM 61,100 → 61,172 (+72); Flash
      2,054,229 → 2,054,457 (+228). JC4020: 2,055,536 → 2,055,760 (+224); RAM 61,132 → 61,204
      (+72); Flash 2,048,961 → 2,049,181 (+220). RAM: 3 `UserPref<uint8_t>` objects of 24 bytes.
      The image fits the OTA slot as before (31.3%).
    - `prefs_contract.py print` against the 2a.3.0 baselines, JC6416 and JC4020 (same diff):
      exactly 3 new entries (`colorIvri` 72 / 6, `colorLoazi` 73 / 1, `colorParashaV` 74 / 6) in
      `simplePrefs`, `allPrefs` and `nvs-load` (`getUChar`, after `IORT`), and the indexes of
      `screen` (75) and `bt` (76); counts 75 / 50 / 125 / 126. Nothing else. JC6432 unchanged.
      The JC6416 output against JC6432 differs only in the defaults of `colorYom`, `colorParasha`,
      `leil` and `screen`. Baselines rewritten with `write`; `check` OK for all three.
    - `diff -w` of `UserPreferences.h`, JC6432 vs JC6416 / JC4020: only the defaults of
      `colorYom`, `colorParasha`, `leil`, `screen`, the `screen` comment and the existing blank
      line before `{`. JC6416 and JC4020 `UserPreferences.h` byte-identical.
    - `count_prefs.py`: identical. `count_deps.py --compare` (tree of `9f9c39e`): only edges of the
      view units changed — `DisplayPrefs -> UserPreferences` +6, `PrefsViews -> DisplayPrefs` +3,
      `PrefsViews -> UserPreferences` +3. Every consumer sum unchanged (784 / 820).
    - Contract strings: UUIDs, namespaces (`clk`, `sec`, `sys`) and the advertisement prefix not
      touched; keys: 3 added, none changed.
    - `git ls-files --eol`: `UserPreferences.h` `w/crlf`, the views `w/lf` (F20), as before; no
      mixed line endings (byte count).
  - **Contract change, recorded:**
    - **NVS:** existing keys and types unchanged; the index is not stored in NVS. The 3 new keys
      are not written at boot (load only reads). They are written by a BLE `PREF_OP_WRITE` of one
      of them, and by a reset from the menu: `resetSimplePrefs` (`SetResetPrefState`,
      `SetResetAllState`) calls `reset()` → `save()` for every `simplePrefs` entry. 3 more u8
      entries in `clk`.
    - **BLE (JC6416, JC4020):** `PREF_OP_SYNC` sends 75 / 76 instead of 72 / 73 for `screen` /
      `bt`, and 72–74 when a color differs from its default. `PREF_OP_READ` / `PREF_OP_WRITE`
      accept the 3 keys (before: `INVALID_KEY`). Nothing reads them on these models.
    - **App:** per the owner, the table in `preference_constants.dart` ends at `IORT` (71) and
      `getDefinitionByIndex` returns null for an unknown index; no effect on the app.
    - **Clock test in 2a.3.5:**
      - JC6416 / JC4020: SYNC returns 75 when `screen` differs from its default, 76 when `bt` is
        off.
      - READ of `colorIvri` returns 6.
      - "Reset settings", then reboot: no change on the display.
      - A clock with 7.08 NVS keeps all its settings after the update.
      - JC6416: BLE write `colorIvri` = 3, then SYNC sends 72; the app keeps working without error
        (`getDefinitionByIndex` returns null); then reset.
      - JC6432: confirm explicitly that SYNC sends 72–76 when they differ from the defaults (as
        today).
  - **Docs:** `preferences.md` §2, §3, §4 (rows 72–76), §8 (`MainScreen.cpp:92`);
    `ble.md` §7 item 1 and §8 "In this repo since 2a.3.2" (the 7.08 snapshot kept); `notes.md`
    (`MainScreen.cpp:92`; direction "colors per area" in the layout model); F2 status; "Planned for
    next production release" (sync note).
  - `CLAUDE.md`: stage line — 2a.3.0–2a.3.2 done; next 2a.3.3 (ownership in JC6416), then stop
    for review with the owner.
- **2a.3.3 — plan for JC6416 alone cancelled** (owner's decision, Sync 6): 2a.3.3 is planned again
  for all three models.
- **2a.3.3** (all models, one step; 2a.3.4 cancelled) — the layer objects own their preferences.
  Commits: one per model (`src/Preferences/` only), then one docs commit.
  - **Commit fix** (owner's decision): the staged `git rm` of `PrefsViews.cpp` in JC4020 and JC6432
    first went into the JC6416 commit (as in Sync 6). The three commits were rebuilt with plumbing
    (`read-tree` into a temporary index, `update-index --cacheinfo`, `write-tree`, `commit-tree`,
    same messages and dates), `main` moved with `update-ref` after a backup branch; local, not
    pushed. Final tree identical to the original (`HEAD^{tree}` = `76efa5c^{tree}`); each commit
    holds only its model's files. Backup branch deleted after the checks. New rule in `CLAUDE.md`:
    commit with a path in multi-model steps.
  - **Method** (same in every model):
    - Each layer has a header and its own `.cpp` (new: `DataPrefs.cpp`, `LogicPrefs.cpp`,
      `PresentationPrefs.cpp`, `DisplayPrefs.cpp`, `SystemPrefs.cpp`, `UserContent.cpp`). Global objects
      without `new` (`DataPrefs dataPrefs;` …), declared `extern` in the header instead of
      `inline constexpr`. Fields are `UserPref<T>` without `&`; one constructor per struct that holds
      `UserPref` fields (`PlacePrefs`, `ZmanimPrefs`, `MinhagimPrefs`, `LogicPrefs`, `LayoutPrefs`,
      `PresentationPrefs`, `DisplayPrefs`, `SystemPrefs`, `UserContent`).
    - Split: Data 26 (place 9, zmanim 14, minhagim 3), Logic 27, Presentation 8 (5 + layout 3),
      Display 12, System 1, UserContent 50 = 124. `forceWrite` stays in the registry.
    - Each `.cpp` has `static Preferences &prefs = userPreferences.prefs;`, so every definition line
      (key, default, type, index, `&prefs`) and its comment lines moved as they are. The constructor
      header is on its own line (`X::X() :`); the only change in a moved line is the dropped comma
      after the last entry of each constructor (8 lines), and the `forceWrite` line
      (`UserPreferences::UserPreferences() : forceWrite(...)`).
    - `UserContent` keeps `std::vector<UserPrefBase *> &textPrefs` (first member, bound to
      `userPreferences.textPrefs` in its constructor), so `userContent.textPrefs[i]` in
      `TextCommandHandler` is unchanged.
    - Registry (`UserPreferences.h`): the 124 members and the inline constructor removed; it keeps
      `forceWrite`, the three vectors, `prefs`, `init`, `loadAll`, `resetSimplePrefs`,
      `resetTextPrefs`, `printAll`, `UserPrefBase` / `UserPref` (unchanged). Its constructor is in
      `UserPreferences.cpp`: the same 125 `push_back` lines in the same order and indentation, the
      target changed only (`&textPlace` → `&dataPrefs.place.textPlace`, `&bt` → `&systemPrefs.bt`,
      `&textInput1` → `&userContent.textInput1`), and the two `insert` lines as they were.
    - `PrefsViews.cpp` deleted (`git rm`). `CoreService`, consumer code and the tools not changed.
    - Static initialization: no constructor depends on another translation unit. Layer constructors
      take only the address `&userPreferences.prefs`; the registry constructor stores constant
      addresses and does not read through them; the first read is `setupPreferences()` in `setup()`.
  - **Editing:** editor tools only. The deletions in `UserPreferences.h` matched the whitespace-only
    lines with Edit, so the approved byte-range script fallback was not needed. Whitespace-only
    lines in the moved block (71 × 24 spaces, 1 × 48) are empty in the new files.
  - **Reference:** clean builds of `f084e48` (`SOURCE_DATE_EPOCH=1763337600` in reference and
    candidate), `git archive HEAD` for `count_deps` / `count_prefs`.
  - **Verification** (all passed in every model before the first commit):

    | Model | `firmware.bin` | RAM | Flash | Compiler warnings |
    |---|---|---|---|---|
    | JC6416 | 2,061,072 → 2,061,744 (+672) | 61,172 → 61,172 (0) | 2,054,501 → 2,055,161 (+660) | 0 |
    | JC4020 | 2,055,680 → 2,056,336 (+656) | 61,204 → 61,204 (0) | 2,049,097 → 2,049,753 (+656) | 0 |
    | JC6432 | 2,072,448 → 2,073,120 (+672) | 61,924 → 61,924 (0) | 2,065,869 → 2,066,549 (+680) | 0 |

    - Contract tool: `check` OK for all three, without `write` (75 / 50 / 125 / 126, `[serial] 0`).
      With the `.cpp` files in reverse order (scratch wrapper around `source_files()`, `main.cpp`
      last; MSVC runs the constructors in link order) the output is byte-identical to the baseline.
    - Moved lines (script): only whitespace-only lines ignored, all other lines byte for byte. All 125
      blocks (entry + comments) once each, in source order per constructor; the only differences are
      the dropped last commas and the `forceWrite` line. Struct order = constructor order (no
      `-Wreorder`), field types equal the removed registry members, 125 `push_back` in the same order
      with the right layer path, `insert` lines identical.
    - Across models after the step: all new files identical, except `DisplayPrefs.cpp` in JC6432
      (`colorYom`, `colorParasha` default 1). `UserPreferences.h`: only the two lines that differed
      before (`~UserPreferences` indentation in JC6432, a whitespace-only line in JC4020 / JC6432).
    - `count_deps --compare` (archive of HEAD vs working tree): the same 19 edge changes in all three
      models, all inside `Preferences/`: the 7 `PrefsViews` edges (150) removed;
      `UserPreferences →` Data 26, Logic 27, Presentation 8, Display 12, System 1, UserContent 50
      (124, the `push_back` lines); layer → `UserPreferences`: Data 54 → 29, Logic 56 → 30,
      Display 26 → 15, Presentation 18 → 11, System 4 → 4, UserContent 4 → 55. These are
      **references** on one edge each; "UserContent 4 → 55" is the reference count of the single edge
      `UserContent → UserPreferences` (`UserPref` ×50, `UserPrefBase`, `UserPreferences`, `extern`,
      `userPreferences.textPrefs`, `userPreferences.prefs`). Totals: JC6416 421 / 7,175 → 420 / 7,131;
      JC4020 392 / 7,135 → 391 / 7,091; JC6432 439 / 7,891 → 438 / 7,847 (−1 edge, −44 references
      each: −150 + 124 − 18). Consumer sums unchanged: 784 / 820 / 847.
    - `count_prefs`: 124 differences per model, every one an added `UserPreferences r1/w0` (the
      `push_back` lines). The split between existing and new output lines is not fixed (JC6416: 69 / 55;
      five preferences had no mention before).
    - RAM: `userPreferences` (4,512 bytes) became seven `.bss` objects — `dataPrefs` 688,
      `displayPrefs` 288, `logicPrefs` 648, `presentationPrefs` 192, `systemPrefs` 24, `userContent`
      2,604, `userPreferences` 68 — contiguous, 4,512 bytes, the same span, no gaps, in all three
      models. The local `prefs` references take no memory (no symbol).
    - Symbols added / removed (all three, script against the expected set): the 9 layer constructors,
      `UserContent::~UserContent`, `_GLOBAL__sub_I/D_*` of the new files, the six layer objects;
      removed `UserPreferences::~UserPreferences` (now inlined) and the old `_GLOBAL__sub_I/D_`. No
      other data object changed size. `UserPreferences::UserPreferences()`: 8,918 → 2,145 bytes.
    - Flash: every function whose size changed classified by the flash check below — JC6416 101,
      JC4020 103, JC6432 114 functions; none left unexplained:

      | Category | JC6416 | JC4020 | JC6432 |
      |---|---|---|---|
      | pref-addressing | 23 | 25 | 26 |
      | rodata-moved | 6 | 8 | 7 |
      | alignment | 43 | 44 | 47 |
      | regalloc | 9 | 10 | 10 |
      | scheduling | 8 | 8 | 9 |
      | ref-indirection | 3 | 1 | 3 |
      | same-effects | 7 | 5 | 10 |
      | comdat | 1 | 1 | 1 |
      | planned (registry constructor) | 1 | 1 | 1 |

      Two-bases and registry-offset occur, always together with a later category, so no function is
      counted under them alone (each function gets one category, the first level that matches).
      `compare_bins.py`: code differs, as expected; not a criterion. The image fits the OTA slot (31.4%).
    - Contract strings: no UUID, `LEDHAARETS`, namespace or `prefs.begin` line in any diff.
    - `git ls-files --eol`: `UserPreferences.*` `w/crlf`; the headers and the six new `.cpp` `w/lf`
      (F20 now also covers the new files). No mixed line endings (byte count). Scope: only
      `<model>/src/Preferences/`.
  - **Flash check when the image is not identical** (owner's decision; permanent criterion, applies
    to 2a.4 too). It is a **filter for surprises, not a proof of equivalence**. The proofs are the
    contract tool in both file orders, the moved source lines, `count_deps`, RAM, and the clock test.
    - Scope: every function present in both images whose size changed (`nm -C -S`), plus the expected
      set of added / removed symbols and data objects.
    - Decoding (scratch script `flashcheck2.py`): instruction regions from `.xt.prop` (all
      `.xt.prop*` sections), jump tables (`.rodata` words pointing into the function), exception
      landing pads (unreached region starts and blocks after a jump or no-return call; entry state =
      intersection of the states after every call). Calls to no-return functions end a block.
    - A dataflow over the CFG tracks registers and stack slots holding a preference address; every
      access is named by field and `UserPref` member (`[ofek._value]`). Non-preference literals: same
      symbol, `.rodata` with the same bytes (moved), pointer words inside `.rodata` that point to the
      same content, or the same constant. Calls through a literal (`l32r` + `callx`, linker
      relaxation) count as calls to that function; argument registers by the callee's arity.
    - Accepted categories, each function in exactly one (the first that matches):
      pref-addressing (only preference addressing differs, e.g. a removed `addmi`), two-bases (the
      number of preference base loads differs), registry-offset (offsets on `this` inside a
      `UserPreferences` member), rodata-moved, alignment (narrow / wide forms, NOPs, zero fill),
      regalloc (consistent register renaming), scheduling (same instructions per basic block, other
      order), ref-indirection (the extra load of `userContent.textPrefs`; cost of the design, to be
      revisited in 2a.4), same-effects, comdat (a template instance kept from another translation
      unit; calls may differ only in inlined standard helpers), planned (the registry constructor).
    - **Same-effects** ("different code, same effects"), mechanical, by multisets, not order: every
      preference access maps to the same field and member, with the same kind (read / write) and the
      same count; the same calls with the same count; every non-preference literal as above. A
      function that fails is a deviation: stop.
  - **Tools:** not changed. The docstrings of `count_deps.py` (`inline constexpr` view globals) and of
    `prefs_contract.py` / `tools/prefs_contract/main.cpp` (`PrefsViews.cpp`) still describe the views;
    correct for trees before 2a.3.3, to be updated in the docs step. The flash-check scripts are in
    the scratchpad only; adding them to `tools/` is a separate step if the owner wants it.
  - **Docs:** `dependencies.md` (`gen_deps_doc.py write`; by hand §1 unit rule and totals, §2,
    §4 total, §5.6 graph and notes, §8 globals, §10 per-model totals after 2a.3.3, A.1 note);
    `check` OK (420 edges, 7,131 references). `preferences.md` §1, §2, §7, §8. `notes.md` status.
    `ble.md` not changed: it describes production, where the constructor is still in
    `UserPreferences.h`.
  - **Clock test (added to 2a.3.5, all three models):**
    - Before flashing, on 709A: change at least one preference of every layer from its default (Data,
      Logic, Presentation incl. layout, Display, System) and one text; note the values. After
      flashing: all of them, and the rest of the settings, are kept.
    - Change one preference from the clock menu, reboot: it is kept.
    - BLE SYNC; READ / WRITE of one preference per layer, including `shita` and `ofek` (range check
      and immediate apply).
    - Text write and read (JC6416, JC6432; JC4020 by key with `PREF_OP_WRITE`, F3).
    - "Reset settings" and "Reset texts", then reboot.

### Decisions

- **Flash check when the image is not identical** (owner's decision, 2a.3.3; applies to 2a.4 and
  later): a filter for surprises, not a proof of equivalence. Accepted categories and the
  "same-effects" rule as recorded in 2a.3.3. The proofs are the contract tool in both file orders,
  the moved source lines, `count_deps`, RAM and the clock test.
- **New generation after the refactor (major 8)** (owner's decision, after Sync 6; also in
  `CLAUDE.md`, with the contract rule):
  - App and firmware versions depend on each other, and the app forces updates. No BLE
    compatibility with old versions is needed.
  - Frozen, must stay compatible: the advertisement (`LEDHAARETS` + version), authentication, OTA
    and the partition table. The build must fit the existing slot. There is no update by cable.
  - NVS: on the first boot the firmware migrates. Texts, shita, location and eda carry over, the
    rest is reset, and a generation key is written. The migration is safe to run again. Before
    OTA the app keeps a backup of the same data, as a safety net, and guides the new setup with a
    wizard.
  - The app checks `MAX_SUPPORTED_FW_MAJOR` and shows an "app update required" banner. This check
    goes into the app before the new generation is released. Downgrade is not offered to users,
    only as a support action.
  - The legacy checks in the app and `docs/contracts/ble.md` §9 are removed when the new
    generation is released.
  - Until then, the current contract rule applies.

## Planned for next production release

Decided by the owner. To be done outside this repo, then ported here with the next sync.
Nothing planned.

## Findings

Line numbers in F1–F9 refer to commit `e9c7639`; in F10–F18, to commit `dcd2cdb` (7.07); from
F19 on, to commit `d01dd0f` (7.08).
Status updates inside older findings use `dcd2cdb` line numbers and say so.
Only findings that describe the current state are kept (open, accepted, intentional, won't fix,
documentation only). Fixed findings were removed; their text is in git at `707d5d2`. Numbers are
not reused.

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
- **Also (JC4020):** the settings menu without `resetText` (`src/States/ConfigState.cpp:23`, `:26`
  at Sync 5) is intentional: JC4020 has no texts.
- **Status:** closed as a finding (owner's decision); moved to the refactor plan
  (`docs/architecture/models-plan.md` §6). Goal: the BLE layer is the same in all three models,
  and the decision whether to accept texts moves to the logic layer when logic is separated from
  BLE. On JC4020 the `TEXT_OP_*` responses stay `INVALID_INDEX`, because they are part of the
  contract; only the place where the decision is made changes. Closed.

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
- **Status:** documentation only (owner's decision). The format with `\0` is the contract. Closed.

### F5 — OTA service is not behind the Core auth gate

- **Where:** gate in `src/BLE/CoreService.cpp:867-873` applies only to the Core CMD
  characteristic. `src/BLE/FirmwareService.cpp` and `src/BLE/OtaProtocol.cpp` do not reference
  `authManager`.
- **Impact:** a connected client can run OTA without HELLO/PIN. Recorded for awareness;
  behavior is part of the current field contract.
- **Models:** same in all three.
- **Status:** accepted risk (owner's decision). OTA is performed only through the app, and the
  app authenticates first. Closed.

### F6 — `MENU_OP_GOTO` accepts undefined menu codes

- **Where:** `src/BLE/CoreService.cpp:242-253`
- **What:** the comment at `:244` says undefined values are rejected, but only `0` (NONE) is.
  Any other byte is cast to `MenuCommand` and acknowledged with OK.
- **Status:** documentation only (owner's decision). An undefined code is cast and acknowledged;
  `Logic::goToMenuCommand` returns `&mainState` in its `default` branch (`src/Logic.cpp:961`
  JC6416, `:953` JC4020, `:994` JC6432, at Sync 5), so the clock returns to the home screen.
- **Models:** same in all three.

### F7 — `SYS_ERR_NAME_TOO_LONG` is defined but never sent

- **Where:** defined `src/BLE/CoreProtocol.h:284`; `SYS_OP_SET_NAME` over 24 bytes returns
  `CORE_ERR_INVALID_VALUE` (0x05) instead (`src/BLE/CoreService.cpp:795-798`).
- **Impact:** the app sees 0x05. The 0x05 response is the contract.
- **Models:** same in all three.
- **Status:** documentation only (owner's decision). 0x05 is the contract. Closed.

### F9 — Preference defaults differ between the app and the firmware

- **Where (firmware):** `src/Preferences/UserPreferences.h`
  - `colorYom` (18): JC6416 / JC4020 `6` (`:602`); JC6432 `1` (`:605`).
  - `colorParasha` (19): JC6416 / JC4020 `6` (`:605`); JC6432 `1` (`:608`).
- **Where (app, as reported by the user):** `lib/config/preference_constants.dart`,
  `lib/models/app_preferences.dart:90`, `lib/providers/ble/ble_preferences_mixin.dart:154-180`.
  App defaults: `colorYom` = 6, `colorParasha` = 6.
- **Mismatches:** `colorYom` and `colorParasha` on JC6432.
- **Impact:** `PREF_OP_SYNC` reports only values that differ from the *firmware* default. When
  the clock holds its firmware default, the index is not reported, and the app shows its own
  default, which is a different value. E.g. JC6432 with `colorYom` = 1 → app shows 6.
- **Action:** documentation only, no fix. Firmware defaults are part of the BLE contract
  (`docs/contracts/ble.md` §8).
- **Status:** won't fix (owner's decision). After the refactor the issue will no longer be
  relevant. Closed.

### F10 — Selecting a city overwrites `nz_hul`

- **Where:** `src/States/OfekState.cpp:510-519` (`SetOfekState::printUpdateSucc`), lines `:516-517`.
  Called from the menu (`SetOfekState::okAction`, `:67-71`) and from BLE `PREF_OP_WRITE ofek` ≠ 0
  (`src/BLE/CoreService.cpp:669-678`).
- **What:** the function writes `nz_hul` (and `hul`) from `places[ofek].hul` of the selected
  city. Afterwards, when the user goes back to the custom place (`ofek` = 0), `nz_hul`
  still holds the city's value, not the value the custom place had before.
- **Impact:** `nz_hul` selects the DST rules (`src/DB/DayInfo.cpp:198-199`, Israel or USA) and
  `zmanTalit` in `myZmanim::setDefault` (F15).
- **Status:** intentional. `nz_hul` is the Israel/abroad flag of the *active* place, and selecting
  a city sets it, even at the expense of the custom place's value. Closed.
- **Models:** same in all three.

### F11 — A rollback below 7.05 with a new `shita` or zeet value stored

- **What:** `numOfShitot` is 10 (`shita` 0–9: `myZmanim` = 8, `pozen` = 9) and
  `numOfZeetCochavin` 28 (`zeetMethod` rows 0–28; method 28 = 7.08°) (`src/DB/Shita.h:10,
  12-13`). Firmware below 7.05 has 8 and 27. The arrays are indexed with the stored value without
  a check:
  - `shitot[userPreferences.shita]` in `DayInfo::initDay` (`src/DB/DayInfo.cpp:20`), at every day
    calculation including boot. Also `src/States/MinhagState.cpp:309-642` (10 places) and
    `src/States/ShitaState.cpp:38`.
  - `zeetMethod[...]` in the menu states (`src/States/MinhagState.cpp:310, 347, 606, 643`), with
    the temp value taken from `zeetCochavim`, `rabenuTam`, `zeetShabat`, `zeetZom`
    (`src/Logic.cpp:68-77`, `MinhagState.cpp:315, 352, 611, 648`). The same list serves all four
    keys. The calculation itself (`Shita::zeetCochavimMethods`) is a `switch` with a `default`
    branch and does not index the array.
- **Risk:** rolling back to firmware below 7.05 with a new value already stored: `shitot[8]` /
  `[9]` reads past the array of 8 → most likely a crash in `initDay` at every boot (boot loop). A
  stored 28 in one of the four `zeetMethod` keys reads past the array when that menu is shown.
  NVS is not reset by an install. `pozen::setDefault` itself stores 28 in `zeetCochavim` and
  `zeetZom`, so selecting that shita is enough.
- **BLE:** out-of-range writes of `shita`, `ofek` and the four zeet keys are rejected
  (`docs/contracts/ble.md` §6.5; older firmware: §9).
- **Status:** accepted (owner's decision): the rollback is not blocked in the app, because it is a
  manual, deliberate install. Closed.
- **Models:** same in all three (`Shita.h`, `Shita.cpp` identical).

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

### F20 — Layer view files are LF in the working copy

- **What:** the 7 view files under `src/Preferences/` (`DataPrefs.h` … `PrefsViews.cpp`) are LF in
  the working copy, in all three models. All other
  source files are CRLF in the working copy (`core.autocrlf=true`; the index is LF for all). The
  editor tool writes
  LF, and the files have not been checked out since they were created.
- **Impact:** none on the build or the image. Git warns "LF will be replaced by CRLF the next time
  Git touches it".
- **Status:** closed, no action needed (owner's decision) — working copy only; the index is LF for all files, and a fresh
  checkout makes the views CRLF like the rest.
- **Models:** JC6416, JC4020, JC6432.

### F22 — `fromBytes<String>` is not bounded by the value length

Fixed in JC6416/JC4020/JC6432; still open in BA6416, BC6416, JC8040, Laniado (out of refactor
scope).

### F23 — `stamp_app_desc.py`: two docstring lines joined

- **Where:** `stamp_app_desc.py:12` (all three models).
- **What:** the docstring text that dropped `HARDWARE_VERSION` joined two lines without a space: "…מזהה בוודאותדגם וגרסה — …"
  (should be "בוודאות" + newline + "דגם וגרסה").
- **Impact:** none. Docstring only; the script and the image are not affected.
- **Source:** the file is copied from `ledhaarets-build`, so a fix belongs there and comes in with
  a sync. Not changed here.
- **Models:** same in all three.
