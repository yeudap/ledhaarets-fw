# הערות ארכיטקטורה — תשובות והחלטות

תשובות של בעל הפרויקט לשאלות שעלו במיפוי (שלב 0, משימה ב). הקובץ הזה הוא מקור האמת להיגיון ולאילוצים
שלא נראים מהקוד. המיפוי עצמו נמצא ב-[dependencies.md](dependencies.md).

בסיס: JC6416, commit `9c75812`.

## מטרת המיפוי

המטרה היא חלוקה לשכבות, לא חיפוש פגמים. הקוד עובד. שאלות נשאלות רק כשהן משפיעות על השיוך של יחידה לשכבה.

## כיוון התלויות הרצוי (השערה לבדיקה)

מלמעלה למטה:

1. `main`
2. מקורות קלט: BLE (כולל `Commands`) ו-BTN
3. זרימה: `Logic` ו-`States`
4. תצוגה (`Screens` → `Area`) ודומיין (`DB`, `DayInfo`, `Shita`)
5. שירותי מערכת: Preferences/NVS ו-Clock
6. חומרה

`Metadata` היא שכבת "תצורה" בבסיס. כל השכבות רשאיות להשתמש בה, ותלות בה אינה קשת נגד הכיוון.

## תפקידי יחידות

- **`main`** ([main.cpp](../../JC6416/src/main.cpp)): נקודת הכניסה.
  - בונה את החלקים ומריצה את פונקציות האתחול ב-`setup`.
  - ב-`loop` מאזינה ללחיצות ומחליפה מצב.
- **`Logic`** ([Logic.h](../../JC6416/src/Logic.h), [Logic.cpp](../../JC6416/src/Logic.cpp)): המנוע.
  - מייצר את המסכים והמצבים ומאתחל אותם.
  - מגדיר לכל מצב את המעברים לפי לחיצה.
  - אליו פונים כדי לצייר או להחליף מצב שלא דרך `main`.
- **`Area`** ([Area.h](../../JC6416/src/Area.h)): תצוגה טהורה מעל ספריית הרינדור. כל Screen מורכב מכמה Area, ולכל Area אזור תחום ומאפיינים משלה.
- **`TextCommandHandler`** ([BLE/TextCommandHandler.cpp](../../JC6416/src/BLE/TextCommandHandler.cpp)): מתאם בין BLE לשירות הטקסטים. שייך לשכבת BLE/פרוטוקול.
- **`Commands.h`** ([BLE/Commands.h](../../JC6416/src/BLE/Commands.h)): שייך ל-BLE. ממפה מצבים ולחיצות למספרים, כדי שהאפליקציה תוכל לשלוח מצב רצוי או לדמות לחיצה. יש בו שני חלקים:
  - `MenuCommand` משמש רק את BLE.
  - `ButtonCommand` ו-`currentButtonCommand` משותפים. `Button::isPushed()` ([BTN/Button.cpp:18-28](../../JC6416/src/BTN/Button.cpp#L18-L28)) מאחד לחיצה פיזית (דרך `activeButtonDriver`) ולחיצה מדומה מ-BLE (`currentButtonCommand`, שמוגדר ב-`CoreService.cpp`).
  - זה מכוון. במיפוי, הקשת BTN → BLE מסומנת כקשת הזו.

## שיוך לשכבה של יחידות שהיו לא ברורות

| יחידה | שכבה | הסבר |
| --- | --- | --- |
| `States/Digit` | תצוגה | |
| `DB/Strings` | תצוגה | טקסטים של הממשק בלבד. השאיפה היא לתמוך בכמה שפות. הטקסטים של הדומיין (פרשות, חגים) נמצאים ב-`DB/Dict` ושייכים לדומיין. |
| `Display` | חומרה | דרייבר הפאנל והחיבור לספריית HUB75. Render (`Area`, `Text`, `Colors`, `Fonts`) יושב מעליו. |
| `Hardware.h` | חומרה | כותרת מאגדת לכל קבצי החומרה, בלי לוגיקה משלה. ה-include שלה ל-`BLE/Bluetooth.h` נכנס לרשימת הקשתות נגד הכיוון. |
| `Metadata` | תצורה (בסיס) | מידע בסיסי על הפרויקט. כל השכבות רשאיות להשתמש בה. |
| `Screens/RegsScreen` | תצוגה | מסך תמיכה שמציג את כל ההעדפות בצורה ויזואלית. היה חשוב לפני שהייתה אפליקציה. |
| `States/UpdateState` | תצוגה | מסך התקדמות של OTA, שמצויר דרך `messageScreen`. לא נקרא דרך `main` או `Logic`, אלא ישירות מ-BLE בזמן OTA. זה מכוון. הקשת OTA → UpdateState מסומנת "מכוון: מסך התקדמות OTA". |

## שאלות שנדחו לשלב 2

השאלות האלה לא נדרשות לחלוקה לשכבות:

- ריבוי משימות: callbacks של NimBLE וה-worker של ה-OTA מציירים ומחליפים את `currentState` במקביל ל-`loop()`.
- ההחלפה הזמנית `currentState = &bl.updateState` סביב הודעות, והמעבר ל-`clockSetTime` ב-`TIME_OP_SET`.
- כתיבת `ofek` (אינדקס 6) מ-BLE: מפעילה את `SetCustomState::printUpdateSucc` ומאפסת את `ofek` ל-0.
- בדיקת אורך טקסט ב-BLE לפי רוחב בפיקסלים (`validateTextLen`).
- `Zman` קורא את `clockFormat` ו-`timeFormat` מההעדפות.
- `Shita::setDefault` כותב העדפות.
- שעון קיץ: הזזת ה-RTC ושינוי `place->kaiz`.
- אתחול יומי ב-03:01.
- `HARDWARE_MODEL` הקבוע ב-`Hardware.h` מול `HARDWARE_VERSION` ב-build flags.
- מיקום הגלובליים: `strings`, `bl` ו-`currentState` ב-`main.cpp`, אובייקטי ה-Screen וה-`DayInfo` ב-`Logic.cpp`, ו-`display` ב-`Area.cpp`.
- `DB.cpp:34-35`: הקריאה של `molad_shaa` ו-`molad_daka`.
