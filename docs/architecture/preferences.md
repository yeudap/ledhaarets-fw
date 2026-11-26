# העדפות לפי שכבה — JC6416

סיווג של כל העדפות המשתמש לשכבות היעד, כבסיס להפרדה. בסוף ההפרדה לכל שכבה יהיה מבנה העדפות משלה, ורישום מרכזי אחד יטפל ב-NVS ובאינדקסי ה-BLE.
**מפתחות NVS, ה-namespace ואינדקסי ה-BLE לא משתנים.** המסמך מתאר את המצב הקיים ואת הסיווג. אין בו הצעות מימוש או תיקונים.

הסיווג סופי לפי תשובות בעל הפרויקט (סעיף 9). התשובות מתועדות גם ב-[notes.md](notes.md).

קשור: [dependencies.md](dependencies.md) (שכבות הקוד), [notes.md](notes.md), [../contracts/ble.md](../contracts/ble.md) (חוזה ה-PREF).

## 1. בסיס ושיטה

- **בסיס:** `JC6416`. המסמך מתאר את הקוד הנוכחי. ההפניות לקוד הן לפי קובץ וסמל (פונקציה, מחלקה, שדה). מספרי שורות נשארו רק בהפניות לקוד האפליקציה, שנמצא מחוץ לריפו (כפי שדווח). הבדלים בדגמים האחרים מופיעים בסעיף 8.
- **מקור ההגדרות:** מ-2a.3.3, הבנאים של השכבות, כל שכבה ב-`.cpp` שלה ([DataPrefs.cpp](../../JC6416/src/Preferences/DataPrefs.cpp), [LogicPrefs.cpp](../../JC6416/src/Preferences/LogicPrefs.cpp), [PresentationPrefs.cpp](../../JC6416/src/Preferences/PresentationPrefs.cpp), [DisplayPrefs.cpp](../../JC6416/src/Preferences/DisplayPrefs.cpp), [SystemPrefs.cpp](../../JC6416/src/Preferences/SystemPrefs.cpp), [UserContent.cpp](../../JC6416/src/Preferences/UserContent.cpp)), ו-`forceWrite` בבנאי הרישום ב-[UserPreferences.cpp](../../JC6416/src/Preferences/UserPreferences.cpp). סדר הרשימות (`push_back`) בבנאי הרישום. עד 2a.3.3: בנאי `UserPreferences` ב-`UserPreferences.h`.
- **שכבות היעד:**
  - **נתונים**: מיקום, שיטה, מנהגים
  - **לוגיקה**: מה מוצג, מתי ולכמה זמן
  - **הצגה**: שפה, פורמט שעה ותאריך; **פריסה**: אילו אזורים יש במסך הראשי ואיזו גרסה מוצגת בכל אזור
  - **תצוגה**: צבעים, בהירות, גופן
  - **אבטחה**
  - **תוכן אישי**
  - **מערכת**
- **ספירה:** כל אזכור של `userPreferences.<שם>` בקוד, אחרי סינון הערות, או של אותה העדפה דרך אובייקט השכבה (`dataPrefs.place.<שם>`, `logicPrefs.<שם>` וכו', שלב 2a). קובצי השכבות עצמם, שמגדירים את ההעדפות, לא נספרים. מ-2a.3.3, `UserPreferences r1` לכל אחת מ-124 העדפות השכבות והטקסטים ב-`count_prefs` הוא שורת ה-`push_back` בבנאי הרישום, כלומר רישום ולא שימוש.
  - **כתיבה:** השמה (`=`), או קריאה ל-`setValue`/`reset`.
  - **קריאה:** כל שימוש אחר.
  - הסקריפט: [tools/count_prefs.py](../../tools/count_prefs.py), קריאה בלבד. הוראות שימוש בראש הקובץ.
- **צרכנים מול עורכים.** כמעט לכל העדפה יש מצב תפריט ב-`States/` שעורך אותה: קורא את הערך, מציג אותו, וכותב בחזרה ב-`okAction`. לכן הטבלה מפרידה בין שני סוגי משתמשים:
  - **צרכנים**: יחידות שקוראות את הערך כדי להתנהג לפיו.
  - **עורכים/כותבים**: מצבי תפריט שכותבים את ההעדפה, וכל כותב אחר. כותב שאינו תפריט (למשל `Shita`, `Place`) מופיע גם בעמודת הצרכנים אם הוא גם קורא.
  - `Logic::configStates` ([Logic.cpp](../../JC6416/src/Logic.cpp)) קורא העדפות לשתי מטרות:
    - הזנת אובייקט פעיל, למשל `mainScreen.colorUp` או `mainState.intervalTimeSet`. נספר כצרכן.
    - אתחול ערך זמני של עורך, למשל `setColorUpLine.colorTemp`. מסומן `Logic (temp)`.
- **לא נספרים כצרכנים:** `Screens/RegsScreen` ו-`BLE/CoreService` (`PREF_OP_*`). שניהם ניגשים לכל ההעדפות באופן כללי. פירוט בסעיף 6.

## 2. איפה ההעדפות נשמרות היום

| namespace | מי מנהל | תוכן | גישה מ-BLE |
| --- | --- | --- | --- |
| `clk` | `UserPreferences` (`setupPreferences` ב-[UserPreferences.cpp](../../JC6416/src/Preferences/UserPreferences.cpp)) | 125 העדפות: 75 ב-`simplePrefs` (אינדקסים 1-76, ו-3 חור) ו-50 טקסטים ב-`textPrefs` | `PREF_OP_*` לפי מפתח, ו-`TEXT_OP_*` לפי מיקום ב-`textPrefs` |
| `sec` | `AuthManager` (`NVS_NAMESPACE`, `NVS_KEY_PIN`, `NVS_KEY_SLOTS` ב-[Authmanager.cpp](../../JC6416/src/BLE/Authmanager.cpp)) | `pin` (u16), `trusted` (blob של 48B) | `pin` דרך `PREF_OP_WRITE` בלבד, כ-special-case (`handlePref`, המפתח `AUTH_PIN_PREF_KEY`, ב-[CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)). `trusted` רק דרך `AUTH_OP_REGISTER` |
| `sys` | `CoreService` (`loadDeviceName`, `saveDeviceName` ב-[CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)) | `dev_name` (string) | `SYS_OP_SET_NAME` |

עובדות על האינדקסים ב-`clk`:

- לכל 50 ה-`textInputN` יש אינדקס 0. כלפי BLE הן ממוענות לפי המיקום שלהן ב-`textPrefs` (`TEXT_OP_*`, 0-49), ולא לפי האינדקס.
- `textPrefs` לא נמצאים ב-`simplePrefs`, ולכן הם לא מופיעים ב-`PREF_OP_READ` וב-`PREF_OP_SYNC`.
- `PREF_OP_WRITE` מחפש ב-`allPrefs`, ולכן הוא מקבל גם מפתחות `textInputN`.
- `forceWrite` מחזיק את אינדקס 1, ונמצא ב-`simplePrefs`.
- מ-2a.4 ההגדרות (מפתח, טיפוס, אינדקס, ברירת מחדל) הן רשומות ב-flash, ובזמן ריצה רק הערכים ב-RAM (§7). המפתחות, הטיפוסים, האינדקסים, ברירות המחדל וה-namespace לא השתנו.

## 3. סיכום לפי שכבת יעד

| שכבה | כמות | העדפות |
| --- | --- | --- |
| נתונים | 26 | **רשומת מקום (9):** `ofek`, `textPlace`, `nz_lat`, `nz_long`, `nz_height`, `nz_hul`, `timezone`, `shaonKaiz`, `homa` · **שיטה (14):** `shita`, 12 שיטות חישוב (`zriha` … `zeetZom`), `nerot` · **מנהגים (3):** `eda`, `hul`, `shanim` |
| לוגיקה | 27 | `netzClk`, 4×`interval*`, 10 בחירות "מה מוצג" (`dafHayomi` … `showIsraeli`), 12 בחירות "מה ומתי" (`TAZ` … `IORT`) |
| הצגה | 8 | `safa`, `dateFormat`, `clockFormat`, `timeFormat`, `leil` · **פריסה (3):** `middleLine`, `upLine`, `screen` |
| תצוגה | 12 | `brightness`, 7×`color*`, `clockFont`, ועוד 3 צבעים: `colorIvri`, `colorLoazi`, `colorParashaV`. הם בטבלה של כל הדגמים, ובשימוש רק ב-JC6432 |
| מערכת | 1 + 1 | `bt`; ב-`sys`: `dev_name` |
| רישום מרכזי | 1 | `forceWrite` |
| אבטחה | 2 | ב-`sec`: `pin`, `trusted` |
| תוכן אישי | 50 | `textInput1` … `textInput50` |

## 4. טבלה מלאה

**עמודות:**

- **אינדקס**: אינדקס ה-BLE (`getIndex()`).
- **טיפוס**: `PreferenceType` בלי הקידומת `PT_`. `BLOB` הוא `double`.
- **צרכנים**: יחידה ומספר קריאות.
- **עורכים/כותבים**: `rN/wM` = קריאות/כתיבות. `Logic (temp)` = אתחול ערך זמני של עורך.
- שורה עם "—" בשתי עמודות השימוש: ב-JC6416 ההעדפה נגישה רק דרך `RegsScreen` ו-`CoreService`, או בכלל לא.

| אינדקס | מפתח | טיפוס | ברירת מחדל | צרכנים | עורכים / כותבים | שכבה מוצעת | נימוק |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | `forceWrite` | U8 | `0` | — | — | **רישום מרכזי** | נועד לאיפוס מרחוק לברירות מחדל. לא בשימוש (9.1) |
| 2 | `textPlace` | STR | `whaht otwun` | Place 1 | — | **נתונים · מקום** | שם המיקום המותאם, נטען ל-`places[0]` (9.8) |
| 3 | — | — | — | — | — | **חור** | אינדקס לא בשימוש. המפתח `dayIndex` עשוי להישאר ב-`clk` בשעונים כרשומת NVS יתומה, ואף אחד לא קורא אותו (9.2) |
| 4 | `shita` | U8 | `0` | DB 3, DayInfo 5, MinhagState 11 | EdotState r0/w1, Logic r1 (temp), ShitaState r2/w1 | **נתונים · שיטה** | שיטת החישוב (0-9). ראשית: בחירה דורסת 12 שיטות ו-`nerot`. מ-BLE נטמעת מיד דרך `ShitaState::setShita` |
| 5 | `eda` | U8 | `1` | DayInfo 18, MainStateInfoSets 4, MainStateTimeSets 2, Omer 6 | EdotState r6/w6 | **נתונים · מנהגים** | מנהג העדה |
| 6 | `ofek` | U8 | `1` | DayInfo 1, MinhagState 1, Place 2 | Logic r1 (temp), OfekState r5/w2, Place w1 | **נתונים · מקום** | אינדקס העיר ברשימה; 0 = מקום מותאם. ראשית (9.10). מ-BLE נטמעת מיד, גם 0 וגם ≠0 |
| 7 | `nz_lat` | BLOB | `32.07` | Place 2 | OfekState r1/w1 | **נתונים · מקום** | מיקום מותאם |
| 8 | `nz_long` | BLOB | `34.78` | Place 2 | OfekState r1/w1 | **נתונים · מקום** | מיקום מותאם |
| 9 | `nz_height` | I16 | `20` | Place 1 | Logic r1 (temp), OfekState r5/w1 | **נתונים · מקום** | מיקום מותאם |
| 10 | `nz_hul` | U8 | `0` | DayInfo 2, Place 1, Shita 1 | OfekState r2/w4 | **נתונים · מקום** | ישראל/חו"ל של המקום הפעיל (9.11). קובע את כללי שעון הקיץ, וב-`myZmanim::setDefault` את `zmanTalit`. בחירת עיר כותבת אותו (F10) |
| 11 | `timezone` | I8 | `2` | Place 1 | Logic r1 (temp), OfekState r5/w1 | **נתונים · מקום** | אזור זמן (UTC) של המיקום המותאם |
| 12 | `brightness` | U8 | `200` | main 2 | BrightnessState r1/w1, Logic r1 (temp) | **תצוגה** | בהירות הפאנל |
| 13 | `safa` | U8 | `1` | — | — | **הצגה** | שפה. לא בשימוש ב-JC, בשימוש בדגמי BA ואחרים (9.3) |
| 14 | `colorUp` | U8 | `1` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 15 | `colorMiddle` | U8 | `6` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 16 | `colorBottomT` | U8 | `1` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 17 | `colorBottomV` | U8 | `6` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 18 | `colorYom` | U8 | `6` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 19 | `colorParasha` | U8 | `6` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 20 | `colorIluy` | U8 | `6` | Logic 1 | ColorState r1/w1, Logic r1 (temp) | **תצוגה** | צבע |
| 21 | `clockFont` | U8 | `0` | Logic 1, MainState 1 | FontState r2/w1, Logic r1 (temp) | **תצוגה** | גופן השעון |
| 22 | `dateFormat` | U8 | `1` | MainState 1, MainStateTimeSets 1 | Logic r1 (temp), SetClockState r6/w1 | **הצגה** | פורמט תאריך. ערך 8 = בלי תאריך לועזי (9.4) |
| 23 | `clockFormat` | U8 | `24` | MainState 3, Zman 1 | Logic r1 (temp), SetClockState r4/w1 | **הצגה** | 12/24 שעות |
| 24 | `timeFormat` | U8 | `0` | Zman 2 | SetClockState r4/w6 | **הצגה** | אפס מוביל בשעה |
| 25 | `shaonKaiz` | U8 | `1` | DayInfo 3, MainState 1 | Logic r1 (temp), SetClockState r4/w1 | **נתונים · מקום** | שעון קיץ: ידני, או אוטומטי לפי ישראל / ארה"ב (9.5) |
| 26 | `middleLine` | U8 | `1` | — | — | **הצגה · פריסה** | גרסת האזור האמצעי: 1 = שעון בלבד, 2 = שעון + יום ("יום לצד שעון" בתפריט). בשימוש רק ב-JC4020 |
| 27 | `upLine` | U8 | `1` | — | — | **הצגה · פריסה** | גרסת האזור העליון. בשימוש רק ב-JC4020 |
| 28 | `zriha` | U8 | `2` | DayInfo 4 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 29 | `shkiaa` | U8 | `3` | DayInfo 3 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 30 | `rabenuTam` | U8 | `20` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 31 | `zeetCochavim` | U8 | `8` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 32 | `zmanTalit` | U8 | `4` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w11 | **נתונים · שיטה** | שיטת חישוב |
| 33 | `alotHashahar` | U8 | `4` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 34 | `minhaG` | U8 | `3` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 35 | `sozksMga` | U8 | `1` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 36 | `soztMga` | U8 | `1` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 37 | `plagMinha` | U8 | `1` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 38 | `zeetShabat` | U8 | `16` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 39 | `zeetZom` | U8 | `14` | Shita 1 | Logic r1 (temp), MinhagState r1/w1, Shita w10 | **נתונים · שיטה** | שיטת חישוב |
| 40 | `nerot` | U8 | `22` | Place 1, Shita 10 | Logic r1 (temp), MinhagState r5/w1, OfekState r0/w1, Shita w2 | **נתונים · שיטה** | מנהג הדלקת נרות; ערך התחלתי מהשיטה ומהמיקום |
| 41 | `homa` | U8 | `0` | DayInfo 1, MainStateInfoSets 2, Place 1 | OfekState r2/w3 | **נתונים · מקום** | עיר מוקפת חומה. תכונה של המקום, כמו `nz_hul`. בחירת עיר כותבת אותו (9.10) |
| 42 | `hul` | U8 | `0` | DayInfo 2, MainStateInfoSets 1 | EdotState r2/w2, OfekState r0/w4 | **נתונים · מנהגים** | מנהג בני חו"ל. נכתב גם מהמיקום שנבחר, ומהאפליקציה מתוך `nz_hul` |
| 43 | `leil` | U8 | `1` | DayInfo 2 | SetClockState r2/w2 | **הצגה** | אחרי השקיעה: היום הנוכחי או "ליל" של מחר. משפיע רק על התצוגה (9.6) |
| 44 | `netzClk` | U8 | `0` | Logic 1, MainState 1 | Logic r1 (temp), SetTochenInfoState r5/w1 | **לוגיקה** | ספירה לאחור לנץ: מתי ולכמה זמן |
| 45 | `shanim` | U8 | `0` | DayInfo 1 | Logic r1 (temp), MinhagState r5/w1 | **נתונים · מנהגים** | מנהג ספירת השנים לחורבן |
| 46 | `interval` | U16 | `30` | — | — | **לוגיקה** | קצב החלפה. בשימוש רק ב-JC4020 |
| 47 | `intervalIluy` | U16 | `30` | Logic 1 | Logic r1 (temp), SetInputText r3/w1 | **לוגיקה** | קצב החלפה |
| 48 | `intervalTime` | U16 | `30` | Logic 1 | Logic r1 (temp), SetTochenTimeState r4/w1 | **לוגיקה** | קצב החלפה |
| 49 | `intervalInfo` | U16 | `30` | Logic 1 | Logic r1 (temp), SetTochenInfoState r5/w1 | **לוגיקה** | קצב החלפה |
| 50 | `dafHayomi` | U8 | `1` | MainStateInfoSets 3 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 51 | `yerushalmi` | U8 | `0` | MainStateInfoSets 1 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 52 | `yom` | U8 | `0` | — | — | **לוגיקה** | מה מוצג. בשימוש רק ב-JC4020 |
| 53 | `parasha` | U8 | `0` | — | — | **לוגיקה** | מה מוצג. בשימוש רק ב-JC4020 |
| 54 | `haftara` | U8 | `1` | MainStateInfoSets 1 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 55 | `ivri` | U8 | `0` | MainStateInfoSets 1 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 56 | `loazi` | U8 | `0` | MainStateTimeSets 1 | SetTochenTimeState r2/w2 | **לוגיקה** | מה מוצג |
| 57 | `tfila` | U8 | `1` | MainStateInfoSets 20 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 58 | `moadim` | U8 | `1` | MainStateInfoSets 9 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 59 | `showIsraeli` | U8 | `1` | DayInfo 2, MainStateInfoSets 2 | SetTochenInfoState r2/w2 | **לוגיקה** | מה מוצג |
| 60 | `TAZ` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 61 | `SOZT` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 62 | `SOZKS` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 63 | `HMM` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 64 | `PSZ` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 65 | `HAZ` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 66 | `ZSZ1` | U8 | `1` | MainStateTimeSets 3 | SetTochenTimeState r3/w3 | **לוגיקה** | מה מוצג ומתי |
| 67 | `ZSZ` | U8 | `1` | — | — | **לוגיקה** | מה מוצג ומתי. בשימוש רק ב-JC4020 |
| 68 | `ZYS` | U8 | `3` | — | — | **לוגיקה** | מה מוצג ומתי. בשימוש רק ב-JC4020 |
| 69 | `ZPS` | U8 | `0` | — | — | **לוגיקה** | מה מוצג ומתי. בשימוש רק ב-JC4020 |
| 70 | `IPO` | U8 | `2` | — | — | **לוגיקה** | מה מוצג ומתי. בשימוש רק ב-JC4020 |
| 71 | `IORT` | U8 | `2` | MainStateInfoSets 4 | SetTochenInfoState r4/w2 | **לוגיקה** | מה מוצג ומתי |
| 72 | `colorIvri` | U8 | `6` | — | — | **תצוגה** | צבע. בשימוש רק ב-JC6432 |
| 73 | `colorLoazi` | U8 | `1` | — | — | **תצוגה** | צבע. בשימוש רק ב-JC6432 |
| 74 | `colorParashaV` | U8 | `6` | — | — | **תצוגה** | צבע. בשימוש רק ב-JC6432 |
| 75 | `screen` | U8 | `2` | — | — | **הצגה · פריסה** | בחירת הפריסה: חלוקת האזורים של המסך הראשי. בשימוש רק ב-JC6432 (9.7) |
| 76 | `bt` | U8 | `1` | main 1 | ConfigState r4/w1 | **מערכת** | הפעלת BLE |

**תוכן אישי (`textInput1` … `textInput50`):**

| | |
| --- | --- |
| אינדקס | 0 לכולם |
| טיפוס | `STR` |
| ברירת מחדל | `""` |
| צרכנים | `MainState`, דרך `getTextByIndex` (`MainState::UpdateNameSet`, [MainState.cpp](../../JC6416/src/States/MainState.cpp)) |
| עורכים | `BLE/TextCommandHandler` (קריאה וכתיבה דרך `textPrefs[i]`); `SetInputText` (קריאה דרך `getTextByIndex`, מחיקה דרך `handleDeleteText`) |
| שכבה מוצעת | **תוכן אישי** |

**אבטחה ומערכת מחוץ ל-`clk`:**

| העדפה | טיפוס | ברירת מחדל | משתמש | שכבה מוצעת |
| --- | --- | --- | --- | --- |
| `sec/pin` | u16 | 1111 | `AuthManager::init`, `checkPin`, `setPin`, `factoryReset`. `factoryReset` נקרא מ-`SetResetPassState::okAction` ומ-`SetResetAllState::okAction` (`ConfigState.cpp`) | **אבטחה** |
| `sec/trusted` | blob 48B | ריק | `AuthManager` בלבד | **אבטחה** |
| `sys/dev_name` | string | `LedHaarets` | `loadDeviceName` (נקרא מ-`initBluetooth`, `Bluetooth.cpp`), `saveDeviceName` ו-`applyDeviceName` (ב-`CoreService.cpp`) | **מערכת** |

## 5. העדפות שנצרכות ביותר משכבה אחת

שכבות הקוד לפי [dependencies.md](dependencies.md):

- `DB/*` = דומיין
- `Logic` ו-`States/*` = זרימה
- `Screens/*` = תצוגה
- `main` = כניסה
- `BLE/*` = קלט

הטבלה סופרת רק צרכנים. עורכי תפריט לא נכללים.

| העדפה | שכבת יעד | צרכנים לפי שכבת קוד |
| --- | --- | --- |
| `shita` | נתונים | דומיין: `DB` 3, `DayInfo` 5 · זרימה: `MinhagState` 11. תפריטי הזמנים מוצגים לפי השיטה |
| `ofek` | נתונים | דומיין: `Place` 2, `DayInfo` 1 · זרימה: `MinhagState` 1 (`getZriha`, [MinhagState.cpp](../../JC6416/src/States/MinhagState.cpp)) |
| `eda` | נתונים | דומיין: `DayInfo` 18, `Omer` 6 · זרימה: `MainStateInfoSets` 4, `MainStateTimeSets` 2 |
| `homa` | נתונים | דומיין: `DayInfo` 1, `Place` 1 · זרימה: `MainStateInfoSets` 2 |
| `hul` | נתונים | דומיין: `DayInfo` 2 · זרימה: `MainStateInfoSets` 1 |
| `shaonKaiz` | נתונים · מקום | דומיין: `DayInfo` 3 · זרימה: `MainState` 1. ב-02:00 מזיז את ה-RTC (`MainState::setShaonKaitzHoref`, [MainState.cpp](../../JC6416/src/States/MainState.cpp)). לבדיקה מול 9.5 |
| `showIsraeli` | לוגיקה | דומיין: `DayInfo` 2 (`setHaftara`, `setTahanun`) · זרימה: `MainStateInfoSets` 2 |
| `clockFormat` | הצגה | דומיין: `Zman` 1 (`getShaot`) · זרימה: `MainState` 3 |
| `textInput*` | תוכן אישי | קלט: `TextCommandHandler` · זרימה: `MainState`, דרך `getTextByIndex` |

**צרכנים משכבת קוד שלא תואמת את שכבת היעד** (מידע):

- **`clockFormat` ו-`timeFormat` (הצגה)** נקראים בתוך `DB/Zman` (`Zman::getShaot`, `Zman::toString`, `Zman::toStringSec`, [Zman.cpp](../../JC6416/src/DB/Zman.cpp)). `Zman::toString` מחזיר טקסט מעוצב.
- **`leil` (הצגה), `showIsraeli` (לוגיקה), `eda` (בחלקה) ו-`hul` (נתונים)** נקראים בתוך `DayInfo` בפונקציות שמחזירות טקסט להצגה: `getYomOrLeil`, `getDayInWeek`, `getBircatHashanim`.
- **צבעים ו-`clockFont` (תצוגה)** נקראים ב-`Logic::configStates` ומועתקים ל-`mainScreen`. `Screens` לא קורא אותם ישירות.
- **`brightness` (תצוגה)** נקרא ב-`setup` (`main.cpp`).
- **כתיבה מתוך הדומיין:**
  - `Shita::setDefault` כותב 12 העדפות שיטה ואת `nerot` (המימושים בכל מחלקות השיטה, `orHachaim::setDefault` … `pozen::setDefault`, [Shita.cpp](../../JC6416/src/DB/Shita.cpp)). נקרא מ-`ShitaState::setShita` ([ShitaState.cpp](../../JC6416/src/States/ShitaState.cpp)), מהתפריט ומ-BLE.
  - `myZmanim::setDefault` קורא גם את `nz_hul` כדי לבחור את `zmanTalit` ([Shita.cpp](../../JC6416/src/DB/Shita.cpp)). זו הקריאה היחידה של העדפת מקום בתוך `Shita`. בחירת עיר לא מריצה את `setDefault` מחדש (F15, סגור).
  - `initPlace` כותב את `ofek` כשהערך מחוץ לטווח ([Place.cpp](../../JC6416/src/DB/Place.cpp)).
- **כתיבה של שתי העדפות בפעולה אחת:**
  - `SetOfekState::printUpdateSucc` כותב גם את `nerot`, `homa`, `nz_hul` ו-`hul` מנתוני העיר שנבחרה ([OfekState.cpp](../../JC6416/src/States/OfekState.cpp)). נקרא מהתפריט (`SetOfekState::okAction`) ומ-BLE (`ofek` ≠ 0).
  - `EdotChabad` כותב גם את `shita`=4 (`EdotChabad::okAction`, [EdotState.cpp](../../JC6416/src/States/EdotState.cpp)).

## 6. תגובה לשינוי העדפה

### 6.1 מסלולי כתיבה

| מסלול | מה כותב | מה קורה אחרי הכתיבה |
| --- | --- | --- |
| מצב תפריט, `okAction` | `UserPref::setValue` → `save()` | תלוי במצב. פירוט בסעיף 6.3 |
| BLE `PREF_OP_WRITE` (`handlePref`, [CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)) | `UserPreferences::writeBytes`: `decode`, הבדיקות הרשומות (`shita`, `ofek` וארבעת מפתחות צאת הכוכבים), ואז `setValue` → `save()` רק בשינוי (מ-2b.1; לפני כן `fromBytes()` → `save()` תמיד, בלי `setValue`) | רק אינדקס 6 (`ofek`) ואינדקס 4 (`shita`) מפעילים תגובה. פירוט בסעיף 6.4 |
| BLE `PREF_OP_WRITE` עם המפתח `pin` | `authManager.setPin` | שמירה ב-`sec` בלבד |
| BLE `TEXT_OP_WRITE/DELETE/SWAP` ([TextCommandHandler.cpp](../../JC6416/src/BLE/TextCommandHandler.cpp)) | `textPrefs[i] = …` | WRITE: מעדכן את `bl.mainState.numOfValidTexts`, `stateIndexNameSet` ו-`flagNameSetChanged`, כך שהטקסט מוצג מיד. DELETE: מעדכן את `numOfValidTexts`. SWAP: אין תגובה |
| BLE `SYS_OP_SET_NAME` | `sys/dev_name` | `applyDeviceName`: שם ה-GAP וה-scan response מתעדכנים מיד |
| `Shita::setDefault` | 12 שיטות ו-`nerot` | `ShitaState::setShita` ממשיך ל-`configStates`, ל-`updateDayValues` ולהודעת הצלחה |
| `SetOfekState::printUpdateSucc` | `nerot`, `homa`, `nz_hul`, `hul` מנתוני העיר | `initPlace`, `configStates`, `updateDayValues`. פירוט בסעיף 6.3 |
| `initPlace` | `ofek` (fallback) | — |
| `SetResetPrefState` / `SetResetTextState` (`okAction`, [ConfigState.cpp](../../JC6416/src/States/ConfigState.cpp)) | `resetSimplePrefs` / `resetTextPrefs` | `ESP.restart()` |
| `SetResetPassState` (`okAction`, [ConfigState.cpp](../../JC6416/src/States/ConfigState.cpp)) | `authManager.factoryReset` | `ESP.restart()` |
| `SetResetAllState` (`okAction`, [ConfigState.cpp](../../JC6416/src/States/ConfigState.cpp)) | `authManager.factoryReset`, `resetTextPrefs`, `resetSimplePrefs`: כל `sec` וכל `clk`. `sys/dev_name` לא מתאפס | `ESP.restart()` |

### 6.2 איפה ערכים מוחזקים בזיכרון, ומה מרענן אותם

| מנגנון | מה נשמר | העדפות | מה מרענן |
| --- | --- | --- | --- |
| `Logic::configStates` ([Logic.cpp](../../JC6416/src/Logic.cpp)) | `mainScreen.color*`, `mainScreen.clockFont`, `mainState.intervalTimeSet/InfoSet/NameSet`, `mainState.showTimeToNetz`, `numOfValidTexts`, וערכי temp של העורכים | 7 צבעים, `clockFont`, `intervalTime`, `intervalInfo`, `intervalIluy`, `netzClk` | `Logic::init` באתחול; `okAction` של `ColorState`, `MinhagState`, `SetTochen*`, `SetInputText`; `ShitaState::setShita`; `SetCustomState::printUpdateSucc` ו-`SetOfekState::printUpdateSucc` |
| `initPlace` ([Place.cpp](../../JC6416/src/DB/Place.cpp)) | `places[0]` (המיקום המותאם), המצביע `place`, והגלובליים `lonPre/Int/Frac` ו-`latPre/Int/Frac` | `textPlace`, `nz_lat`, `nz_long`, `nz_height`, `timezone`, `nerot`, `homa`, `nz_hul`, `ofek` | `setup` (`main.cpp`); `SetCustomState::printUpdateSucc` ו-`SetOfekState::printUpdateSucc` |
| חישוב יום: `MainState::setNewDay` → `DayInfo::initDay` ×5 ([MainState.cpp](../../JC6416/src/States/MainState.cpp), [DayInfo.cpp](../../JC6416/src/DB/DayInfo.cpp)) | זמני היום (כל הווריאנטים), ערכי `DB`, שעון קיץ (`place->kaiz`), הפטרה, `startTimeToNetz` | `shita`, 12 שיטות, `nerot` (דרך `Shita::calc`), `shanim`, `shaonKaiz`, `nz_hul`, `eda` (הפטרה), `hul`, `netzClk` | חילוף יום (`isNewDay`), או `bl.mainState.updateDayValues = true`, שנבדק ב-`MainState::stateLoop` ([MainState.cpp](../../JC6416/src/States/MainState.cpp)) |
| אתחול בלבד | בהירות הפאנל; הפעלת BLE | `brightness`, `bt` (`setup`, [main.cpp](../../JC6416/src/main.cpp)) | אתחול. בתפריט: `SetBrightnessState` קובע את `display` ישירות, ו-`SetBTState::okAction` קורא ל-`bleApplyState` |
| קריאה בזמן שימוש | — | `clockFormat`, `timeFormat`, `dateFormat`, `leil`, כל בחירות "מה מוצג" ו"מה ומתי" (`dafHayomi` … `IORT`), `eda`, `homa` ו-`hul` ב-`MainStateInfoSets`, `zriha`/`shkiaa` (הבחירה בין הווריאנטים, ב-`DayInfo::getZriha/getShkiaa`) | — |

### 6.3 תגובות בתפריט (`okAction`)

| מצב | תגובה |
| --- | --- |
| `ColorState` (7 צבעים) | `bl.configStates()` |
| `MinhagState` (שיטות, `nerot`) | `bl.configStates()` ו-`updateDayValues = true` |
| `MinhagShanimSetState` (`shanim`) | `dayInfo.setShanimLahurban()` ו-`updateDayValues = true` (`MinhagShanimSetState::okAction`, [MinhagState.cpp](../../JC6416/src/States/MinhagState.cpp)) |
| `ShitaState` | `okAction` כותב את `shita` וקורא ל-`setShita` ([ShitaState.cpp](../../JC6416/src/States/ShitaState.cpp)): `shitot[shita]->setDefault()`, `bl.configStates()`, `updateDayValues = true`, הודעת הצלחה. `setShita` משמש גם את BLE |
| `EdotState` (`eda`, `hul`) | `updateDayValues = true` רק ב-`EdotBneiHul::okAction` ([EdotState.cpp](../../JC6416/src/States/EdotState.cpp)). בשאר: כתיבה בלבד |
| `SetOfekState` (עיר מהרשימה) | `okAction` כותב את `ofek` וקורא ל-`printUpdateSucc` (`SetOfekState::okAction`, `SetOfekState::printUpdateSucc`, [OfekState.cpp](../../JC6416/src/States/OfekState.cpp)): הודעת הצלחה, `nerot` ו-`homa` מהעיר, `initPlace()`, `nz_hul` ו-`hul` מהעיר, `bl.configStates()`, `updateDayValues = true`. אחר כך עובר למסך הראשי. `printUpdateSucc` משמש גם את BLE (`ofek` ≠ 0) |
| `SetCustomState` (מיקום מותאם) | `printUpdateSucc` ([OfekState.cpp](../../JC6416/src/States/OfekState.cpp)): הודעת הצלחה, `initPlace()`, `ofek = 0`, `bl.configStates()`, `updateDayValues = true`. משמש גם את BLE (`ofek` = 0) |
| `SetShaonKaiz` | `setShaonKaitz()` או `setShaonHoref()` (מזיז את ה-RTC), ו-`updateDayValues = true` (`SetShaonKaiz::okAction`, [SetClockState.cpp](../../JC6416/src/States/SetClockState.cpp)) |
| `SetClkFormat`, `SetDateFormat`, `SetTimeFormat`, `MinhagLeilState` | כתיבה בלבד (הערך נקרא בזמן שימוש) |
| `FontState` | `mainScreen.clockFont` ו-`bl.mainState.morphing` נקבעים ישירות |
| `SetTohenInfoStateKezevSet`, `SetTohenTimeStateKezevSet`, `SetTohenIluyStateKezevSet`, `SetTohenBottomLineNetzSet`, `SetTohenIluyStateDelete` | `bl.configStates()` |
| שאר `SetTochen*` (`TAZ` … `IORT`, `dafHayomi` …) | כתיבה בלבד |
| `SetBrightnessState` | `display->setBrightness8` (כבר בזמן הדפדוף) |
| `SetBTState` | `bleApplyState(on/off)` |

### 6.4 אחרי כתיבה מ-BLE (`PREF_OP_WRITE`)

- **בדיקת טווח** (מ-2b.1: בדיקות שהשכבה הבעלים רושמת ברישום, `UserPreferences::registerCheck`; `ofekInRange` ב-[Place.cpp](../../JC6416/src/DB/Place.cpp), `shitaInRange` ו-`zeetInRange` ב-[Shita.cpp](../../JC6416/src/DB/Shita.cpp), נרשמות מ-`setup` לפני `initBluetooth`): `shita` ≥ `numOfShitot` ו-`ofek` ≥ `numOfOfeks` נדחים ב-`CORE_ERR_INVALID_VALUE`, ושום דבר לא נכתב. נבדקים גם `zeetCochavim`, `rabenuTam`, `zeetShabat` ו-`zeetZom`: ערך > `numOfZeetCochavin` (28) נדחה באותו אופן. שאר המפתחות לא נבדקים.
- **כל מפתח:** `userPreferences.writeBytes(*pref, …)` (`handlePref`, `case PREF_OP_WRITE`, [CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)): `decode`, הבדיקות הרשומות, ואז `setValue`, שנשמר ל-NVS רק בשינוי (מ-2b.1; לפני כן `fromBytes()` ואז `save()` תמיד). אורך קצר מדי לטיפוס או דחייה של בדיקה: `CORE_ERR_INVALID_VALUE`. חוץ מ-`ofek` ו-`shita` אין קריאה ל-`configStates`, ל-`initPlace` או ל-`updateDayValues`. מתי השינוי משפיע תלוי במנגנון שמחזיק את ההעדפה (6.2):
  - נקראת בזמן שימוש: בציור הבא.
  - מוחזקת ב-`configStates`: בקריאה הבאה ל-`configStates`.
  - מוחזקת בחישוב היום: בחילוף היום הבא, או בפעם הבאה ש-`updateDayValues` נדלק.
  - `initPlace`, `brightness` ו-`bt`: באתחול הבא.
- **אינדקסים 6 ו-4: הטמעה מיידית.** בשני המקרים BLE מחליף זמנית את `currentState` ל-`bl.updateState` (6.7), קורא לאותה פונקציה שהתפריט מפעיל, ואחר כך מצייר מחדש את המסך הקודם. ההטמעה רצה בתוך ה-callback של הכתיבה, אחרי שליחת ה-OK. **מתוכנן ב-2b** (refactor-log, "2b — plan"): נקודת כתיבה אחת ובדיקות רשומות (2b.1); גזירות, הטמעה בסדר קבוע ומשוב מ-`loop` (2b.2).

  | אינדקס | ערך | פונקציה | מה היא מבצעת, לפי הסדר |
  | --- | --- | --- | --- |
  | 6 `ofek` (`handlePref`, `case PREF_OP_WRITE`, [CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)) | 0 | `SetCustomState::printUpdateSucc` ([OfekState.cpp](../../JC6416/src/States/OfekState.cpp)) | הודעת הצלחה, `globalchangedOfek = false`, `initPlace()`, `ofek = 0`, `bl.configStates()`, `updateDayValues = true` |
  | 6 `ofek` | ≠ 0 | `SetOfekState::printUpdateSucc` ([OfekState.cpp](../../JC6416/src/States/OfekState.cpp)) | הודעת הצלחה, `nerot` ו-`homa` מהעיר, `initPlace()`, `nz_hul` ו-`hul` מהעיר, `bl.configStates()`, `updateDayValues = true` |
  | 4 `shita` (`handlePref`, `case PREF_OP_WRITE`) | כל ערך בטווח | `ShitaState::setShita` ([ShitaState.cpp](../../JC6416/src/States/ShitaState.cpp)) | `shitot[shita]->setDefault()`, `bl.configStates()`, `updateDayValues = true`, הודעת הצלחה |

  `SetCustomState::printUpdateSucc` משמש בתפריט גם אחרי עריכת קו אורך או קו רוחב.

### 6.5 מנגנוני רענון לפי שכבת יעד

| מנגנון רענון | ההעדפות שהוא מחזיק | שכבת יעד |
| --- | --- | --- |
| `Logic::configStates` | 7 צבעים, `clockFont` | תצוגה |
| `Logic::configStates` | `intervalTime`, `intervalInfo`, `intervalIluy`, `netzClk` | לוגיקה |
| `initPlace` | רשומת המקום (`ofek`, `textPlace`, `nz_*`, `timezone`, `homa`), וגם `nerot` | נתונים · מקום |
| חישוב יום (`updateDayValues`) | `shita`, 12 שיטות, `nerot`, `shanim`, `shaonKaiz`, `nz_hul`, `eda`, `hul` | נתונים |
| חישוב יום (`updateDayValues`) | `netzClk` (`startTimeToNetz`) | לוגיקה |
| אתחול בלבד | `brightness` | תצוגה |
| אתחול בלבד | `bt` | מערכת |
| קריאה בזמן שימוש | `clockFormat`, `timeFormat`, `dateFormat`, `leil` | הצגה |
| קריאה בזמן שימוש | בחירות "מה מוצג" ו"מה ומתי" | לוגיקה |

### 6.6 הפער מול הדרישה (9.9)

**הדרישה:** כל שינוי העדפה, מהתפריט או מ-BLE, נטמע מיד בלי `restart` ומציג מסך הצלחה, דרך אותה פונקציית הטמעה שהתפריט כבר משתמש בה. היום זה קורה מ-BLE בשתי ההעדפות הראשיות: `ofek` (0 ו-≠0) ו-`shita`. ההרחבה לשאר ההעדפות היא שלב נפרד ("Later" ב-CLAUDE.md).

**ההעדפות שלא נטמעות מיד אחרי `PREF_OP_WRITE`** (לפי 6.2 ו-6.4), כשהן נכתבות לבד:

- **עד הקריאה הבאה ל-`configStates`:** 7 צבעים, `clockFont`, `intervalTime`, `intervalInfo`, `intervalIluy`, `netzClk`.
- **עד חילוף היום או `updateDayValues`:** 12 שיטות, `nerot`, `shanim`, `shaonKaiz`, `eda`, `hul`, `nz_hul`.
- **עד האתחול הבא:** `brightness`, `bt`, ושדות רשומת המקום כשלא נכתב אחריהם `ofek`.
- **נטמעות מיד, כי הן נקראות בזמן שימוש:** `clockFormat`, `timeFormat`, `dateFormat`, `leil`, ובחירות "מה מוצג" ו"מה ומתי".

העדפה שנכתבת באותו רצף לפני `ofek` או `shita` נטמעת יחד עם ההעדפה הראשית, אם היא מוחזקת ב-`configStates` או בחישוב היום, כי שתי ההטמעות קוראות ל-`configStates` ול-`updateDayValues`. שדות רשומת המקום נטמעים רק לפני `ofek`, כי רק ההטמעה שלו קוראת ל-`initPlace`.

**`brightness` ו-`bt`** נטמעים מ-BLE רק באתחול, גם כשאחריהם נכתבת העדפה ראשית:

- **`brightness`:** אין היום מסלול שמחיל אותו מ-BLE. **מתוכנן:** הטמעה מיידית כחלק ממנגנון הפעולות של הרישום המרכזי (אחרי 2a), יחד עם הוספת כל ההגדרות לאפליקציה. לא כבלוק נוסף ב-`CoreService`.
- **`bt`:** נטמע רק באתחול, וזה רצוי. כיבוי BLE מיידי היה מנתק את האפליקציה שכתבה את הערך לפני שהיא מקבלת תשובה. **חריג מכוון לדרישה ב-9.9.**

**`shita` מהאפליקציה.** לפי קוד האפליקציה (`lib/ui/.../settings_times_tab.dart:299-330`, מחוץ לריפו, כפי שדווח), האפליקציה כותבת `shita`, ממתינה 500ms וקוראת בחזרה 12 שיטות ו-`nerot`, בהנחה שהקושחה הריצה `Shita::setDefault`. `ShitaState::setShita` רץ בתוך ה-callback של הכתיבה, ולכן הקריאה בחזרה מחזירה את ערכי השיטה החדשה.

**העדפות ראשיות ותלויות.** האפליקציה כותבת את ההעדפה הראשית אחרונה:

- **מקום מותאם:** `nz_lat`, `nz_long`, `nz_height`, `timezone`, `shaonKaiz`, `nz_hul`, `hul`, `textPlace`, ובסוף `ofek` = 0 בתור "החל מיקום מותאם" (`lib/providers/ble/ble_location_mixin.dart:79-131`).
- **עיר מהרשימה:** `ofek` ≠ 0. הקושחה כותבת בעצמה `nerot`, `homa`, `nz_hul` ו-`hul` מנתוני העיר. לכן `nz_hul` הוא תמיד של המקום הפעיל (9.11).
- **שיטה:** רק `shita`.

כלומר, הפעלת ההטמעה על ההעדפה הראשית בלבד מספיקה כדי שכתיבות ברצף לא יציגו מסך הצלחה כמה פעמים.

### 6.7 `updateState`: המסך תפוס על ידי הודעה

- **מה זה:** מצב שמשמעותו "המסך תפוס על ידי הודעה" ([UpdateState.cpp](../../JC6416/src/States/UpdateState.cpp)). ל-`UpdateState` אין `stateLoop` ואין מצבי המשך, ולכן כשהוא `currentState`, `loop` לא מצייר והכפתורים לא עושים כלום. זה כולל את האזור שמחוץ למסך ההודעות בשעונים הגדולים מ-JC4020, שבהם מסך ההודעות בגודל של JC4020.
- **מקור:** נבנה למסך ההתקדמות של OTA (`UpdateState::showUpdateStartScreen` ושאר הפונקציות, נקראות מ-`BLE/OtaProtocol`), כדי שמידע מ-`loop` (למשל השניות שמתחלפות) לא יהבהב מעליו.
- **הורחב** למקומות שצריכים להתנתק לרגע מהלולאה של המצב הראשי. התבנית: שמירת `currentState`, החלפה ל-`bl.updateState`, הודעה או פעולה, שחזור ו-`showStateScreen`.
  - `handlePref`: ההטמעה של `ofek` ו-`shita` (6.4).
  - `handleAuth`: הודעת ההתחברות.
  - `handleSys`: `SYS_OP_RESTART`, בלי שחזור.
  - `onDisconnect` ([Bluetooth.cpp](../../JC6416/src/BLE/Bluetooth.cpp)): הודעת הניתוק.
  - `handleTime` מחליף ל-`bl.clockSetTime` ולא ל-`updateState`.
- **ב-2b.2** (refactor-log, "2b — plan"): `updateState` נשאר סביב החישוב שרץ ב-callback של BLE, בלי הודעה. המשוב שמוצג מ-`loop` לא צריך אותו, כי `printMessages` חוסמת את `loop` לכל משך ההודעה.
- **בעתיד:** אם ההודעות יפסיקו לחסום, זה המנגנון (או הכללה שלו לשכבה עליונה שתופסת את המסך) שימנע מ-`loop` לצייר מתחתיהן.

## 7. גישה כללית לכל ההעדפות (לא נספרו)

- **`Screens/RegsScreen`** ([RegsScreen.cpp](../../JC6416/src/Screens/RegsScreen.cpp)): מציג 58 העדפות לפי שם, בדרך כלל 2 אזכורים לכל אחת, וגם את `lonPre` … `latFrac` מ-`Place`.
  - **לא מוצגות בו:** `textPlace`, `nz_hul`, `brightness`, `safa`, `timeFormat`, `sozksMga`, `soztMga`, `plagMinha`, `haftara`, `ivri`, `loazi`, `screen`, `bt`, והטקסטים.
- **`BLE/CoreService`:**
  - `PREF_OP_READ`: חיפוש ב-`simplePrefs` (`handlePref`, `case PREF_OP_READ`, [CoreService.cpp](../../JC6416/src/BLE/CoreService.cpp)).
  - `PREF_OP_WRITE`: חיפוש ב-`allPrefs` (`case PREF_OP_WRITE`).
  - `PREF_OP_SYNC`: מעבר על `simplePrefs` ודיווח האינדקסים שערכם שונה מברירת המחדל (`case PREF_OP_SYNC`).
  - התלויות בהעדפה ספציפית: `getIndex() == 6` ו-`getIndex() == 4`, גם בבדיקת הטווח וגם בהטמעה. בבדיקת הטווח BLE משתמש גם ב-`numOfShitot` (`DB/Shita.h`) וב-`numOfOfeks` (`DB/Place.cpp`), ובהטמעה ב-`bl.setCustomState`, `bl.setOfekState` ו-`bl.shitaState`.
- **`States/ConfigState`:** `resetSimplePrefs` ו-`resetTextPrefs`, כל אחת משני מקומות: `SetResetPrefState`/`SetResetTextState` ו-`SetResetAllState`.
- **`BLE/TextCommandHandler`:** `textPrefs[i]` (7 מקומות).
- **`Preferences/UserPreferences`:** פונקציות הרישום הן `init` ו-`loadAll` (נקראות מתוך `setupPreferences`), `resetSimplePrefs` ו-`resetTextPrefs` (מ-`ConfigState`), ו-`printAll` (debug, לא נקראת בשום מקום).
  - **מ-2a.3.3:** הרישום מחזיק רק את `forceWrite`, את שלוש הרשימות ואת מופע ה-`Preferences` של `clk`. 124 ההעדפות האחרות הן חברים של אובייקטי השכבות (`dataPrefs`, `logicPrefs`, `presentationPrefs`, `displayPrefs`, `systemPrefs`, `userContent`), והבנאי של הרישום ב-`UserPreferences.cpp` מכניס את הכתובות שלהן לרשימות באותו סדר. `userContent.textPrefs` הוא הפניה ל-`userPreferences.textPrefs`.
  - **מ-2a.4:** ההגדרה של כל העדפה (מפתח, ברירת מחדל, טיפוס, אינדקס, מופע `Preferences`, טווח) היא רשומה `constexpr PrefDef<T>` ב-flash, בקובץ השכבה ליד האובייקט, עם `static_assert` שברירת המחדל בטווח (לא לרשומות `PR_OPEN`). ב-RAM כל `UserPref<T>` מחזיק רק vptr, `_meta`, `_isModified` ו-`_value`. שלוש הרשימות הן תצוגות `static constexpr PrefList` (מצביע ומספר) על מערך קבוע אחד ב-flash, `UserPreferences::allPrefsArray` (50 הטקסטים ואחריהם 75 הפשוטות, סדר הטעינה); `userContent.textPrefs` היא תצוגה סטטית על אותו מערך. הטווח הוא טיוטה לתיעוד בלבד, ואף קוד לא קורא אותו (refactor-log, 2a.4).

## 8. הבדלים בין דגמים

### JC4020

- **הגדרות:** 75 ההגדרות זהות ל-JC6416: מפתח, אינדקס, טיפוס וברירת מחדל.
- **אובייקטי השכבות:** זהים ל-JC6416: אותם מבנים, שדות וסדר. אין שדה ייחודי לדגם. מ-2a.3.3 קבצי השכבות והרישום (`.h` ו-`.cpp`) זהים בשלושת הדגמים, חוץ מ-`DisplayPrefs.cpp` ב-JC6432 (ברירות המחדל של `colorYom` ו-`colorParasha`) ושתי שורות קיימות ב-`UserPreferences.h` (הזחה ושורת רווחים). `PrefsViews.cpp` נמחק.
- **שימוש:**
  - `middleLine` ו-`upLine` בשימוש. עורכים: `SetTochenState` ו-`Logic (temp)`. ל-`middleLine` יש צרכן, `MainState` (4). ל-`upLine` אין צרכן.
  - `interval` מחליף את `intervalIluy`, `intervalTime` ו-`intervalInfo`. צרכן: `Logic`, שמזין את `mainState.intervalTimeSet`. עורך: `SetTochenState`. שלוש ההעדפות האחרות לא בשימוש.
  - `yom`, `parasha`, `ZSZ`, `ZYS`, `ZPS` ו-`IPO` בשימוש. צרכנים: `MainStateInfoSets`/`MainStateTimeSets`. עורך: `SetTochenState`.
  - `colorYom`, `colorParasha`, `colorIluy` ו-`loazi` לא בשימוש. אין עורך ואין צרכן.
  - `leil`: גם `MainStateTimeSets` קורא אותו.
  - `dateFormat`: `MainState` 2, ואין קריאה ב-`MainStateTimeSets`.
- **עורכי התוכן:** כל עורכי בחירות התוכן נמצאים בקובץ אחד, `States/SetTochenState.cpp`. אין `SetTochenInfoState`, `SetTochenTimeState` ו-`SetInputText`.
- **טקסטים:** `numOfNameSets = 0`, ולכן `textInput*` לא נגישים דרך `TEXT_OP_*` (F3). הם עדיין ב-`allPrefs`: נטענים, מאופסים, ואפשר לכתוב אותם לפי מפתח דרך `PREF_OP_WRITE`.

### JC6432

- **ברירות מחדל שונות:** `colorYom` = 1, `colorParasha` = 1 (F9). שאר ברירות המחדל זהות בשלושת הדגמים.
- **הטבלה:** אותן העדפות ואותם אינדקסים כמו ב-JC6416 וב-JC4020. ההבדל היחיד בטבלה הוא ברירות המחדל שלמעלה.
- **3 צבעים בשימוש רק כאן:**
  - `colorIvri` (72, ברירת מחדל 6)
  - `colorLoazi` (73, ברירת מחדל 1)
  - `colorParashaV` (74, ברירת מחדל 6)
  - צרכן: `Logic` → `mainScreen`. עורך: `ColorState`. שכבה מוצעת: **תצוגה**. ב-JC6416 וב-JC4020 הם בטבלה, בלי צרכן ובלי עורך.
- **`screen` בשימוש** (שכבה: הצגה · פריסה):
  - צרכנים: `Logic` (`mainScreen.setScreen`), `FontState`, `SetClockState` (`setScreen`), `MainState`.
  - `MainState` גם כותב אותו ב-`upAction`/`downAction` של המסך הראשי. כלומר, לחיצה למעלה או למטה במסך הראשי מחליפה את המסך ושומרת את הבחירה.
- **`dateFormat`:** נקרא גם ב-`MainScreen::Small` (`Screens/MainScreen.cpp`), כלומר שכבת התצוגה קוראת העדפה ישירות. הפריסה משתנה כשאין תאריך לועזי.
- **`loazi`:** יש לו עורך ואין לו צרכן.
- **`MainStateInfoSets`:** מפוצל ל-`…1.cpp` ול-`…2.cpp`. הצרכנים של `tfila`, `moadim`, `IORT` ושאר בחירות התוכן מתחלקים בין שני הקבצים.
- **Layer objects:** the same as JC6416 and JC4020 in all three models; `DisplayPrefs` ends with `colorIvri`, `colorLoazi`, `colorParashaV` (index order); `screen` in `layout`. Since 2a.3.3 the layer objects own the preferences; `DisplayPrefs.cpp` holds the JC6432 defaults of `colorYom` and `colorParasha` (1).

## 9. תשובות והחלטות

1. **`forceWrite`** → **רישום מרכזי.** נועד לאיפוס מרחוק: אם הערך שונה מ-0, `setup` מאפס את ההעדפות לברירות מחדל ומאפס גם את `forceWrite`. בפועל לא בשימוש. נשאר בטבלה לתאימות (`PREF_OP_READ`/`SYNC`).
   - `forceWrite` לא נקרא בשום מקום בקוד, בשלושת הדגמים. הוא נשאר רק בגלל חוזה ה-BLE (אינדקס 1).
2. **אינדקס 3** → **חור.** האינדקסים מפורשים, ולכן אינדקס 3 לא בשימוש ושום אינדקס אחר לא תלוי בו. המפתח `dayIndex` עשוי להישאר ב-`clk` בשעונים כרשומת NVS יתומה, ואף אחד לא קורא אותו.
3. **`safa`** → **הצגה.** לשימוש עתידי כשיתווספו שפות. לא בשימוש ב-JC, אבל בשימוש בדגמי BA ובדגמים אחרים.
4. **`dateFormat`** → **הצגה.** הערך 8 (טקסט ריק) נוסף לבקשת לקוח שלא רצה תאריך לועזי. אחר כך הפריסה הותאמה להופעת התאריך הלועזי, ונבדקת מול הערך 8. כוונה עתידית: להפריד ל"הצג תאריך לועזי" ולפורמט התאריך. שיפור עתידי, לא לביצוע עכשיו.
5. **`shaonKaiz`** → **נתונים, חלק מרשומת המקום.** לא חלק משירות הזמן. ידני: מוסיף שעה לזמנים המחושבים, לא לשעון. אוטומטי: לפי האלגוריתם של ישראל או של ארה"ב.
   - **לבדיקה:** לפי הקוד, `SetShaonKaiz::okAction` מזיז את ה-RTC ([SetClockState.cpp](../../JC6416/src/States/SetClockState.cpp)), וב-02:00 `MainState::setShaonKaitzHoref` מזיז אותו ([MainState.cpp](../../JC6416/src/States/MainState.cpp)). צריך לתעד מה בדיוק קורה ל-RTC ולזמנים בכל מצב.
6. **`leil`** → **הצגה.** אחרי השקיעה אפשר להציג את היום כיום הנוכחי או כ"ליל" של מחר. משפיע רק על התצוגה. שינויים אחרים שקורים אחרי השקיעה הם נושא נפרד.
7. **`screen`** (JC6432) → **הצגה · פריסה.** שתי חלוקות שונות של האזורים במסך הראשי. התוכן מותאם לחלוקה, ולא להפך.
8. **`textPlace`** → **נתונים, חלק מרשומת המקום.** לערים ברשימה יש שמות מובנים. מקום מותאם (`ofek` = 0) מקבל כברירת מחדל את השם "מותאם אישית", ואפשר לשנות אותו מהאפליקציה.
9. **תגובה לכתיבה מ-BLE.** האפליקציה לא שולחת `restart` אחרי כתיבת העדפות; `SYS_OP_RESTART` נשלח רק מכפתור ידני (`home_tab.dart:203`). **דרישה:** כל שינוי נטמע מיד ומציג מסך הצלחה, דרך אותה פונקציה שהתפריט מפעיל. ראה 6.6.
   - **כיוון לתכנון:** BLE לא מכיר את `Logic`. `Preferences` מגדיר לכל העדפה ראשית את הפעולה הלוגית שקורית בשינוי, והפונקציות הקיימות של התפריט מבצעות אותה.
   - **נדחה:** איך `Logic` מקבל את ההודעה, ובאיזה הקשר ריצה ההטמעה מתבצעת.
     - **הוחלט בתכנון 2b** (refactor-log, "2b — plan"): בחלק הראשון החישוב נשאר בהקשר של הכותב (ה-callback של BLE, או `okAction`), ורק מסך ההצלחה של BLE עובר ל-`loop`, אחרי פרק זמן שקט. איחוד ההטמעה לסבב אחד ב-`loop` נדחה (P10).
   - **חריג מכוון:** `bt` נטמע רק באתחול (6.6).
10. **`ofek`** → **נתונים, רשומת מקום, העדפה ראשית.** `ofek` הוא אינדקס העיר ברשימת הערים השמורה. 0 = מקום שהלקוח מגדיר ידנית או מהאפליקציה. מקום מוגדר על ידי קואורדינטות, גובה, ישראל או לא, אזור זמן UTC, והאם שעון קיץ פעיל, והאם העיר מוקפת חומה (`homa`). בחירת עיר כותבת את `homa` מנתוני העיר, כמו את `nz_hul`. האפליקציה כותבת את השדות ובסוף `ofek` = 0 בתור "החל מיקום מותאם" (6.6). גם בחירת עיר מהאפליקציה (`ofek` ≠ 0) נטמעת מיד, דרך הפונקציה של התפריט (6.4).
11. **`nz_hul`** → **נתונים, רשומת מקום.** `nz_hul` הוא ישראל/חו"ל של המקום הפעיל, לא רק של המקום המותאם. בחירת עיר קובעת אותו, גם על חשבון הערך של המקום המותאם. זה מכוון (F10, סגור). אחרי חזרה למקום מותאם הערך נשאר של העיר, עד שכותבים אותו מחדש. האפליקציה כותבת אותו בכל החלה של מקום מותאם (6.6).