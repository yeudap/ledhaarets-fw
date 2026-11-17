**Pending.** Based on commit `0e48b1a` (production 7.07). Re-run the comparison before writing models.md if a newer production version was synced.

# תוכנית: `docs/architecture/models.md` — מפת הבדלים בין הדגמים

## הקשר

לקראת פרויקט אחד עם `[env:<דגם>]` צריך לדעת מה בקוד באמת שונה בין JC6416, JC4020 ו-JC6432:
מה נובע מהחומרה או מהפריסה, מה פיצ'ר של דגם, ומה סתם התפצלות. התוצר הוא מסמך אחד בלבד.
**שום קובץ קוד לא משתנה.** גם `refactor-log.md` לא משתנה. ממצא שנראה כבאג יירשם במסמך כ"מועמד ל-Finding",
ואני אציע להעביר אותו ל-Findings בצעד נפרד.

בסיס: commit `0e48b1a` (פרודקשן 7.07). שיטה: `diff -w --strip-trailing-cr` לכל קובץ מול JC6416,
ובקבצים שונים גם JC4020 מול JC6432, כדי לזהות מי הדגם החריג.

## הקובץ שייכתב

`docs/architecture/models.md` בלבד. בעברית, בסגנון של `preferences.md` ו-`notes.md`, עם קישורים יחסיים
לקבצים ולשורות. הוא יקשר ל-`preferences.md` §8 ול-F2, F3 ו-F9 ב-`refactor-log.md` במקום לחזור עליהם.

## מבנה המסמך ותוכן עיקרי (מה שנמצא)

### 1. תמונת מצב של הדגמים
| | JC6416 | JC4020 | JC6432 |
|---|---|---|---|
| פאנל | 2×128×64 בשרשרת = 256×64 | 1×128×64 | 2×2 `VirtualMatrixPanel_T` = 256×128 |
| מסך ראשי | שורה עליונה (תאריכים + עילוי נשמה), שעון + יום + פרשה, שני אזורים מתחלפים (זמנים 8, מידע 21) | שורה עליונה, שעון + יום, שורה תחתונה אחת מתחלפת (33 סטים) | שתי פריסות (`screen`: 1 = Big, 2 = Small), מעבר בלחיצה למעלה/למטה; זמנים 7, מידע 28 / 21 |
| טקסטים (עילוי נשמה) | כן | לא (F3) | כן |
| צבעים בתפריט | 7 | 4 | 10 |

`platformio.ini` זהה בשלושת הדגמים חוץ מ-`DEVICE_MODEL`: אותה פלטפורמה, אותם `lib_deps`
(ה-`VirtualMatrixPanel` של JC6432 נמצא באותה ספריית HUB75). זה בסיס טוב ל-`[env:<דגם>]`.

### 2. טבלת קבצים (סעיף 1 בבקשה)
ארבעה מצבים: **זהה** / **שונה מעט** / **שונה מהותית** / **חלקי** (לא קיים בכל הדגמים). עמודה לכל דגם, ועמודת סיווג.
- **זהה בשלושה** (~8,400 שורות קוד + ~9,400 שורות טבלאות וגופנים): כל `BLE/` חוץ מ-`TextCommandHandler.cpp`,
  כל `BTN/`, `ClockDS3231.*`, `ClockDriver.h`, `Colors.h`, `Hardware.h`, `Metadata.h`, `metadata.cpp`, `Area.h`,
  `DB.*`, `DayInfo.h`, `HebrewDate.h`, `Omer.cpp`, `Place.*`, `Shita.*`, `Sun.*`, `Zman.*`, `Preferences/UserPreferences.cpp`,
  `MinhagState`, `EdotState`, `ShitaState`, `State.*`, `States.cpp`, `UpdateState`, רוב `Screens/`, `stamp_app_desc.py`, וכל `DB/DataBase/`
  (ה-`luchotChai` וה-`kiseRahamim` זהים).
- **שונה מעט:** `platformio.ini`, `Area.cpp`, `Text.*`, `Display.*`, `main.cpp`, `Clock.cpp`, `Clock.h`, `DafYomi.h`, `LoaziDate.h`,
  `Dict.h`, `Strings.*`, `DayInfo.cpp`, `UserPreferences.h`, `ConfigState`, `OfekState` (רווחים), `FontState`, `BrightnessState`,
  `SetClockState`, `Digit.h`, `SetInputText`, `SetTochenInfoState`, `SetTochenTimeState`, `TextCommandHandler.cpp` (JC4020).
- **שונה מהותית:** `ClockRV3028.*` (JC6416 חריג), `MainScreen.cpp`, `Screens.h`, `Logic.*`, `States.h`, `MainState.cpp`,
  `MainStateTimeSets.cpp`, `MainStateInfoSets*.cpp`, `ColorState.cpp`, `Digit.cpp`, `Fonts/ariel10.h` (JC4020).
- **חלקי:** `TextScreen.cpp`, `SetInputText`, `SetTochenInfoState`, `SetTochenTimeState` (לא ב-JC4020);
  `SetTochenState.cpp` (רק ב-JC4020); `MainStateInfoSets1/2.cpp` (רק ב-JC6432); גופנים `number10/12/15/15d/28/35/35d/37`,
  `russian` (לא ב-JC4020); `or_BeerSheva/Haifa/Jerusalem/TelAviv_db.h` (רק ב-JC6432, לא נכללים בשום מקום).
- `.vscode/*` ו-`.gitignore`: מחוץ לקושחה. יסומנו "סביבה" ולא ייכנסו לסיווג.

### 3. פירוט ההבדלים עם סיווג (סעיף 2 בבקשה)
לכל הבדל: קובץ ושורות, מה שונה, וסיווג אחד. עיקרי הממצאים:
- **תצורת חומרה:** `Display.h/.cpp` (גיאומטריה, `PANELS_NUMBER`, virtual 2×2, `HZ_8M` מול `HZ_10M`, `double_buff`, `min_refresh_rate`);
  `display` מול `dma_display` ב-`main.cpp` וב-`BrightnessState` (JC6432).
- **פריסה:** `MainScreen.cpp` ו-`Screens.h` (אזורים); `Text.cpp` ו-`Text.h` (רשימת גופני השעון; משמעות הערך של `clockFont` שונה
  בכל דגם, וה-morphing הוא 4 מול 0); `Digit.cpp` (גודל הסגמנטים); `ColorState` (אילו אזורים מסומנים) ו-3 הצבעים הנוספים של JC6432;
  `ariel10` של JC4020 (ספרות ברוחב קבוע, באותו שם); `R` ב-`Dict.h` (ר' צרה, רוחב 4 מול 5) ב-JC6416 וב-JC4020; קיצורים
  ב-`Strings` (JC6432); `DafYomi` בבלי/ירושלמי (JC6432); הסרת "תאריך עברי" ו"לועזי" מתפריט התוכן ב-JC6432 (קבועים על המסך);
  פונט הבדיקה ב-`validateTextLen` (בדיקת אורך טקסט ב-BLE תלויה בפריסה); `screen` ו-`setScreen`.
- **יכולת:** טקסטים ועילוי נשמה (`TextScreen`, `SetInputText`, `TextCommandHandler`, `resetText` בתפריט ההגדרות) לא קיימים
  ב-JC4020 (F3); מעבר בין שתי פריסות ב-JC6432; עורך תוכן אחר ב-JC4020 (`upLine`/`middleLine`/`interval`/`ZSZ`/`ZYS`/`ZPS`/`IPO`,
  לפי `preferences.md` §8); סטים מפוצלים בפריסה Big של JC6432 (`Tahanun`, `Bavli`, `…1`, `…2`).
- **חוזה:** אינדקסים וברירות מחדל (F2, F9), והמשמעות לפי דגם של `clockFont`, של `screen`, ושל `dateFormat` אם יתברר כמכוון.
  יודגש שבפרויקט אחד טבלת ההעדפות חייבת להישאר לפי דגם.

### 4. רשימת סטיות והמלצה (סעיף 3 בבקשה)
| # | סטייה | הדגם הנכון (המלצה) |
|---|---|---|
| S1 | `ClockRV3028`: ב-JC6416 יש `ensureBackupConfiguration` ו-`rtc.begin(Wire, true, false, false)`. ב-JC4020 וב-JC6432 יש `rtc.begin()` עם ברירות המחדל (`disableTrickleCharge`, `setBackupSwitchoverMode` בכל אתחול) | JC6416. דורש בדיקה על כרטיסי RV3028 של JC4020 ו-JC6432 |
| S2 | `validate_time`: תאריך ברירת המחדל הוא 2025-01-01 ב-JC4020, ו-2026-09-01 ב-JC6416 וב-JC6432 | JC6416/JC6432 |
| S3 | `Display.cpp` ב-JC6432: הדפסות debug ל-Serial, ו-`dumpMem` שלא נקרא | להסיר (כמו JC6416), להשאיר את פרמטרי החומרה |
| S4 | הגלובלי `display`: מוגדר ב-`Area.cpp` ב-JC6416 וב-JC4020, וב-`Display.cpp` ב-JC6432 | JC6432 (`Display` = שכבת החומרה, notes.md) |
| S5 | קוד debug בהערה: `DayInfo.cpp` ו-`main.cpp` (`rtc.adjust`) ב-JC6432 | JC6416 (בלי) |
| S6 | קבצים יתומים: גופנים שלא נכללים (ב-JC6416 `number10/12/15/15d/28/33/35/35d/37`, `russian`; `number33` ב-JC4020) ו-`or_*` באותיות גדולות ב-JC6432 | להשאיר רק קבצים בשימוש. בלי השפעה על הבינארי |
| S7 | כתיבת `dayIndex`: קיימת ב-JC6416, ובהערה או חסרה ב-JC4020 וב-JC6432 | להחליט יחד עם `preferences.md` 9.2. לא ליישר לפני כן |
| S8 | רווחים בלבד: `Clock.h` ו-`ConfigState` (JC4020), `OfekState`, `platformio.ini` (JC6432) | כל אחד מהם. מתיישר מעצמו במיזוג |
| S9 | `.gitignore` | JC6432 (המלא) |

### 5. לא ברור — שאלות אליך
- Q1: `LoaziDate.h` ב-JC6432: שנה בשתי ספרות בכל הפורמטים, ופורמט 3 הוא חודש/יום. כלומר ערך `dateFormat` מציג משהו אחר בכל דגם. האם זה מכוון (מקום) או סטייה? ההערות בקובץ לא עודכנו.
- Q2: `DayInfo.cpp`: נוסח "שנים למדינת ישראל" מול "ה-N למדינת ישראל", ו"לחורבן" מול "לחורבן בית שני". איזה נוסח נכון, או שזה מכוון לפי רוחב?
- Q3: Morphing: ב-JC6416 וב-JC4020 הציור הטקסטואלי של הספרות לא נחסם בזמן morphing. ב-JC6432 הוא נחסם (`if(!morphing)`). תיקון שצריך להעביר, או הבדל שנובע מהגופן?
- Q4: פריסה Big ב-JC6432: `flaginfoSet_1_Omer` לא בודק `tfila`, ו-`HagYaaleVeyavoHalel` לא מוצג בימי צום. מכוון?
- Q5: ב-`flaginfoSet_1_Omer1` (JC6432) השורה `f && dayInfo.omer >= 7;` לא משפיעה על כלום. נראה כמו באג, מועמד ל-Finding.
- Q6: מה הם קבצי `or_*` באותיות גדולות ב-JC6432 (4,017 שורות, נתונים אחרים)? אפשר למחוק?

### 6. הערכת שיתוף ומה ב-`models/<דגם>/` (סעיף 4 בבקשה)
- **מספרים:** ב-JC6416 יש כ-20,500 שורות קוד (בלי טבלאות וגופנים). כ-8,400 זהות בשלושת הדגמים. כ-7,300 בקבצים ששונים מעט,
  שמתאחדים בפרמטר או בדגל יכולת. כ-4,900 בקבצים ששונים מהותית, וגם בהם חלק משותף: חיווט התפריטים של שעון, אופק ומנהגים
  ב-`Logic.cpp`, ופונקציות `flag*` שזהות כמעט לגמרי.
  **הערכה: כ-75–80% משותף. בערך 3,000–4,000 שורות לכל דגם.**
- **משותף (`src/`):** BLE, BTN, CLK (אחרי S1 ו-S2), Preferences (המנגנון), Metadata, Hardware, Area, Colors, DB/דומיין
  וטבלאות, רוב המסכים והמצבים, `main.cpp` (בהירות דרך שכבת התצוגה).
- **חייב לשבת ב-`models/<דגם>/`:**
  1. תצורת התצוגה: גיאומטריה, סוג ה-display (`Display.h/.cpp`).
  2. פריסת המסך הראשי: `MainScreen.cpp` וה-Areas שלו.
  3. ציור הסטים ורשימות הסטים (`MainStateTimeSets`, `MainStateInfoSets*`). כללי ה-`flag*` יכולים להיות משותפים.
  4. תפריט התוכן (`SetTochen*` וחיווט ה-Tohen ב-`Logic`), ורשימת הצבעים ב-`ColorState`.
  5. גופני השעון (`font_list`), `ariel10` של JC4020, ופרמטרים של `Digit`.
  6. **טבלת ההעדפות לפי דגם** (חוזה: F2, F9, `screen`, 3 צבעים).
  7. דריסות של `Strings` ו-`Dict` (קיצורים, ר' צרה), ו-`LoaziDate` אם Q1 מכוון.
- **דגלי יכולת מוצעים:** `HAS_TEXTS` (JC6416, JC6432), `HAS_SCREEN_SWITCH` (JC6432). ב-JC4020 התשובות של `TEXT_OP_*`
  חייבות להישאר `INVALID_INDEX` (F3).

## אימות
- אין שינוי קוד, ולכן אין `pio run`. אוודא ש-`git status` מראה רק את `docs/architecture/models.md` כקובץ חדש.
- אבדוק שכל קישור יחסי במסמך מצביע לקובץ קיים, ושמספרי השורות שמצוטטים נכונים ל-`0e48b1a`.
- כל מחרוזת חוזה שמוזכרת (אינדקסים, מפתחות) תצוטט מהקוד, בלי לשנות דבר.
- הודעת commit מוצעת: `docs: model differences map (JC6416 / JC4020 / JC6432)`.
