#include "UserContent.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות 50 הטקסטים - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח (PR_OPEN).
static constexpr PrefDef<String> textInput1Def("textInput1", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput2Def("textInput2", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput3Def("textInput3", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput4Def("textInput4", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput5Def("textInput5", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput6Def("textInput6", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput7Def("textInput7", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput8Def("textInput8", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput9Def("textInput9", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput10Def("textInput10", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput11Def("textInput11", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput12Def("textInput12", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput13Def("textInput13", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput14Def("textInput14", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput15Def("textInput15", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput16Def("textInput16", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput17Def("textInput17", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput18Def("textInput18", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput19Def("textInput19", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput20Def("textInput20", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput21Def("textInput21", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput22Def("textInput22", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput23Def("textInput23", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput24Def("textInput24", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput25Def("textInput25", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput26Def("textInput26", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput27Def("textInput27", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput28Def("textInput28", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput29Def("textInput29", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput30Def("textInput30", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput31Def("textInput31", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput32Def("textInput32", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput33Def("textInput33", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput34Def("textInput34", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput35Def("textInput35", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput36Def("textInput36", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput37Def("textInput37", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput38Def("textInput38", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput39Def("textInput39", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput40Def("textInput40", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput41Def("textInput41", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput42Def("textInput42", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput43Def("textInput43", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput44Def("textInput44", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput45Def("textInput45", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput46Def("textInput46", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput47Def("textInput47", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput48Def("textInput48", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput49Def("textInput49", "", PT_STR, 0, &prefs, PR_OPEN);
static constexpr PrefDef<String> textInput50Def("textInput50", "", PT_STR, 0, &prefs, PR_OPEN);

UserContent::UserContent() :
                        textInput1(textInput1Def),
                        textInput2(textInput2Def),
                        textInput3(textInput3Def),
                        textInput4(textInput4Def),
                        textInput5(textInput5Def),
                        textInput6(textInput6Def),
                        textInput7(textInput7Def),
                        textInput8(textInput8Def),
                        textInput9(textInput9Def),
                        textInput10(textInput10Def),
                        textInput11(textInput11Def),
                        textInput12(textInput12Def),
                        textInput13(textInput13Def),
                        textInput14(textInput14Def),
                        textInput15(textInput15Def),
                        textInput16(textInput16Def),
                        textInput17(textInput17Def),
                        textInput18(textInput18Def),
                        textInput19(textInput19Def),
                        textInput20(textInput20Def),
                        textInput21(textInput21Def),
                        textInput22(textInput22Def),
                        textInput23(textInput23Def),
                        textInput24(textInput24Def),
                        textInput25(textInput25Def),
                        textInput26(textInput26Def),
                        textInput27(textInput27Def),
                        textInput28(textInput28Def),
                        textInput29(textInput29Def),
                        textInput30(textInput30Def),
                        textInput31(textInput31Def),
                        textInput32(textInput32Def),
                        textInput33(textInput33Def),
                        textInput34(textInput34Def),
                        textInput35(textInput35Def),
                        textInput36(textInput36Def),
                        textInput37(textInput37Def),
                        textInput38(textInput38Def),
                        textInput39(textInput39Def),
                        textInput40(textInput40Def),
                        textInput41(textInput41Def),
                        textInput42(textInput42Def),
                        textInput43(textInput43Def),
                        textInput44(textInput44Def),
                        textInput45(textInput45Def),
                        textInput46(textInput46Def),
                        textInput47(textInput47Def),
                        textInput48(textInput48Def),
                        textInput49(textInput49Def),
                        textInput50(textInput50Def)
{
}

UserContent userContent;
