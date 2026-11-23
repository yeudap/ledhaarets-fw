**Pending.** Based on commit `0e48b1a` (production 7.07). Re-run the comparison before writing models.md. Differences that production has since aligned are no longer listed; the line counts in §2 and §6 are the estimates from the base.

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
לקבצים ולשורות. הוא יקשר ל-`preferences.md` §8 ול-F3 ו-F9 ב-`refactor-log.md` במקום לחזור עליהם.

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
  כל `BTN/`, כל `CLK/`, `Colors.h`, `Hardware.h`, `Metadata.h`, `metadata.cpp`, `Area.h`, `Area.cpp`,
  `DB.*`, `DayInfo.h`, `HebrewDate.h`, `Omer.cpp`, `Place.*`, `Shita.*`, `Sun.*`, `Zman.*`, `Preferences/UserPreferences.cpp`,
  `MinhagState`, `EdotState`, `ShitaState`, `OfekState`, `State.*`, `States.cpp`, `UpdateState`, רוב `Screens/`, `stamp_app_desc.py`, וכל `DB/DataBase/`
  (ה-`luchotChai` וה-`kiseRahamim` זהים).
- **שונה מעט:** `platformio.ini` (`DEVICE_MODEL`), `Text.*`, `Display.*`, `main.cpp`, `DafYomi.h`, `LoaziDate.h`,
  `Dict.h`, `Strings.*`, `DayInfo.cpp`, `UserPreferences.h`, `ConfigState` (JC4020), `FontState`, `BrightnessState`,
  `SetClockState`, `Digit.h`, `SetInputText`, `SetTochenInfoState`, `SetTochenTimeState`, `TextCommandHandler.cpp` (JC4020).
- **שונה מהותית:** `MainScreen.cpp`, `Screens.h`, `Logic.*`, `States.h`, `MainState.cpp`,
  `MainStateTimeSets.cpp`, `MainStateInfoSets*.cpp`, `ColorState.cpp`, `Digit.cpp`, `Fonts/ariel10.h` (JC4020).
- **חלקי:** `TextScreen.cpp`, `SetInputText`, `SetTochenInfoState`, `SetTochenTimeState` (לא ב-JC4020);
  `SetTochenState.cpp` (רק ב-JC4020); `MainStateInfoSets1/2.cpp` (רק ב-JC6432); גופנים `number10/12/15/15d/28/35/35d/37`,
  `russian` (לא ב-JC4020).
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
- **חוזה:** הטבלה אחת (מפתחות, אינדקסים, טיפוסים). לפי דגם: ברירות המחדל של `colorYom` ו-`colorParasha` (F9),
  והמשמעות של `clockFont`, של `screen`, ושל `dateFormat` אם יתברר כמכוון.

### 4. רשימת סטיות והמלצה (סעיף 3 בבקשה)
| # | סטייה | הדגם הנכון (המלצה) | סטטוס |
|---|---|---|---|
| S3 | `Display.cpp` ב-JC6432: `dumpMem`, שלא נקרא | להשאיר | **בכוונה**, ככלי debug, כי ב-JC6432 הזיכרון צפוף |
| S6 | קבצים יתומים: גופנים שלא נכללים (ב-JC6416 `number10/12/15/15d/28/33/35/35d/37`, `russian`; `number33` ב-JC4020; ב-JC6432 `number10/12/15/15d/28`, `russian`) | הגופנים נשארים כספריית גופנים. בלי השפעה על הבינארי | **נשארים בכוונה**, כספריית גופנים. בתוכנית הריפקטור תהיה תיקיית `Fonts` אחת לכל הדגמים. לפני האיחוד צריך לוודא שקבצים עם אותו שם זהים גם בתוכן, אחרת דגם יקבל גופן אחר בלי שנשים לב. הרשימה המלאה כאן משמשת לבדיקה הזו |

### 5. לא ברור — שאלות אליך
- Q1: `LoaziDate.h` ב-JC6432: שנה בשתי ספרות בכל הפורמטים, ופורמט 3 הוא חודש/יום. כלומר ערך `dateFormat` מציג משהו אחר בכל דגם. האם זה מכוון (מקום) או סטייה? ההערות בקובץ לא עודכנו.
- Q2: `DayInfo.cpp`: נוסח "שנים למדינת ישראל" מול "ה-N למדינת ישראל", ו"לחורבן" מול "לחורבן בית שני". איזה נוסח נכון, או שזה מכוון לפי רוחב? **החלטה:** להשאיר את הנוסח הנוכחי, ולהחליט כשנחזור למפת ההבדלים.
- Q3: Morphing: ב-JC6416 וב-JC4020 הציור הטקסטואלי של הספרות לא נחסם בזמן morphing. ב-JC6432 הוא נחסם (`if(!morphing)`). תיקון שצריך להעביר, או הבדל שנובע מהגופן?
- Q4: פריסה Big ב-JC6432: `flaginfoSet_1_Omer` לא בודק `tfila`, ו-`HagYaaleVeyavoHalel` לא מוצג בימי צום. מכוון?
- Q5: ב-`flaginfoSet_1_Omer1` (JC6432) השורה `f && dayInfo.omer >= 7;` לא משפיעה על כלום. נראה כמו באג, מועמד ל-Finding.

### 6. הערכת שיתוף ומה ב-`models/<דגם>/` (סעיף 4 בבקשה)
- **מספרים:** ב-JC6416 יש כ-20,500 שורות קוד (בלי טבלאות וגופנים). כ-8,400 זהות בשלושת הדגמים. כ-7,300 בקבצים ששונים מעט,
  שמתאחדים בפרמטר או בדגל יכולת. כ-4,900 בקבצים ששונים מהותית, וגם בהם חלק משותף: חיווט התפריטים של שעון, אופק ומנהגים
  ב-`Logic.cpp`, ופונקציות `flag*` שזהות כמעט לגמרי.
  **הערכה: כ-75–80% משותף. בערך 3,000–4,000 שורות לכל דגם.**
- **משותף (`src/`):** BLE, BTN, CLK, Preferences (המנגנון), Metadata, Hardware, Area, Colors, DB/דומיין
  וטבלאות, רוב המסכים והמצבים, `main.cpp` (בהירות דרך שכבת התצוגה).
- **חייב לשבת ב-`models/<דגם>/`:**
  1. תצורת התצוגה: גיאומטריה, סוג ה-display (`Display.h/.cpp`).
  2. פריסת המסך הראשי: `MainScreen.cpp` וה-Areas שלו.
  3. ציור הסטים ורשימות הסטים (`MainStateTimeSets`, `MainStateInfoSets*`). כללי ה-`flag*` יכולים להיות משותפים.
  4. תפריט התוכן (`SetTochen*` וחיווט ה-Tohen ב-`Logic`), ורשימת הצבעים ב-`ColorState`.
  5. מהגופנים רק `font_list`, הרשימה של הגופנים שהדגם משתמש בהם. קובצי הגופנים עצמם יושבים בתיקיית `Fonts/` אחת,
     משותפת לכל הדגמים (S6). לפני האיחוד צריך לוודא שקבצים עם אותו שם זהים גם בתוכן. וגם: `ariel10` של JC4020,
     ופרמטרים של `Digit`.
  6. **מה שנשאר לפי דגם בהעדפות:** ברירות המחדל של `colorYom` ו-`colorParasha` (F9), והמשמעות של `clockFont` ושל `screen`.
     הטבלה עצמה אחת.
  7. דריסות של `Strings` ו-`Dict` (קיצורים, ר' צרה), ו-`LoaziDate` אם Q1 מכוון.
- **דגלי יכולת מוצעים:** `HAS_TEXTS` (JC6416, JC6432), `HAS_SCREEN_SWITCH` (JC6432). ב-JC4020 התשובות של `TEXT_OP_*`
  נשארות `INVALID_INDEX`, כי הן חלק מהחוזה (F3). רק המקום שבו ההחלטה מתקבלת משתנה: שכבת ה-BLE תהיה זהה
  בשלושת הדגמים, וההחלטה אם לקבל טקסטים תעבור ללוגיקה כשהלוגיקה תופרד מ-BLE. התפריט של JC4020 בלי `resetText`
  (`ResetPrefState::showStateScreen`, `ResetPassState::showStateScreen` ב-`ConfigState.cpp`) מכוון.

## אימות
- אין שינוי קוד, ולכן אין `pio run`. אוודא ש-`git status` מראה רק את `docs/architecture/models.md` כקובץ חדש.
- אבדוק שכל קישור יחסי במסמך מצביע לקובץ קיים, ושמספרי השורות שמצוטטים נכונים ל-`0e48b1a`.
- כל מחרוזת חוזה שמוזכרת (אינדקסים, מפתחות) תצוטט מהקוד, בלי לשנות דבר.
- הודעת commit מוצעת: `docs: model differences map (JC6416 / JC4020 / JC6432)`.
