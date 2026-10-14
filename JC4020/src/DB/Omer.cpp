#include "DayInfo.h"

void line1_baladi(char* buffer, int asarot, int ahadot)
{
    if(asarot==0) // 1-9
    { 
        if(ahadot==1) { sprintf(buffer, "%s %s %s %s", omer_words[25], omer_words[28], omer_words[1], omer_words[23]); } // 1
        else { sprintf(buffer, "%s %s %s %s", omer_words[25], omer_words[24], omer_words[ahadot], omer_words[23]); } // 2-9
    }
    else if(asarot==1) // 10-19
    { 
        if(ahadot==0) { sprintf(buffer, "%s %s %s %s", omer_words[25], omer_words[24], omer_words[10], omer_words[23]); } // 10
        else if(ahadot==1) { sprintf(buffer, "%s %s %s %s %s", omer_words[25], omer_words[24], omer_words[11], omer_words[1], omer_words[23]); } // 11
        else { sprintf(buffer, "%s %s %s %s %s", omer_words[25], omer_words[24], omer_words[11], omer_words[ahadot+10], omer_words[23]); } // 12-19
    }
    else // 20-49
    { 
        int asar = 0;
        if(asarot==2) asar = 20;
        if(asarot==3) asar = 21;
        if(asarot==4) asar = 22;
        if(ahadot==0) { sprintf(buffer, "%s %s %s %s", omer_words[25], omer_words[24], omer_words[asar], omer_words[23]); } // 20 30 40
        else { sprintf(buffer, "%s %s %s%s %s %s", omer_words[25], omer_words[24], omer_words[ahadot], omer_words[53], omer_words[asar], omer_words[23]); } // 21-29 31-39 41-49
    }
}
void line2_baladi(char* buffer, int shavua, int yomBashavua)
{
    char shavuaStr[25];
    if(shavua==0) { sprintf(shavuaStr, "%s", "");  } // 1-6
    else if(shavua==1) { sprintf(shavuaStr, "%s %s %s ", omer_words[27], omer_words[1], omer_words[26]); } // 7-13
    else { sprintf(shavuaStr, "%s %s %s ", omer_words[29], omer_words[shavua], omer_words[26]); } // 14-49

    char yomBashavuaStr[25];
    if(yomBashavua==0 || shavua==0) // 0-7 14 21 28 35 42 49
    { 
        if(shavua==7) { sprintf(yomBashavuaStr, "%s ", omer_words[30]); } // 49
        else { sprintf(yomBashavuaStr, "%s", ""); } // 0-7 14 21 28 35 42
    }
    else if(yomBashavua==1) { sprintf(yomBashavuaStr, "%s %s%s ", omer_words[28], omer_words[1], omer_words[53]); } // 8 15 22 29 36 43
    else { sprintf(yomBashavuaStr, "%s %s%s ", omer_words[24], omer_words[yomBashavua], omer_words[53]); } // 9-13 16-20 23-27 30-34 37-41 44-48

    sprintf(buffer, "%s%s", yomBashavuaStr, shavuaStr);
}

void line1_base(char* buffer, int asarot, int ahadot)
{
    if(asarot==0) // 1-9
    { 
        if(ahadot==1) { sprintf(buffer, "%s %s %s", omer_words[31], omer_words[51], omer_words[46]); } // 1
        else if(ahadot==2) { sprintf(buffer, "%s %s %s", omer_words[47], omer_words[42], omer_words[46]); } // 2
        else { sprintf(buffer, "%s %s %s", omer_words[47], omer_words[ahadot+30], omer_words[46]); } // 3-9
    }
    else if(asarot==1) // 10-19
    { 
        if(ahadot==0) { sprintf(buffer, "%s %s %s", omer_words[47], omer_words[40], omer_words[46]); } // 10
        else { sprintf(buffer, "%s %s %s %s", omer_words[51], omer_words[11], omer_words[ahadot+30], omer_words[46]); } // 11-19
    }
    else // 20-49
    { 
        int asar = 0;
        if(asarot==2) asar = 43;
        if(asarot==3) asar = 44;
        if(asarot==4) asar = 45;
        if(ahadot==0) { sprintf(buffer, "%s %s %s", omer_words[51], omer_words[asar], omer_words[46]); } // 20 30 40
        else { sprintf(buffer, "%s %s%s %s %s", omer_words[51], omer_words[asar], omer_words[53], omer_words[ahadot+30], omer_words[46]); } // 21-29 31-39 41-49
    }
}
void line2_base(char* buffer, int shavua, int yomBashavua)
{
    char shavuaStr[25];
    if(shavua==0) { sprintf(shavuaStr, "%s", "");  } // 1-6
    else if(shavua==1) { sprintf(shavuaStr, "%s %s ", omer_words[31], omer_words[50]); } // 7-13
    else if(shavua==2) { sprintf(shavuaStr, "%s %s ", omer_words[52], omer_words[42]); } // 14-20
    else { sprintf(shavuaStr, "%s %s ", omer_words[52], omer_words[shavua+30]); } // 21-49

    char yomBashavuaStr[25];
    if(yomBashavua==0 || shavua==0) { sprintf(yomBashavuaStr, "%s", ""); } // 0-7 14 21 28 35 42 49
    else if(yomBashavua==1) { sprintf(yomBashavuaStr, "%s %s%s ", omer_words[31], omer_words[51], omer_words[53]); } // 8 15 22 29 36 43
    else if(yomBashavua==2) { sprintf(yomBashavuaStr, "%s %s%s ", omer_words[47], omer_words[42], omer_words[53]); } // 9 16 23 30 37 44
    else { sprintf(yomBashavuaStr, "%s %s%s ", omer_words[47], omer_words[yomBashavua+30], omer_words[53]); } // 10-13 17-20 24-27 31-34 38-41 45-48

    sprintf(buffer, "%s%s", yomBashavuaStr, shavuaStr);
}

void omer_baladi(char* line1, char* line2, int asarot, int ahadot, int shavua, int yomBashavua)
{
	line1_baladi(line1, asarot, ahadot);
	line2_baladi(line2, shavua, yomBashavua);
}
void omer_EM(char* line1, char* line2, int asarot, int ahadot, int shavua, int yomBashavua)
{
	char buffer1[50];
	line1_base(buffer1, asarot, ahadot);
	sprintf(line1, "%s %s", omer_words[48], buffer1);
	if (shavua != 0)
	{
		char buffer2[50];
		line2_base(buffer2, shavua, yomBashavua);
		sprintf(line2, "%s%s", buffer2, omer_words[49]);
	}
	else {
		line2_base(line2, shavua, yomBashavua);
	}
}
void omer_ashkenaz(char* line1, char* line2, int asarot, int ahadot, int shavua, int yomBashavua, bool la)
{
	int laomer = 0;
	if(la) {laomer=48;}
	else{ laomer=41;}
	if (yomBashavua == 0)
	{
		line1_base(line1, asarot, ahadot);
		char buffer2[50];
		line2_base(buffer2, shavua, yomBashavua);
		sprintf(line2, "%s %s%s", omer_words[laomer], buffer2, omer_words[49]);
	}
	else {
		char buffer1[50];
		line1_base(buffer1, asarot, ahadot);
		sprintf(line1, "%s %s", omer_words[49], buffer1);
		char buffer2[50];
		line2_base(buffer2, shavua, yomBashavua);
		sprintf(line2, "%s %s", omer_words[laomer], buffer2);
	}
	if (shavua == 0)
	{
		char buffer1[50];
		line1_base(buffer1, asarot, ahadot);
		sprintf(line1, "%s %s", omer_words[laomer], buffer1);
		line2_base(line2, shavua, yomBashavua);
	}
}


void DayInfo::getOmer(char* line1, char* line2)
{
    int ahadot = omer%10;
    int asarot = omer/10;
    int shavua = omer/7;
    int yomBashavua = omer%7;

    if(userPreferences.eda==TEIMAN_BALADI) { omer_baladi(line1, line2, asarot, ahadot, shavua, yomBashavua); }
    if(userPreferences.eda==TEIMAN_SHAMI || userPreferences.eda==EDOT_MIZRAH) { omer_EM(line1, line2, asarot, ahadot, shavua, yomBashavua);}
    if(userPreferences.eda==SFARAD || userPreferences.eda==CHABAD) { omer_ashkenaz(line1, line2, asarot, ahadot, shavua, yomBashavua, true);}
    if(userPreferences.eda==ASHKENAZ) { omer_ashkenaz(line1, line2, asarot, ahadot, shavua, yomBashavua, false);}
}
