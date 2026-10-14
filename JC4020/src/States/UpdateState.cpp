#include "States.h"
#include "Logic.h"

extern Logic bl;
extern State* currentState;


void UpdateState::showStateScreen() { messageScreen.printScreen(); }
void UpdateState::updateScreen(uint8_t percent) 
{ 
    char message[10];
    sprintf(message, "%d%%", percent);
    messageScreen.printProgressBar(messageScreen.INFO, "...vbfuw ifsgn", message, percent); 
    // מעדכן תוכנה
}

void UpdateState::showUpdateStartScreen()
{
    /// נקודה 1: מעבר מהמסך הראשי למסך העדכון הייעודי.
    /// נקרא פעם אחת, אחרי שכל הולידציות עברו וה-worker רץ, לפני START_OK.
    currentState = &bl.updateState;
}
void UpdateState::showUpdateProgress(uint8_t percent)
{
    /// נקודה 2: עדכון אחוז ההתקדמות (נקרא כל ~500ms מה-worker).
    bl.updateState.updateScreen(percent);
}
void UpdateState::showUpdateSuccessScreen()
{
    /// נקודה 3: העדכון נצרב בהצלחה — הודעת הצלחה לפני האתחול.
    /// נקרא מ: _handleClientFinalOk (רגיל), ה-watchdog (אפליקציה נעלמה),
    /// ו-_handleAbort במצב DONE (ביטול מאוחר מדי = הצלחה).
    bl.updateState.showStateScreen();
    messageScreen.printMessages(messageScreen.SUCCESS, "!okauv iufsgv", "wgf  kujwt  gmcn  iugav", 1000);
    // העדכון הושלם!, השעון יבצע אתחול כעת
}
void UpdateState::showUpdateCancelledScreen()
{
    /// נקודה 4א: העדכון בוטל ע"י המשתמש — הודעה + חזרה למסך הראשי.
    /// כלום לא נצרב, השעון ממשיך בגרסה הנוכחית בלי אתחול.
    bl.updateState.showStateScreen();
    messageScreen.printMessages(messageScreen.INFO, "...kyuc iufsgv", "ohhubha ugmuc tk", 1000);
    currentState = &bl.mainState;
    currentState->showStateScreen();
    // העדכון בוטל... לא בוצעו שינויים
}
void UpdateState::showUpdateFailedScreen(bool willRestart)
{
    /// נקודות 4ב+4ג: העדכון נכשל. כלום לא נצרב — השעון ממשיך בגרסה הנוכחית
    ///   willRestart=false — סיום נקי (MD5, ניתוק, ביטול-NACK): הודעה
    ///                       + חזרה למסך הראשי, בלי אתחול.
    ///   willRestart=true  — כשל ללא התאוששות (flash, timeout, הקצאה):
    ///                       הודעה בלבד; _fatalRestart מאתחל מיד אחרי.
    if(willRestart)
    {
        messageScreen.printMessages(messageScreen.ERROR, "!kafb iufsgv", "wgf  kujwt  gmcn  iugav", 1000);
        // העדכון נכשל! השעון יבצע אתחול כעת
    }
    else{
        messageScreen.printMessages(messageScreen.ERROR, "!kafb iufsgv", "ohhubha ugmuc tk", 1000);
        currentState = &bl.mainState;
        currentState->showStateScreen();
        // העדכון נכשל! לא בוצעו שינויים
    }
}
