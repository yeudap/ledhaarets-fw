# העדפות לפי שכבה — JC6416

סיווג של כל העדפות המשתמש לשכבות היעד, כבסיס להפרדה. בסוף ההפרדה לכל שכבה יהיה מבנה העדפות משלה, ורישום מרכזי אחד יטפל ב-NVS ובאינדקסי ה-BLE.
**מפתחות NVS, ה-namespace ואינדקסי ה-BLE לא משתנים.** המסמך מתאר את המצב הקיים ואת הסיווג. אין בו הצעות מימוש או תיקונים.

הסיווג סופי לפי תשובות בעל הפרויקט (סעיף 9). התשובות מתועדות גם ב-[notes.md](notes.md).

קשור: [dependencies.md](dependencies.md) (שכבות הקוד), [notes.md](notes.md), [../contracts/ble.md](../contracts/ble.md) (חוזה ה-PREF).

## 1. בסיס ושיטה

- **בסיס:** `JC6416`, פרודקשן 7.07 (commit `dcd2cdb`). הבסיס הקודם היה `9c75812` (7.04). בין שני הבסיסים `Preferences/` לא השתנה: אותם מפתחות, אינדקסים, טיפוסים וברירות מחדל. השתנו רק הצרכנים והכותבים של `shita`, `ofek`, `nz_hul`, `nerot`, `hul` ו-12 השיטות, וגם התגובה לכתיבה מ-BLE (סעיף 6). הבדלים בדגמים האחרים מופיעים בסעיף 8.
- **7.08** (commit `d01dd0f`): `dayIndex` נמחק בשלושת הדגמים, ואינדקס 3 הוא חור. נוספה ב-BLE בדיקת טווח לארבעת מפתחות צאת הכוכבים (סעיף 6.4). שאר `Preferences/` לא השתנה. מספרי השורות במסמך עדיין לפי `dcd2cdb`.
- **מקור ההגדרות:** בנאי `UserPreferences` ב-[Preferences/UserPreferences.h:487-846](../../JC6416/src/Preferences/UserPreferences.h#L487-L846).
- **שכבות היעד:**
  - **נתונים**: מיקום, שיטה, מנהגים
  - **לוגיקה**: מה מוצג, מתי ולכמה זמן
  - **הצגה**: שפה, פורמט שעה ותאריך; **פריסה**: אילו אזורים יש במסך הראשי ואיזו גרסה מוצגת בכל אזור
  - **תצוגה**: צבעים, בהירות, גופן
  - **אבטחה**
  - **תוכן אישי**
  - **מערכת**
- **ספירה:** כל אזכור של `userPreferences.<שם>` בקוד, אחרי סינון הערות, או של אותה העדפה דרך ה-view של השכבה (`dataPrefs.place.<שם>`, `logicPrefs.<שם>` וכו', שלב 2a). הגדרות ה-views עצמן לא נספרות.
  - **כתיבה:** השמה (`=`), או קריאה ל-`setValue`/`reset`.
  - **קריאה:** כל שימוש אחר.
  - הסקריפט: [tools/count_prefs.py](../../tools/count_prefs.py), קריאה בלבד. הוראות שימוש בראש הקובץ.
- **צרכנים מול עורכים.** כמעט לכל העדפה יש מצב תפריט ב-`States/` שעורך אותה: קורא את הערך, מציג אותו, וכותב בחזרה ב-`okAction`. לכן הטבלה מפרידה בין שני סוגי משתמשים:
  - **צרכנים**: יחידות שקוראות את הערך כדי להתנהג לפיו.
  - **עורכים/כותבים**: מצבי תפריט שכותבים את ההעדפה, וכל כותב אחר. כותב שאינו תפריט (למשל `Shita`, `Place`) מופיע גם בעמודת הצרכנים אם הוא גם קורא.
  - `Logic::configStates` ([Logic.cpp:38-97](../../JC6416/src/Logic.cpp#L38-L97)) קורא העדפות לשתי מטרות:
    - הזנת אובייקט פעיל, למשל `mainScreen.colorUp` או `mainState.intervalTimeSet`. נספר כצרכן.
    - אתחול ערך זמני של עורך, למשל `setColorUpLine.colorTemp`. מסומן `Logic (temp)`.
- **לא נספרים כצרכנים:** `Screens/RegsScreen` ו-`BLE/CoreService` (`PREF_OP_*`). שניהם ניגשים לכל ההעדפות באופן כללי. פירוט בסעיף 6.

## 2. איפה ההעדפות נשמרות היום

| namespace | מי מנהל | תוכן | גישה מ-BLE |
| --- | --- | --- | --- |
| `clk` | `UserPreferences` ([UserPreferences.cpp:9](../../JC6416/src/Preferences/UserPreferences.cpp#L9)) | 122 העדפות: 72 ב-`simplePrefs` (אינדקסים 1-73, ו-3 חור מ-7.08) ו-50 טקסטים ב-`textPrefs` | `PREF_OP_*` לפי מפתח, ו-`TEXT_OP_*` לפי מיקום ב-`textPrefs` |
| `sec` | `AuthManager` ([Authmanager.cpp:11-13](../../JC6416/src/BLE/Authmanager.cpp#L11-L13)) | `pin` (u16), `trusted` (blob של 48B) | `pin` דרך `PREF_OP_WRITE` בלבד, כ-special-case ([CoreService.cpp:616-640](../../JC6416/src/BLE/CoreService.cpp#L616-L640)). `trusted` רק דרך `AUTH_OP_REGISTER` |
| `sys` | `CoreService` ([CoreService.cpp:731-749](../../JC6416/src/BLE/CoreService.cpp#L731-L749)) | `dev_name` (string) | `SYS_OP_SET_NAME` |

עובדות על האינדקסים ב-`clk`:

- לכל 50 ה-`textInputN` יש אינדקס 0. כלפי BLE הן ממוענות לפי המיקום שלהן ב-`textPrefs` (`TEXT_OP_*`, 0-49), ולא לפי האינדקס.
- `textPrefs` לא נמצאים ב-`simplePrefs`, ולכן הם לא מופיעים ב-`PREF_OP_READ` וב-`PREF_OP_SYNC`.
- `PREF_OP_WRITE` מחפש ב-`allPrefs`, ולכן הוא מקבל גם מפתחות `textInputN`.
- `forceWrite` מחזיק את אינדקס 1, ונמצא ב-`simplePrefs`.

## 3. סיכום לפי שכבת יעד

| שכבה | כמות | העדפות |
| --- | --- | --- |
| נתונים | 26 | **רשומת מקום (9):** `ofek`, `textPlace`, `nz_lat`, `nz_long`, `nz_height`, `nz_hul`, `timezone`, `shaonKaiz`, `homa` · **שיטה (14):** `shita`, 12 שיטות חישוב (`zriha` … `zeetZom`), `nerot` · **מנהגים (3):** `eda`, `hul`, `shanim` |
| לוגיקה | 27 | `netzClk`, 4×`interval*`, 10 בחירות "מה מוצג" (`dafHayomi` … `showIsraeli`), 12 בחירות "מה ומתי" (`TAZ` … `IORT`) |
| הצגה | 8 | `safa`, `dateFormat`, `clockFormat`, `timeFormat`, `leil` · **פריסה (3):** `middleLine`, `upLine`, `screen` |
| תצוגה | 9 | `brightness`, 7×`color*`, `clockFont`. ב-JC6432 עוד 3 צבעים |
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
| 3 | — | — | — | — | — | **נמחק ב-7.08** | חור. היה `dayIndex` (U8, ברירת מחדל `1`): נכתב כל יום ב-`MainState::setNewDay` ולא נקרא. נמחק בפרודקשן 7.08 (F16), והאינדקס לא הוקצה מחדש (9.2) |
| 4 | `shita` | U8 | `0` | DB 3, DayInfo 5, MinhagState 11 | EdotState r0/w1, Logic r1 (temp), ShitaState r2/w1 | **נתונים · שיטה** | שיטת החישוב (0-9 מ-7.05). ראשית: בחירה דורסת 12 שיטות ו-`nerot`. מ-BLE נטמעת מיד דרך `ShitaState::setShita` (7.07) |
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
| 43 | `leil` | U8 | `0` | DayInfo 2 | SetClockState r2/w2 | **הצגה** | אחרי השקיעה: היום הנוכחי או "ליל" של מחר. משפיע רק על התצוגה (9.6) |
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
| 72 | `screen` | U8 | `1` | — | — | **הצגה · פריסה** | בחירת הפריסה: חלוקת האזורים של המסך הראשי. בשימוש רק ב-JC6432 (9.7) |
| 73 | `bt` | U8 | `1` | main 1 | ConfigState r4/w1 | **מערכת** | הפעלת BLE |

**תוכן אישי (`textInput1` … `textInput50`):**

| | |
| --- | --- |
| אינדקס | 0 לכולם |
| טיפוס | `STR` |
| ברירת מחדל | `""` |
| צרכנים | `MainState`, דרך `getTextByIndex` ([MainState.cpp:142](../../JC6416/src/States/MainState.cpp#L142)) |
| עורכים | `BLE/TextCommandHandler` (קריאה וכתיבה דרך `textPrefs[i]`); `SetInputText` (קריאה דרך `getTextByIndex`, מחיקה דרך `handleDeleteText`) |
| שכבה מוצעת | **תוכן אישי** |

**אבטחה ומערכת מחוץ ל-`clk`:**

| העדפה | טיפוס | ברירת מחדל | משתמש | שכבה מוצעת |
| --- | --- | --- | --- | --- |
| `sec/pin` | u16 | 1111 | `AuthManager::init`, `checkPin`, `setPin`, `factoryReset`. `factoryReset` נקרא מ-`ConfigState.cpp:120` (`SetResetPassState`) ומ-`:140` (`SetResetAllState`, מ-7.06) | **אבטחה** |
| `sec/trusted` | blob 48B | ריק | `AuthManager` בלבד | **אבטחה** |
| `sys/dev_name` | string | `LedHaarets` | `loadDeviceName` (ב-`Bluetooth.cpp:82`), `saveDeviceName` ו-`applyDeviceName` (ב-`CoreService.cpp`) | **מערכת** |

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
| `ofek` | נתונים | דומיין: `Place` 2, `DayInfo` 1 · זרימה: `MinhagState` 1 (`getZriha`, [MinhagState.cpp:149](../../JC6416/src/States/MinhagState.cpp#L149)) |
| `eda` | נתונים | דומיין: `DayInfo` 18, `Omer` 6 · זרימה: `MainStateInfoSets` 4, `MainStateTimeSets` 2 |
| `homa` | נתונים | דומיין: `DayInfo` 1, `Place` 1 · זרימה: `MainStateInfoSets` 2 |
| `hul` | נתונים | דומיין: `DayInfo` 2 · זרימה: `MainStateInfoSets` 1 |
| `shaonKaiz` | נתונים · מקום | דומיין: `DayInfo` 3 · זרימה: `MainState` 1. ב-02:00 מזיז את ה-RTC ([MainState.cpp:238-246](../../JC6416/src/States/MainState.cpp#L238-L246)). לבדיקה מול 9.5 |
| `showIsraeli` | לוגיקה | דומיין: `DayInfo` 2 (`setHaftara`, `setTahanun`) · זרימה: `MainStateInfoSets` 2 |
| `clockFormat` | הצגה | דומיין: `Zman` 1 (`getShaot`) · זרימה: `MainState` 3 |
| `textInput*` | תוכן אישי | קלט: `TextCommandHandler` · זרימה: `MainState`, דרך `getTextByIndex` |

**צרכנים משכבת קוד שלא תואמת את שכבת היעד** (מידע):

- **`clockFormat` ו-`timeFormat` (הצגה)** נקראים בתוך `DB/Zman` ([Zman.cpp:14-44](../../JC6416/src/DB/Zman.cpp#L14-L44)). `Zman::toString` מחזיר טקסט מעוצב.
- **`leil` (הצגה), `showIsraeli` (לוגיקה), `eda` (בחלקה) ו-`hul` (נתונים)** נקראים בתוך `DayInfo` בפונקציות שמחזירות טקסט להצגה: `getYomOrLeil`, `getDayInWeek`, `getBircatHashanim`.
- **צבעים ו-`clockFont` (תצוגה)** נקראים ב-`Logic::configStates` ומועתקים ל-`mainScreen`. `Screens` לא קורא אותם ישירות.
- **`brightness` (תצוגה)** נקרא ב-`main.cpp:29-40`.
- **כתיבה מתוך הדומיין:**
  - `Shita::setDefault` כותב 12 העדפות שיטה ואת `nerot` ([Shita.cpp:61-474](../../JC6416/src/DB/Shita.cpp#L61-L474)). נקרא מ-`ShitaState::setShita` ([ShitaState.cpp:38](../../JC6416/src/States/ShitaState.cpp#L38)), מהתפריט ומ-BLE.
  - `myZmanim::setDefault` (מ-7.05) קורא גם את `nz_hul` כדי לבחור את `zmanTalit` ([Shita.cpp:421-422](../../JC6416/src/DB/Shita.cpp#L421-L422)). זו הקריאה היחידה של העדפת מקום בתוך `Shita`. בחירת עיר לא מריצה את `setDefault` מחדש (F15, סגור).
  - `initPlace` כותב את `ofek` כשהערך מחוץ לטווח ([Place.cpp:117-122](../../JC6416/src/DB/Place.cpp#L117-L122)).
- **כתיבה של שתי העדפות בפעולה אחת:**
  - `SetOfekState::printUpdateSucc` כותב גם את `nerot`, `homa`, `nz_hul` ו-`hul` מנתוני העיר שנבחרה ([OfekState.cpp:510-519](../../JC6416/src/States/OfekState.cpp#L510-L519)). נקרא מהתפריט (`SetOfekState::okAction`) ומ-BLE (`ofek` ≠ 0).
  - `EdotChabad` כותב גם את `shita`=4 ([EdotState.cpp:113-114](../../JC6416/src/States/EdotState.cpp#L113-L114)).

## 6. תגובה לשינוי העדפה

### 6.1 מסלולי כתיבה

| מסלול | מה כותב | מה קורה אחרי הכתיבה |
| --- | --- | --- |
| מצב תפריט, `okAction` | `UserPref::setValue` → `save()` | תלוי במצב. פירוט בסעיף 6.3 |
| BLE `PREF_OP_WRITE` ([CoreService.cpp:588-694](../../JC6416/src/BLE/CoreService.cpp#L588-L694)) | בדיקת טווח ל-`shita` ול-`ofek` (7.07) ולארבעת מפתחות צאת הכוכבים (7.08), ואז `fromBytes()` → `save()`, ישירות, בלי `setValue` | רק אינדקס 6 (`ofek`) ואינדקס 4 (`shita`) מפעילים תגובה. פירוט בסעיף 6.4 |
| BLE `PREF_OP_WRITE` עם המפתח `pin` | `authManager.setPin` | שמירה ב-`sec` בלבד |
| BLE `TEXT_OP_WRITE/DELETE/SWAP` ([TextCommandHandler.cpp](../../JC6416/src/BLE/TextCommandHandler.cpp)) | `textPrefs[i] = …` | WRITE: מעדכן את `bl.mainState.numOfValidTexts`, `stateIndexNameSet` ו-`flagNameSetChanged`, כך שהטקסט מוצג מיד. DELETE: מעדכן את `numOfValidTexts`. SWAP: אין תגובה |
| BLE `SYS_OP_SET_NAME` | `sys/dev_name` | `applyDeviceName`: שם ה-GAP וה-scan response מתעדכנים מיד |
| `Shita::setDefault` | 12 שיטות ו-`nerot` | `ShitaState::setShita` ממשיך ל-`configStates`, ל-`updateDayValues` ולהודעת הצלחה |
| `SetOfekState::printUpdateSucc` | `nerot`, `homa`, `nz_hul`, `hul` מנתוני העיר | `initPlace`, `configStates`, `updateDayValues`. פירוט בסעיף 6.3 |
| `initPlace` | `ofek` (fallback) | — |
| `SetResetPrefState` / `SetResetTextState` ([ConfigState.cpp:76-105](../../JC6416/src/States/ConfigState.cpp#L76-L105)) | `resetSimplePrefs` / `resetTextPrefs` | `ESP.restart()` |
| `SetResetPassState` ([ConfigState.cpp:116-125](../../JC6416/src/States/ConfigState.cpp#L116-L125)) | `authManager.factoryReset` | `ESP.restart()` |
| `SetResetAllState` ([ConfigState.cpp:136-147](../../JC6416/src/States/ConfigState.cpp#L136-L147)), מ-7.06 | `authManager.factoryReset`, `resetTextPrefs`, `resetSimplePrefs`: כל `sec` וכל `clk`. `sys/dev_name` לא מתאפס | `ESP.restart()` |

### 6.2 איפה ערכים מוחזקים בזיכרון, ומה מרענן אותם

| מנגנון | מה נשמר | העדפות | מה מרענן |
| --- | --- | --- | --- |
| `Logic::configStates` ([Logic.cpp:38-97](../../JC6416/src/Logic.cpp#L38-L97)) | `mainScreen.color*`, `mainScreen.clockFont`, `mainState.intervalTimeSet/InfoSet/NameSet`, `mainState.showTimeToNetz`, `numOfValidTexts`, וערכי temp של העורכים | 7 צבעים, `clockFont`, `intervalTime`, `intervalInfo`, `intervalIluy`, `netzClk` | `Logic::init` באתחול; `okAction` של `ColorState`, `MinhagState`, `SetTochen*`, `SetInputText`; `ShitaState::setShita`; `SetCustomState::printUpdateSucc` ו-`SetOfekState::printUpdateSucc` |
| `initPlace` ([Place.cpp:112-147](../../JC6416/src/DB/Place.cpp#L112-L147)) | `places[0]` (המיקום המותאם), המצביע `place`, והגלובליים `lonPre/Int/Frac` ו-`latPre/Int/Frac` | `textPlace`, `nz_lat`, `nz_long`, `nz_height`, `timezone`, `nerot`, `homa`, `nz_hul`, `ofek` | `main.cpp:43`; `SetCustomState::printUpdateSucc` ו-`SetOfekState::printUpdateSucc` |
| חישוב יום: `MainState::setNewDay` → `DayInfo::initDay` ×5 ([MainState.cpp:257-286](../../JC6416/src/States/MainState.cpp#L257-L286), [DayInfo.cpp:7-25](../../JC6416/src/DB/DayInfo.cpp#L7-L25)) | זמני היום (כל הווריאנטים), ערכי `DB`, שעון קיץ (`place->kaiz`), הפטרה, `startTimeToNetz` | `shita`, 12 שיטות, `nerot` (דרך `Shita::calc`), `shanim`, `shaonKaiz`, `nz_hul`, `eda` (הפטרה), `hul`, `netzClk` | חילוף יום (`isNewDay`), או `bl.mainState.updateDayValues = true`, שנבדק ב-`MainState::stateLoop` ([MainState.cpp:44](../../JC6416/src/States/MainState.cpp#L44)) |
| אתחול בלבד | בהירות הפאנל; הפעלת BLE | `brightness` ([main.cpp:29-40](../../JC6416/src/main.cpp#L29-L40)), `bt` ([main.cpp:60](../../JC6416/src/main.cpp#L60)) | אתחול. בתפריט: `SetBrightnessState` קובע את `display` ישירות, ו-`SetBTState::okAction` קורא ל-`bleApplyState` |
| קריאה בזמן שימוש | — | `clockFormat`, `timeFormat`, `dateFormat`, `leil`, כל בחירות "מה מוצג" ו"מה ומתי" (`dafHayomi` … `IORT`), `eda`, `homa` ו-`hul` ב-`MainStateInfoSets`, `zriha`/`shkiaa` (הבחירה בין הווריאנטים, ב-`DayInfo::getZriha/getShkiaa`) | — |

### 6.3 תגובות בתפריט (`okAction`)

| מצב | תגובה |
| --- | --- |
| `ColorState` (7 צבעים) | `bl.configStates()` |
| `MinhagState` (שיטות, `nerot`) | `bl.configStates()` ו-`updateDayValues = true` |
| `MinhagShanimSetState` (`shanim`) | `dayInfo.setShanimLahurban()` ו-`updateDayValues = true` ([MinhagState.cpp:292-297](../../JC6416/src/States/MinhagState.cpp#L292-L297)) |
| `ShitaState` | `okAction` כותב את `shita` וקורא ל-`setShita` ([ShitaState.cpp:31-42](../../JC6416/src/States/ShitaState.cpp#L31-L42)): `shitot[shita]->setDefault()`, `bl.configStates()`, `updateDayValues = true`, הודעת הצלחה. `setShita` משמש גם את BLE (7.07) |
| `EdotState` (`eda`, `hul`) | `updateDayValues = true` רק ב-`EdotBneiHul` ([EdotState.cpp:131](../../JC6416/src/States/EdotState.cpp#L131)). בשאר: כתיבה בלבד |
| `SetOfekState` (עיר מהרשימה) | `okAction` כותב את `ofek` וקורא ל-`printUpdateSucc` ([OfekState.cpp:67-71](../../JC6416/src/States/OfekState.cpp#L67-L71), [:510-519](../../JC6416/src/States/OfekState.cpp#L510-L519)): הודעת הצלחה, `nerot` ו-`homa` מהעיר, `initPlace()`, `nz_hul` ו-`hul` מהעיר, `bl.configStates()`, `updateDayValues = true`. אחר כך עובר למסך הראשי (מ-7.06). `printUpdateSucc` משמש גם את BLE (`ofek` ≠ 0, מ-7.05) |
| `SetCustomState` (מיקום מותאם) | `printUpdateSucc` ([OfekState.cpp:501-509](../../JC6416/src/States/OfekState.cpp#L501-L509)): הודעת הצלחה, `initPlace()`, `ofek = 0`, `bl.configStates()`, `updateDayValues = true`. משמש גם את BLE (`ofek` = 0) |
| `SetShaonKaiz` | `setShaonKaitz()` או `setShaonHoref()` (מזיז את ה-RTC), ו-`updateDayValues = true` ([SetClockState.cpp:173-180](../../JC6416/src/States/SetClockState.cpp#L173-L180)) |
| `SetClkFormat`, `SetDateFormat`, `SetTimeFormat`, `MinhagLeilState` | כתיבה בלבד (הערך נקרא בזמן שימוש) |
| `FontState` | `mainScreen.clockFont` ו-`bl.mainState.morphing` נקבעים ישירות |
| `SetTohenInfoStateKezevSet`, `SetTohenTimeStateKezevSet`, `SetTohenIluyStateKezevSet`, `SetTohenBottomLineNetzSet`, `SetTohenIluyStateDelete` | `bl.configStates()` |
| שאר `SetTochen*` (`TAZ` … `IORT`, `dafHayomi` …) | כתיבה בלבד |
| `SetBrightnessState` | `display->setBrightness8` (כבר בזמן הדפדוף) |
| `SetBTState` | `bleApplyState(on/off)` |

### 6.4 אחרי כתיבה מ-BLE (`PREF_OP_WRITE`)

- **בדיקת טווח (מ-7.07)** ([CoreService.cpp:653-659](../../JC6416/src/BLE/CoreService.cpp#L653-L659)): לפני `fromBytes`, `shita` ≥ `numOfShitot` ו-`ofek` ≥ `numOfOfeks` נדחים ב-`CORE_ERR_INVALID_VALUE`, ושום דבר לא נכתב. מ-7.08 נבדקים גם `zeetCochavim`, `rabenuTam`, `zeetShabat` ו-`zeetZom`, לפי כתובת ההעדפה ולא לפי אינדקס: ערך > `numOfZeetCochavin` (28) נדחה באותו אופן (F11). שאר המפתחות לא נבדקים.
- **כל מפתח:** `pref->fromBytes()` ואז `pref->save()`. הערך בזיכרון וב-NVS מתעדכן. חוץ מ-`ofek` ו-`shita` אין קריאה ל-`configStates`, ל-`initPlace` או ל-`updateDayValues`. מתי השינוי משפיע תלוי במנגנון שמחזיק את ההעדפה (6.2):
  - נקראת בזמן שימוש: בציור הבא.
  - מוחזקת ב-`configStates`: בקריאה הבאה ל-`configStates`.
  - מוחזקת בחישוב היום: בחילוף היום הבא, או בפעם הבאה ש-`updateDayValues` נדלק.
  - `initPlace`, `brightness` ו-`bt`: באתחול הבא.
- **אינדקסים 6 ו-4: הטמעה מיידית.** בשני המקרים BLE מחליף זמנית את `currentState` ל-`bl.updateState`, קורא לאותה פונקציה שהתפריט מפעיל, ואחר כך מצייר מחדש את המסך הקודם. ההטמעה רצה בתוך ה-callback של הכתיבה, אחרי שליחת ה-OK.

  | אינדקס | ערך | פונקציה | מה היא מבצעת, לפי הסדר | מאז |
  | --- | --- | --- | --- | --- |
  | 6 `ofek` ([CoreService.cpp:669-678](../../JC6416/src/BLE/CoreService.cpp#L669-L678)) | 0 | `SetCustomState::printUpdateSucc` ([OfekState.cpp:501-509](../../JC6416/src/States/OfekState.cpp#L501-L509)) | הודעת הצלחה, `globalchangedOfek = false`, `initPlace()`, `ofek = 0`, `bl.configStates()`, `updateDayValues = true` | ≤ 7.03 |
  | 6 `ofek` | ≠ 0 | `SetOfekState::printUpdateSucc` ([OfekState.cpp:510-519](../../JC6416/src/States/OfekState.cpp#L510-L519)) | הודעת הצלחה, `nerot` ו-`homa` מהעיר, `initPlace()`, `nz_hul` ו-`hul` מהעיר, `bl.configStates()`, `updateDayValues = true` | 7.05 |
  | 4 `shita` ([CoreService.cpp:679-687](../../JC6416/src/BLE/CoreService.cpp#L679-L687)) | כל ערך בטווח | `ShitaState::setShita` ([ShitaState.cpp:36-42](../../JC6416/src/States/ShitaState.cpp#L36-L42)) | `shitot[shita]->setDefault()`, `bl.configStates()`, `updateDayValues = true`, הודעת הצלחה | 7.07 |

  `SetCustomState::printUpdateSucc` משמש בתפריט גם אחרי עריכת קו אורך או קו רוחב. עד 7.04 כל כתיבה של `ofek` מ-BLE עברה דרכו, ולכן עיר שנבחרה מהאפליקציה התאפסה ל-0.

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

**הדרישה:** כל שינוי העדפה, מהתפריט או מ-BLE, נטמע מיד בלי `restart` ומציג מסך הצלחה, דרך אותה פונקציית הטמעה שהתפריט כבר משתמש בה. היום (7.07) זה קורה מ-BLE בשתי ההעדפות הראשיות: `ofek` (0 ו-≠0) ו-`shita`. ההרחבה לשאר ההעדפות היא שלב נפרד ("Later" ב-CLAUDE.md).

**ההעדפות שלא נטמעות מיד אחרי `PREF_OP_WRITE`** (לפי 6.2 ו-6.4), כשהן נכתבות לבד:

- **עד הקריאה הבאה ל-`configStates`:** 7 צבעים, `clockFont`, `intervalTime`, `intervalInfo`, `intervalIluy`, `netzClk`.
- **עד חילוף היום או `updateDayValues`:** 12 שיטות, `nerot`, `shanim`, `shaonKaiz`, `eda`, `hul`, `nz_hul`.
- **עד האתחול הבא:** `brightness`, `bt`, ושדות רשומת המקום כשלא נכתב אחריהם `ofek`.
- **נטמעות מיד, כי הן נקראות בזמן שימוש:** `clockFormat`, `timeFormat`, `dateFormat`, `leil`, ובחירות "מה מוצג" ו"מה ומתי".

העדפה שנכתבת באותו רצף לפני `ofek` או `shita` נטמעת יחד עם ההעדפה הראשית, אם היא מוחזקת ב-`configStates` או בחישוב היום, כי שתי ההטמעות קוראות ל-`configStates` ול-`updateDayValues`. שדות רשומת המקום נטמעים רק לפני `ofek`, כי רק ההטמעה שלו קוראת ל-`initPlace`.

**`brightness` ו-`bt`** נטמעים מ-BLE רק באתחול, גם כשאחריהם נכתבת העדפה ראשית:

- **`brightness`:** אין היום מסלול שמחיל אותו מ-BLE. **מתוכנן:** הטמעה מיידית כחלק ממנגנון הפעולות של הרישום המרכזי (אחרי 2a), יחד עם הוספת כל ההגדרות לאפליקציה. לא כבלוק נוסף ב-`CoreService`.
- **`bt`:** נטמע רק באתחול, וזה רצוי. כיבוי BLE מיידי היה מנתק את האפליקציה שכתבה את הערך לפני שהיא מקבלת תשובה. **חריג מכוון לדרישה ב-9.9.**

**`shita` מהאפליקציה.** לפי קוד האפליקציה (`lib/ui/.../settings_times_tab.dart:299-330`, מחוץ לריפו, כפי שדווח), האפליקציה כותבת `shita`, ממתינה 500ms וקוראת בחזרה 12 שיטות ו-`nerot`, בהנחה שהקושחה הריצה `Shita::setDefault`. עד 7.06 הקושחה לא הריצה אותו, והאפליקציה קראה בחזרה את הערכים הקודמים. מ-7.07 `ShitaState::setShita` רץ בתוך ה-callback של הכתיבה, ולכן הקריאה בחזרה מחזירה את ערכי השיטה החדשה. הפער נסגר.

**העדפות ראשיות ותלויות.** האפליקציה כותבת את ההעדפה הראשית אחרונה:

- **מקום מותאם:** `nz_lat`, `nz_long`, `nz_height`, `timezone`, `shaonKaiz`, `nz_hul`, `hul`, `textPlace`, ובסוף `ofek` = 0 בתור "החל מיקום מותאם" (`lib/providers/ble/ble_location_mixin.dart:79-131`).
- **עיר מהרשימה:** `ofek` ≠ 0. הקושחה כותבת בעצמה `nerot`, `homa`, `nz_hul` ו-`hul` מנתוני העיר (מ-7.05). לכן `nz_hul` הוא תמיד של המקום הפעיל (9.11).
- **שיטה:** רק `shita`.

כלומר, הפעלת ההטמעה על ההעדפה הראשית בלבד מספיקה כדי שכתיבות ברצף לא יציגו מסך הצלחה כמה פעמים.

## 7. גישה כללית לכל ההעדפות (לא נספרו)

- **`Screens/RegsScreen`** ([RegsScreen.cpp](../../JC6416/src/Screens/RegsScreen.cpp)): מציג 58 העדפות לפי שם, בדרך כלל 2 אזכורים לכל אחת, וגם את `lonPre` … `latFrac` מ-`Place`.
  - **לא מוצגות בו:** `textPlace`, `nz_hul`, `brightness`, `safa`, `timeFormat`, `sozksMga`, `soztMga`, `plagMinha`, `haftara`, `ivri`, `loazi`, `screen`, `bt`, והטקסטים.
- **`BLE/CoreService`:**
  - `PREF_OP_READ`: חיפוש ב-`simplePrefs` ([CoreService.cpp:549-583](../../JC6416/src/BLE/CoreService.cpp#L549-L583)).
  - `PREF_OP_WRITE`: חיפוש ב-`allPrefs` ([:588-694](../../JC6416/src/BLE/CoreService.cpp#L588-L694)).
  - `PREF_OP_SYNC`: מעבר על `simplePrefs` ודיווח האינדקסים שערכם שונה מברירת המחדל ([:700-720](../../JC6416/src/BLE/CoreService.cpp#L700-L720)).
  - התלויות בהעדפה ספציפית: `getIndex() == 6` ו-`getIndex() == 4`, גם בבדיקת הטווח וגם בהטמעה. בבדיקת הטווח BLE משתמש גם ב-`numOfShitot` (`DB/Shita.h`) וב-`numOfOfeks` (`DB/Place.cpp`), ובהטמעה ב-`bl.setCustomState`, `bl.setOfekState` ו-`bl.shitaState`.
- **`States/ConfigState`:** `resetSimplePrefs` ו-`resetTextPrefs`, כל אחת משני מקומות: `SetResetPrefState`/`SetResetTextState` ו-`SetResetAllState` (מ-7.06).
- **`BLE/TextCommandHandler`:** `textPrefs[i]` (7 מקומות).
- **`Preferences/UserPreferences`:** `loadAll` (נקרא מתוך `setupPreferences`), `resetSimplePrefs` ו-`resetTextPrefs` (מ-`ConfigState`). הפונקציות `saveAll`, `resetAll`, `printAll`, `getValueByKey`, `setValueByKey` ו-`getPref` לא נקראו בשום מקום, ונמחקו ב-2a.3.1.

## 8. הבדלים בין דגמים

### JC4020

- **הגדרות:** 72 ההגדרות זהות ל-JC6416: מפתח, אינדקס, טיפוס וברירת מחדל.
- **views של שכבות (2a.1):** זהים ל-JC6416: אותם מבנים, שדות וסדר. אין שדה ייחודי לדגם.
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

- **ברירות מחדל שונות:** `colorYom` = 1, `colorParasha` = 1, `leil` = 1 (F9).
- **3 העדפות נוספות:**
  - `colorIvri` (72, ברירת מחדל 6)
  - `colorLoazi` (73, ברירת מחדל 1)
  - `colorParashaV` (74, ברירת מחדל 6)
  - צרכן: `Logic` → `mainScreen`. עורך: `ColorState`. שכבה מוצעת: **תצוגה**.
- **אינדקסים שזזו:** `screen` = 75 (ברירת מחדל 2), `bt` = 76 (F2).
- **`screen` בשימוש** (שכבה: הצגה · פריסה):
  - צרכנים: `Logic` (`mainScreen.setScreen`), `FontState`, `SetClockState` (`setScreen`), `MainState`.
  - `MainState` גם כותב אותו ב-`upAction`/`downAction` של המסך הראשי. כלומר, לחיצה למעלה או למטה במסך הראשי מחליפה את המסך ושומרת את הבחירה.
- **`dateFormat`:** נקרא גם ב-`Screens/MainScreen.cpp:91`, כלומר שכבת התצוגה קוראת העדפה ישירות. הפריסה משתנה כשאין תאריך לועזי.
- **`loazi`:** יש לו עורך ואין לו צרכן.
- **`MainStateInfoSets`:** מפוצל ל-`…1.cpp` ול-`…2.cpp`. הצרכנים של `tfila`, `moadim`, `IORT` ושאר בחירות התוכן מתחלקים בין שני הקבצים.
- **views (2a.1):** like JC6416; `DisplayPrefs` adds `colorIvri`, `colorLoazi`, `colorParashaV` at the end (index order); `screen` in `layout`.

## 9. תשובות והחלטות

1. **`forceWrite`** → **רישום מרכזי.** נועד לאיפוס מרחוק: אם הערך שונה מ-0, `setup` מאפס את ההעדפות לברירות מחדל ומאפס גם את `forceWrite`. בפועל לא בשימוש. נשאר בטבלה לתאימות (`PREF_OP_READ`/`SYNC`).
   - **בדיקה (2a.3.1):** `forceWrite` לא נקרא בשום מקום בקוד, בשלושת הדגמים. הוא נשאר רק בגלל חוזה ה-BLE (אינדקס 1).
2. **`dayIndex`** → **נמחק ב-7.08.** נועד לשמור את אינדקס היום, כדי לשחזר את התאריך אחרי נפילת חשמל וסוללה. בפועל נכתב כל יום (`MainState::setNewDay`) ולא נקרא אף פעם. לא נכנס למבנה של שכבה, ונמחק בפרודקשן 7.08 בשלושת הדגמים (refactor-log F16). האינדקסים מפורשים, ולכן אינדקס 3 הוא חור ושום אינדקס אחר לא זז.
3. **`safa`** → **הצגה.** לשימוש עתידי כשיתווספו שפות. לא בשימוש ב-JC, אבל בשימוש בדגמי BA ובדגמים אחרים.
4. **`dateFormat`** → **הצגה.** הערך 8 (טקסט ריק) נוסף לבקשת לקוח שלא רצה תאריך לועזי. אחר כך הפריסה הותאמה להופעת התאריך הלועזי, ונבדקת מול הערך 8. כוונה עתידית: להפריד ל"הצג תאריך לועזי" ולפורמט התאריך. שיפור עתידי, לא לביצוע עכשיו.
5. **`shaonKaiz`** → **נתונים, חלק מרשומת המקום.** לא חלק משירות הזמן. ידני: מוסיף שעה לזמנים המחושבים, לא לשעון. אוטומטי: לפי האלגוריתם של ישראל או של ארה"ב.
   - **לבדיקה:** לפי הקוד, `SetShaonKaiz` מזיז את ה-RTC ([SetClockState.cpp:173-180](../../JC6416/src/States/SetClockState.cpp#L173-L180)), וב-02:00 `MainState` מזיז אותו ([MainState.cpp:238-246](../../JC6416/src/States/MainState.cpp#L238-L246)). צריך לתעד מה בדיוק קורה ל-RTC ולזמנים בכל מצב.
6. **`leil`** → **הצגה.** אחרי השקיעה אפשר להציג את היום כיום הנוכחי או כ"ליל" של מחר. משפיע רק על התצוגה. שינויים אחרים שקורים אחרי השקיעה הם נושא נפרד.
7. **`screen`** (JC6432) → **הצגה · פריסה.** שתי חלוקות שונות של האזורים במסך הראשי. התוכן מותאם לחלוקה, ולא להפך.
8. **`textPlace`** → **נתונים, חלק מרשומת המקום.** לערים ברשימה יש שמות מובנים. מקום מותאם (`ofek` = 0) מקבל כברירת מחדל את השם "מותאם אישית", ואפשר לשנות אותו מהאפליקציה.
9. **תגובה לכתיבה מ-BLE.** האפליקציה לא שולחת `restart` אחרי כתיבת העדפות; `SYS_OP_RESTART` נשלח רק מכפתור ידני (`home_tab.dart:203`). **דרישה:** כל שינוי נטמע מיד ומציג מסך הצלחה, דרך אותה פונקציה שהתפריט מפעיל. ראה 6.6.
   - **כיוון לתכנון:** BLE לא מכיר את `Logic`. `Preferences` מגדיר לכל העדפה ראשית את הפעולה הלוגית שקורית בשינוי, והפונקציות הקיימות של התפריט מבצעות אותה.
   - **נדחה:** איך `Logic` מקבל את ההודעה, ובאיזה הקשר ריצה ההטמעה מתבצעת.
   - **חריג מכוון:** `bt` נטמע רק באתחול (6.6).
10. **`ofek`** → **נתונים, רשומת מקום, העדפה ראשית.** `ofek` הוא אינדקס העיר ברשימת הערים השמורה. 0 = מקום שהלקוח מגדיר ידנית או מהאפליקציה. מקום מוגדר על ידי קואורדינטות, גובה, ישראל או לא, אזור זמן UTC, והאם שעון קיץ פעיל, והאם העיר מוקפת חומה (`homa`). בחירת עיר כותבת את `homa` מנתוני העיר, כמו את `nz_hul`. האפליקציה כותבת את השדות ובסוף `ofek` = 0 בתור "החל מיקום מותאם" (6.6). מ-7.05 גם בחירת עיר מהאפליקציה (`ofek` ≠ 0) נטמעת מיד, דרך הפונקציה של התפריט (6.4).
11. **`nz_hul`** → **נתונים, רשומת מקום.** `nz_hul` הוא ישראל/חו"ל של המקום הפעיל, לא רק של המקום המותאם. בחירת עיר קובעת אותו, גם על חשבון הערך של המקום המותאם. זה מכוון (F10, סגור). אחרי חזרה למקום מותאם הערך נשאר של העיר, עד שכותבים אותו מחדש. האפליקציה כותבת אותו בכל החלה של מקום מותאם (6.6).