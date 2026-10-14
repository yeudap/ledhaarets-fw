#ifndef DICT_H
#define DICT_H 

const char daysOfTheWeek1[8][6] = 
{ " ",
	"IUATR",  // 1. ראשון
	"HBA",    // 2. שני
	"HAHKA",  // 3. שלישי
	"HGHCR",  // 4. רביעי
	"HAHNJ",  // 5. חמישי
	"HAHA",   // 6. שישי
	"WCA"     // 7. שבת
};
const char daysOfTheWeek[8][6] = 
{ " ",
	"iuatr",  // 1. ראשון
	"hba",    // 2. שני
	"hahka",  // 3. שלישי
	"hghcr",  // 4. רביעי
	"hahnj",  // 5. חמישי
	"haha",   // 6. שישי
	"wca"     // 7. שבת
};

const char shana_list_s[20][5] = 
{ " ", " ", 
	"cpaw",  // 2. תשפב
	"dpaw",  // 3. תשפג
	"spaw",  // 4. תשפד
	"vpaw",  // 5. תשפה
	"upaw",  // 6. תשפו
	"zpaw",  // 7. תשפז
	"jpaw",  // 8. תשפח
	"ypaw",  // 9. תשפט
	"maw",   // 10. תשצ
	"tmaw",  // 11. תשצא
	"cmaw",  // 12. תשצב
	"dmaw",  // 13. תשצג
	"smaw",  // 14. תשצד
	"vmaw",  // 15. תשצה
	"umaw",  // 16. תשצו
	"zmaw",  // 17. תשצז
	"jmaw",  // 18. תשצח
	"ymaw",  // 19. תשצט
};

const char shana_list_l[20][6] = 
{ " ", " ", 
	"c\"paw",  // 2. תשפב
	"d\"paw",  // 3. תשפג
	"s\"paw",  // 4. תשפד
	"v\"paw",  // 5. תשפה
	"u\"paw",  // 6. תשפו
	"z\"paw",  // 7. תשפז
	"j\"paw",  // 8. תשפח
	"y\"paw",  // 9. תשפט
	"m\"aw",   // 10. תשצ
	"t\"maw",  // 11. תשצא
	"c\"maw",  // 12. תשצב
	"d\"maw",  // 13. תשצג
	"s\"maw",  // 14. תשצד
	"v\"maw",  // 15. תשצה
	"u\"maw",  // 16. תשצו
	"z\"maw",  // 17. תשצז
	"j\"maw",  // 18. תשצח
	"y\"maw",  // 19. תשצט
};

const char ona_list[4][5] = 
{
	"_he",   // 0. קיץ
	"qruj",  // 1. חורף
	"uhwx",  // 2. סתיו
	"chct"   // 3. אביב
};

const char day_part_list[6][11] = 
{
	"reuc",        // 0. לפנות בוקר
	"reuc wubpk",  // 1. בוקר
	"ohhrvm",      // 2. צהריים
	"m\"vjt",      // 3. אחה"צ
	"crg",         // 4. ערב
	"vkhk"         // 5. לילה
};

const char hodesh_list_s[15][6] = 
{ " ", 
	"hraw",  // 1. תשרי
	"iuuaj", // 2. חשוון
	"ukxf",  // 3. כסלו
	"wcy",   // 4. טבת
	"yca",   // 5. שבט
	"rst",   // 6. אדר
	"t rst", // 7. אדר א
	"c rst", // 8. אדר ב
	"ixhb",  // 9. ניסן
	"rhht",  // 10. אייר
	"iuuhx", // 11. סיוון
	"zunw",  // 12. תמוז
	"ct",    // 13. אב
	"kukt"   // 14. אלול
};

const char hodesh_list_l[15][7] = 
{ " ", 
	"hraw",  // 1. תשרי
	"iuuaj", // 2. חשוון
	"ukxf",  // 3. כסלו
	"wcy",   // 4. טבת
	"yca",   // 5. שבט
	"rst",   // 6. אדר
	"'t rst", // 7. אדר א
	"'c rst", // 8. אדר ב
	"ixhb",  // 9. ניסן
	"rhht",  // 10. אייר
	"iuuhx", // 11. סיוון
	"zunw",  // 12. תמוז
	"ct",    // 13. אב
	"kukt"   // 14. אלול
};

const char masechet_list[64][10] =
{ 
    " ",           
    "wufrc",      // 0. ברכות
    "vtp",        // 0. פאה
    "htns",       // 0. דמאי
    "ohtkf",      // 0. כלאים
    "whghca",     // 0. שביעית
    "wunurw",     // 0. תרומות
    "wuragn",     // 0. מעשרות
    "hba ragn",   // 0. מעשר שני
    "vkj",        // 0. חלה
    "vkrg",       // 0. ערלה
    "ohrufhc",    // 0. ביכורים
    "wca",        // 0. שבת
    "ihcurhg",    // 0. עירובין
    "ohjxp",      // 0. פסחים
    "ohkea",      // 0. שקלים
    "tnuh",       // 0. יומא
    "vfux",       // 0. סוכה
    "vmhc",       // 0. ביצה
    "vbav atr",   // 0. ראש השנה
    "whbgw",      // 0. תענית
    "vkhdn",      // 0. מגילה
    "iye sgun",   // 0. מועד קטן
    "vdhdj",      // 0. חגיגה
    "wunch",      // 0. יבמות
    "wucuwf",     // 0. כתובות
    "ohrsb",      // 0. נדרים
    "rhzb",       // 0. נזיר
    "vyux",       // 0. סוטה
    "ihyhd",      // 0. גיטין
    "ihaushe",    // 0. קידושין
    "tne tcc",    // 0. בבא קמא
    "tghmn tcc",  // 0. בבא מציעא
    "trwc tcc",   // 0. בבא בתרא
    "ihrsvbx",    // 0. סנהדרין
    "wufn",       // 0. מכות
    "wuguca",     // 0. שבועות
    "wuhsg",      // 0. עדיות
    "vrz vsucg",  // 0. עבודה זרה
    "wuct",       // 0. אבות
    "wuhruv",     // 0. הוריות
    "ohjcz",      // 0. זבחים
    "wujbn",      // 0. מנחות
    "ihkuj",      // 0. חולין
    "wurufc",     // 0. בכורות
    "ihfrg",      // 0. ערכין
    "vrunw",      // 0. תמורה
    "wuwhrf",     // 0. כריתות
    "vkhgn",      // 0. מעילה
    "shnw",       // 0. תמיד
    "wusn",       // 0. מדות
    "ohbe",       // 0. קנים
    "ohkf",       // 0. כלים
    "wukvut",     // 0. אוהלות
    "ohgdb",      // 0. נגעים
    "vrp",        // 0. פרה
    "wurvy",      // 0. טהרות
    "wutuen",     // 0. מקואות
    "vshb",       // 0. נידה
    "ihrhafn",    // 0. מכשירין
    "ohcz",       // 0. זבים
    "ouh kucy",   // 0. טבול יום
    "ohsh",       // 0. ידים
    "ohmeug"      // 0. עוקצים
};

const char ot_list[256][4] = 
{ " ", 
	"t",   //1. א
	"c",   //2. ב
	"d",   //3. ג
	"s",   //4. ד
	"v",   //5. ה
	"u",   //6. ו
	"z",   //7. ז
	"j",   //8. ח
	"y",   //9. ט
	"h",   //10. י
	"th",  //11. יא
	"ch",  //12. יב
	"dh",  //13. יג
	"sh",  //14. יד
	"uy",  //15. טו
	"zy",  //16. טז
	"zh",  //17. יז
	"jh",  //18. יח
	"yh",  //19. יט
	"f",   //20. כ
	"tf",  //21. כא
	"cf",  //22. כב
	"df",  //23. כג
	"sf",  //24. כד
	"vf",  //25. כה
	"uf",  //26. כו
	"zf",  //27. כז
	"jf",  //28. כח
	"yf",  //29. כט
	"k",   //30. ל
	"tk",  //31. לא
	"ck",  //32. לב
	"dk",  //33. לג
	"sk",  //34. לד
	"vk",  //35. לה
	"uk",  //36. לו
	"zk",  //37. לז
	"jk",  //38. לח
	"yk",  //39. לט
	"n",   //40. מ
	"tn",  //41. מא
	"cn",  //42. מב
	"dn",  //43. מג
	"sn",  //44. מד
	"vn",  //45. מה
	"un",  //46. מו
	"zn",  //47. מז
	"jn",  //48. מח
	"yn",  //49. מט
	"b",   //50. נ
	"tb",  //51. נא
	"cb",  //52. נב
	"db",  //53. נג
	"sb",  //54. נד
	"vb",  //55. נה
	"ub",  //56. נו
	"zb",  //57. נז
	"jb",  //58. נח
	"yb",  //59. נט
	"x",   //60. ס
	"tx",  //61. סא
	"cx",  //62. סב
	"dx",  //63. סג
	"sx",  //64. סד
	"vx",  //65. סה
	"ux",  //66. סו
	"zx",  //67. סז
	"jx",  //68. סח
	"yx",  //69. סט
	"g",   //70. ע
	"tg",  //71. עא
	"cg",  //72. עב
	"dg",  //73. עג
	"sg",  //74. עד
	"vg",  //75. עה
	"ug",  //76. עו
	"zg",  //77. עז
	"jg",  //78. עח
	"yg",  //79. עט
	"p",   //80. פ
	"tp",  //81. פא
	"cp",  //82. פב
	"dp",  //83. פג
	"sp",  //84. פד
	"vp",  //85. פה
	"up",  //86. פו
	"zp",  //87. פז
	"jp",  //88. פח
	"yp",  //89. פט
	"m",   //90. צ
	"tm",  //91. צא
	"cm",  //92. צב
	"dm",  //93. צג
	"sm",  //94. צד
	"vm",  //95. צה
	"um",  //96. צו
	"zm",  //97. צז
	"jm",  //98. צח
	"ym",  //99. צט
	"e",   //100. ק
	"te",  //101. קא
	"ce",  //102. קב
	"de",  //103. קג
	"se",  //104. קד
	"ve",  //105. קה
	"ue",  //106. קו
	"ze",  //107. קז
	"je",  //108. קח
	"ye",  //109. קט
	"he",  //110. קי
	"the", //111. קיא
	"che", //112. קיב
	"dhe", //113. קיג
	"she", //114. קיד
	"uye", //115. קטו
	"zye", //116. קטז
	"zhe", //117. קיז
	"jhe", //118. קיח
	"yhe", //119. קיט
	"fe",  //120. קכ
	"tfe", //121. קכא
	"cfe", //122. קכב
	"dfe", //123. קכג
	"sfe", //124. קכד
	"vfe", //125. קכה
	"ufe", //126. קכו
	"zfe", //127. קכז
	"jfe", //128. קכח
	"yfe", //129. קכט
	"ke",  //130. קל
	"tke", //131. קלא
	"cke", //132. קלב
	"dke", //133. קלג
	"ske", //134. קלד
	"vke", //135. קלה
	"uke", //136. קלו
	"zke", //137. קלז
	"jke", //138. קלח
	"yke", //139. קלט
	"ne",  //140. קמ
	"tne", //141. קמא
	"cne", //142. קמב
	"dne", //143. קמג
	"sne", //144. קמד
	"vne", //145. קמה
	"une", //146. קמו
	"zne", //147. קמז
	"jne", //148. קמח
	"yne", //149. קמט
	"be",  //150. קנ
	"tbe", //151. קנא
	"cbe", //152. קנב
	"dbe", //153. קנג
	"sbe", //154. קנד
	"vbe", //155. קנה
	"ube", //156. קנו
	"zbe", //157. קנז
	"jbe", //158. קנח
	"ybe", //159. קנט
	"xe",  //160. קס
	"txe", //161. קסא
	"cxe", //162. קסב
	"dxe", //163. קסג
	"sxe", //164. קסד
	"vxe", //165. קסה
	"uxe", //166. קסו
	"zxe", //167. קסז
	"jxe", //168. קסח
	"yxe", //169. קסט
	"ge",  //170. קע
	"tge", //171. קעא
	"cge", //172. קעב
	"dge", //173. קעג
	"sge", //174. קעד
	"vge", //175. קעה
	"uge", //176. קעו
	"zge", //177. קעז
	"jge", //178. קעח
	"yge", //179. קעט
	"pe",  //180. קפ
	"tpe", //181. קפא
	"cpe", //182. קפב
	"dpe", //183. קפג
	"spe", //184. קפד
	"vpe", //185. קפה
	"upe", //186. קפו
	"zpe", //187. קפז
	"jpe", //188. קפח
	"ype", //189. קפט
	"me",  //190. קצ
	"tme", //191. קצא
	"cme", //192. קצב
	"dme", //193. קצג
	"sme", //194. קצד
	"vme", //195. קצה
	"ume", //196. קצו
	"zme", //197. קצז
	"jme", //198. קצח
	"yme", //199. קצט
	"r",   //200. ר
	"tr",  //201. רא
	"cr",  //202. רב
	"dr",  //203. רג
	"sr",  //204. רד
	"vr",  //205. רה
	"ur",  //206. רו
	"zr",  //207. רז
	"jr",  //208. רח
	"yr",  //209. רט
	"hr",  //210. רי
	"thr", //211. ריא
	"chr", //212. ריב
	"dhr", //213. ריג
	"shr", //214. ריד
	"uyr", //215. רטו
	"zyr", //216. רטז
	"zhr", //217. ריז
	"jhr", //218. ריח
	"yhr", //219. ריט
	"fr",  //220. רכ
	"tfr", //221. רכא
	"cfr", //222. רכב
	"dfr", //223. רכג
	"sfr", //224. רכד
	"vfr", //225. רכה
	"ufr", //226. רכו
	"zfr", //227. רכז
	"jfr", //228. רכח
	"yfr", //229. רכט
	"kr",  //230. רל
	"tkr", //231. רלא
	"ckr", //232. רלב
	"dkr", //233. רלג
	"skr", //234. רלד
	"vkr", //235. רלה
	"ukr", //236. רלו
	"zkr", //237. רלז
	"jkr", //238. רלח
	"ykr", //239. רלט
	"nr",  //240. רמ
	"tnr", //241. רמא
	"cnr", //242. רמב
	"dnr", //243. רמג
	"snr", //244. רמד
	"vnr", //245. רמה
	"unr", //246. רמו
	"znr", //247. רמז
	"jnr", //248. רמח
	"ynr", //249. רמט
	"br",  //250. רנ
	"tbr", //251. רנא
	"cbr", //252. רנב
	"dbr", //253. רנג
	"sbr", //254. רנד
	"vbr"  //255. רנה
};

const char yom_list[31][4] = 
{ " ", 
	"'t",    //1. א
	"'c",    //2. ב
	"'d",    //3. ג
	"'s",    //4. ד
	"'v",    //5. ה
	"'u",    //6. ו
	"'z",    //7. ז
	"'j",    //8. ח
	"'y",    //9. ט
	"'h",    //10. י
	"t\"h",  //11. יא
	"c\"h",  //12. יב
	"d\"h",  //13. יג
	"s\"h",  //14. יד
	"u\"y",  //15. טו
	"z\"y",  //16. טז
	"z\"h",  //17. יז
	"j\"h",  //18. יח
	"y\"h",  //19. יט
	"'f",    //20. כ
	"t\"f",  //21. כא
	"c\"f",  //22. כב
	"d\"f",  //23. כג
	"s\"f",  //24. כד
	"v\"f",  //25. כה
	"u\"f",  //26. כו
	"z\"f",  //27. כז
	"j\"f",  //28. כח
	"y\"f",  //29. כט
	"'k",    //30. ל
};

const char parasha_list[78][19] = 
{"",
	"vbav atrs t",            //1. א דראש השנה
	"vbav atrs c",            //2. ב דראש השנה
	"lkhu",                   //3. וילך
	"ubhztv",                 //4. האזינו
	"ruphf ouh",              //5. יום כיפור
	"n\"vuj wca",             //6. שבת חוה"מ
	"wufux",                  //7. סוכות
	"wufux ka hba y\"uh",     //8. יו"ט שני של סוכות
	"wufux n\"vuj",           //9. חוה"מ סוכות
	"wrmg hbhna",             //10. שמיני עצרת
	"vfrcv wtzu",             //11. וזאת הברכה
	"whatrc",                 //12. בראשית
	"jb",                     //13. נח
	"lk lk",                  //14. לך לך
	"trhu",                   //15. וירא
	"vra hhj",                //16. חיי שרה
	"wuskuw",                 //17. תולדות
	"tmhu",                   //18. ויצא
	"jkahu",                  //19. וישלח
	"cahu",                   //20. וישב
	"_en",                    //21. מקץ
	"adhu",                   //22. ויגש
	"hjhu",                   //23. ויחי
	"wuna",                   //24. שמות
	"trtu",                   //25. וארא
	"tc",                     //26. בא
	"jkac",                   //27. בשלח
	"urwh",                   //28. יתרו
	"ohypan",                 //29. משפטים
	"vnurw",                  //30. תרומה
	"vumw",                   //31. תצוה
	"taw hf",                 //32. כי תשא
	"hsuep kvehu",            //33. ויקהל פקודי
	"trehu",                  //34. ויקרא
	"um",                     //35. צו
	"jxp",                    //36. פסח
	"jxp ka hba y\"uh",       //37. יו"ט שני של פסח
	"jxp n\"vuj",             //38. חוה"מ פסח
	"jxp ka hghca",           //39. שביעי של פסח
	"jxp ka hbhna",           //40. שמיני של פסח
	"hbhna",                  //41. שמיני
	"grumn ghrzw",            //42. תזריע מצורע
	"ohause hrjt",            //43. אחרי קדושים
	"runt",                   //44. אמור
	"hwueujc rvc",            //45. בהר בחוקותי
	"rcsnc",                  //46. במדבר
	"wuguca ka hba y\"uh",    //47. יו"ט שני של שבועות
	"wuguca",                 //48. שבועות
	"tab",                    //49. נשא
	"lwukgvc",                //50. בהעלותך
	"jka",                    //51. שלח
	"jre",                    //52. קרח
	"ekc wej",                //53. חקת בלק
	"xjbhp",                  //54. פינחס
	"wuyn",                   //55. מטות
	"hgxn",                   //56. מסעי
	"hgxn wuyn",              //57. מטות מסעי
	"ohrcs",                  //58. דברים
	"ibjwtu",                 //59. ואתחנן
	"ctc vgaw",               //60. תשעה באב
	"ceg",                    //61. עקב
	"vtr",                    //62. ראה
	"ohypua",                 //63. שופטים
	"tmw hf",                 //64. כי תצא
	"tucw hf",                //65. כי תבוא
	"lkhu ohcmhb",            //66. ניצבים וילך
	"kvehu",                  //67. ויקהל
	"hsuep",                  //68. פקודי
	"ghrzw",                  //69. תזריע
	"grumn",                  //70. מצורע
	"wun hrjt",               //71. אחרי מות
	"ohause",                 //72. קדושים
	"rvc",                    //73. בהר
	"hwueujc",                //74. בחוקותי
	"wej",                    //75. חקת
	"ekc",                    //76. בלק
	"ohcmhb",                 //77. ניצבים
};

const char hafrara_shem_list[104][16] = 
{
	"ktv rnt vf",           //0. כה אמר האל
	"vreg hbr",             //1. רני עקרה
	"rntw vnk",             //2. למה תאמר
	"wjt vatu",             //3. ואשה אחת
	"sus lknvu",            //4. והמלך דוד
	"rcs tan",              //5. משא דבר
	"ohtukw hngu",          //6. ועמי תלואים
	"vhscug iuzj",          //7. חזון עובדיה
	"\'v rnt vf",           //8. כה אמר ה'
	"vnka _ehu",            //9. ויקץ שלמה
	"\'v rcs hvhu",         //10. ויהי דבר ה'
	"hnh ucrehu",           //11. ויקרבו ימי
	"uvhnrh hrcs",          //12. דברי ירמיהו
	"rnt vf",               //13. כה אמר
	"rat rcsv",             //14. הדבר אשר
	"vrucs rawu",           //15. ותשר דבורה
	"wun wbac",             //16. בשנת מות
	"rat rcsv",             //17. הדבר אשר
	"iwb \'vu",             //18. וה' נתן
	"ost ic vwt",           //19. אתה בן אדם
	"ctjt jkahu",           //20. וישלח אחאב
	"lknv jkahu",           //21. וישלח המלך
	"ourhj aghu",           //22. ויעש חירום
	"hwrmh uz og",          //23. עם זו יצרתי
	"\'v rnt vf",           //24. כה אמר ה'
	"sus sug qxhu",         //25. ויסף עוד דוד
	"tc ahtu",              //26. ואיש בא
	"ohabt vgcrtu",         //27. וארבעה אנשים
	"\'v rcs hvhu",         //28. ויהי דבר ה'
	"\'v rcs hvhu",         //29. ויהי דבר ה'
	"ohukv ohbvfvu",        //30. והכהנים הלוים
	"uvhnrh rnthu",         //31. ויאמר ירמיהו
	"hzgnu hzg \'v",        //32. ה' עזי ומעזי
	"rpxn vhvu",            //33. והיה מספר
	"sjt aht hvhu",         //34. ויהי איש אחד
	"hjnau hbr",            //35. רני ושמחי
	"gavh jkahu",           //36. וישלח יהשע
	"ktuna rnthu",          //37. ויאמר שמואל
	"hsgkdv jwphu",         //38. ויפתח הגלעדי
	"whrta vhvu",           //39. והיה שארית
	"vwhv \'v shu",         //40. ויד ה' היתה
	"\'v rcs ugna",         //41. שמעו דבר ה'
	"uvhgah iuzj",          //42. חזון ישעיהו
	"unjb unjb",            //43. נחמו נחמו
	"iuhm rntwu",           //44. ותאמר ציון
	"vrgx vhbg",            //45. עניה סערה
	"hfbt hfbt",            //46. אנכי אנכי
	"hrut hnue",            //47. קומי אורי
	"ahat aua",             //48. שוש אשיש
	"ktrah vcua",           //49. שובה ישראל
	"sus rcshu",            //50. וידבר דוד
	"htxf ohnav",           //51. השמים כסאי
	"asuj rjn",             //52. מחר חודש
	"gshuvh wrfhu",         //53. ויכרת יהוידע
	"ktuna rnthu",          //54. ויאמר שמואל
	"\'v rcs hvhu",         //55. ויהי דבר ה'
	"\'v rnt vf",           //56. כה אמר ה'
	"\'vk vcrgu",           //57. וערבה לה'
	"thvv wgc",             //58. בעת ההיא
	"lknv jkahu",           //59. וישלח המלך
	"hkg vwhv",             //60. היתה עלי
	"cbc ouhv sug",         //61. עוד היום בנב
	"ohakac hvhu",          //62. ויהי בשלשים
	"kfhvc \'vu",           //63. וה' בהיכל
	"ophxt qxt",            //64. אסף אסיפם
	"sjt aht hvhu",         //65. ויהי איש אחד
	"\'v rnt vf",           //66. כה אמר ה'
	"ukx rntu",             //67. ואמר סלו
	"\'v rcs hvhu",         //68. ויהי דבר ה'
	"tc ouh vbv",           //69. הנה יום בא
	"kt ukvehu",            //70. ויקהלו אל
	"ouhc hvhu",            //71. ויהי ביום
	"wukff hvhu",           //72. ויהי ככלות
	"wun hrjt hvhu",        //73. ויהי אחרי מות
	"\'v uars",             //74. דרשו ה'
	"cegh jrchu",           //75. ויברח יעקב
	"arah ohtcv",           //76. הבאים ישרש
	"vat vrucsu",           //77. ודבורה אשה
	"ohnh hvhu",            //78. ויהי ימים
	"kf okawu",             //79. ותשלם כל
	"gahkt rnthu",          //80. ויאמר אלישע
	"ohhaf hbcf tukv",      //81. הלוא כבני כשיים
	"ohba gca ic",          //82. בן שבע שנים
	"\'v rnt vf",           //83. כה אמר ה'
	"_rtv og kf",           //84. כל עם הארץ
	"euecjk vkhpw",         //85. תפילה לחבקוק
	"hscg iv",              //86. הן עבדי
	"hbuhnsw hn ktu",       //87. ואל מי תדמיוני
	"\'v rcs hvhu",         //88. ויהי דבר ה'
	"sug vhvh tku",         //89. ולא יהיה עוד
	"ohrmn tan",            //90. משא מצרים
	"ohvkt gbfhu",          //91. ויכנע אלהים
	"vbac hvhu",            //92. ויהי בשנה
	"\'v rcs hvhu",         //93. ויהי דבר ה'
	"vwhv vfht",            //94. איכה היתה
	"ohudc gsubu",          //95. ונודע בגוים
	"\'v rnt vf",           //96. כה אמר ה'
	"vnjknv hvwu",          //97. ותהי המלחמה
	"\'v rnt vf",           //98. כה אמר ה'
	"vba vbuna ic",         //99. בן שמונה שנה
	"\'v rnt vf",           //100. כה אמר ה'
	"\'v rnt vf",           //101. כה אמר ה'
	"wt hwtcvu",            //102. והבאתי את
	"\'v rcs hvhu",         //103. ויהי דבר ה'
};

const char hafrara_makor_list[104][16] = 
{
	"\'v c\"n vhgah",       //0. ישעיה מ"ב ה'
	"\'t s\"b vhgah",       //1. ישעיה נ"ד א'
	"z\"f \'n vhgah",       //2. ישעיה מ' כ"ז
	"\'t \'s c ohfkn",      //3. מלכים ב ד' א'
	"\'t t ohfkn",          //4. מלכים א א'
	"\'t \'t hftkn",        //5. מלאכי א' א'
	"\'z t\"h gauv",        //6. הושע י"א ז'
	"\'t \'t vhscug",       //7. עובדיה א' א'
	"\'u \'c xung",         //8. עמוס ב' ו'
	"u\"y \'d t ohfkn",     //9. מלכים א ג' ט"ו
	"u\"y z\"k ktezjh",     //10. יחזקאל ל"ז ט"ו
	"\'t \'c t ohfkn",      //11. מלכים א ב' א'
	"\'t \'t vhnrh",        //12. ירמיה א' א'
	"v\"f j\"f ktezjh",     //13. יחזקאל כ"ח כ"ה
	"d\"h u\"n vhnrh",      //14. ירמיה מ"ו י"ג
	"\'t \'v ohypua",       //15. שופטים ה' א'
	"\'t \'u vhgah",        //16. ישעיה ו' א'
	"\'j s\"k vhnrh",       //17. ירמיה ל"ד ח'
	"u\"f \'v t ohfkn",     //18. מלכים א ה' כ"ו
	"\'h d\"n ktezjh",      //19. יחזקאל מ"ג י'
	"\'f j\"h t ohfkn",     //20. מלכים א י"ח כ'
	"d\"h \'z t ohfkn",     //21. מלכים א ז' י"ג
	"\'n \'z t ohfkn",      //22. מלכים א ז' מ'
	"t\"f d\"n vhgah",      //23. ישעיה מ"ג כ"א
	"t\"f \'z vhnrh",       //24. ירמיה ז' כ"א
	"\'t \'u c ktuna",      //25. שמואל ב ו' א'
	"c\"n \'s c ohfkn",     //26. מלכים ב ד' מ"ב
	"\'d \'z c ohfkn",      //27. מלכים ב ז' ג'
	"\'t c\"f ktezjh",      //28. יחזקאל כ"ב א'
	"\'c \'f ktezjh",       //29. יחזקאל כ' ב'
	"u\"y s\"n ktezjh",     //30. יחזקאל מ"ד ט"ו
	"\'u c\"k vhnrh",       //31. ירמיה ל"ב ו'
	"y\"h z\"y vhnrh",      //32. ירמיה ט"ז י"ט
	"\'t \'c gauv",         //33. הושע ב' א'
	"\'c d\"h ohypua",      //34. שופטים י"ג ב'
	"s\"h \'c vhrfz",       //35. זכריה ב' י"ד
	"\'t \'c gauvh",        //36. יהושע ב' א'
	"s\"h t\"h t ktuna",    //37. שמואל א י"א י"ד
	"\'t t\"h ohypua",      //38. שופטים י"א א'
	"\'u \'v vfhn",         //39. מיכה ה' ו'
	"u\"y j\"h t ohfkn",    //40. מלכים א י"ח ט"ו
	"\'s \'c vhnrh",        //41. ירמיה ב' ד'
	"\'t \'t vhgah",        //42. ישעיה א' א'
	"\'t \'n vhgah",        //43. ישעיה מ' א'
	"s\"h y\"n vhgah",      //44. ישעיה מ"ט י"ד
	"t\"h s\"b vhgah",      //45. ישעיה נ"ד י"א
	"c\"h t\"b vhgah",      //46. ישעיה נ"א י"ב
	"\'t \'x vhgah",        //47. ישעיה ס' א'
	"\'h t\"x vhgah",       //48. ישעיה ס"א י'
	"\'c s\"h gauv",        //49. הושע י"ד ב'
	"\'t c\"f c ktuna",     //50. שמואל ב כ"ב א'
	"\'t u\"x vhgah",       //51. ישעיה ס"ו א'
	"j\"h \'f t ktuna",     //52. שמואל א כ' י"ח
	"z\"h t\"h c ohfkn",    //53. מלכים ב י"א י"ז
	"\'t u\"y t ktuna",     //54. שמואל א ט"ו א'
	"z\"y u\"k ktezjh",     //55. יחזקאל ל"ו ט"ז
	"j\"h v\"n ktezjh",     //56. יחזקאל מ"ה י"ח
	"\'s \'d hftkn",        //57. מלאכי ג' ד'
	"\'c \'v gauvh",        //58. יהושע ה' ב'
	"\'t d\"f c ohfkn",     //59. מלכים ב כ"ג א'
	"\'t z\"k ktezjh",      //60. יחזקאל ל"ז א'
	"c\"k \'h vhgah",       //61. ישעיה י' ל"ב
	"\'t \'t ktezjh",       //62. יחזקאל א' א'
	"\'f \'c euecj",        //63. חבקוק ב' כ'
	"d\"h \'j vhnrh",       //64. ירמיה ח' י"ג
	"\'t \'t t ktuna",      //65. שמואל א א' א'
	"\'t t\"k vhnrh",       //66. ירמיה ל"א א'
	"s\"h z\"b vhgah",      //67. ישעיה נ"ז י"ד
	"\'t \'t vbuh",         //68. יונה א' א'
	"\'t s\"h vhrfz",       //69. זכריה י"ד א'
	"\'c \'j t ohfkn",      //70. מלכים א ח' ב'
	"j\"h j\"k ktezjh",     //71. יחזקאל ל"ח י"ח
	"s\"b \'j t ohfkn",     //72. מלכים א ח' נ"ד
	"\'t \'t gauvh",        //73. יהושע א' א'
	"\'u v\"b vhgah",       //74. ישעיה נ"ה ו'
	"d\"h c\"h gauv",       //75. הושע י"ב י"ג
	"\'u z\"f vhgah",       //76. ישעיה כ"ז ו'
	"\'s \'s ohypua",       //77. שופטים ד' ד'
	"\'t j\"h t ohfkn",     //78. מלכים א י"ח א'
	"t\"b \'z t ohfkn",     //79. מלכים א ז' נ"א
	"\'t \'z c ohfkn",      //80. מלכים ב ז' א'
	"\'z \'y xung",         //81. עמוס ט' ז'
	"\'t c\"h c ohfkn",     //82. מלכים ב י"ב א'
	"\'c u\"y t ktuna",     //83. שמואל א ט"ו ב'
	"z\"y v\"n ktezjh",     //84. יחזקאל מ"ה ט"ז
	"\'t \'d euecj",        //85. חבקוק ג' א'
	"\'t c\"n vhgah",       //86. ישעיה מ"ב א'
	"v\"f \'n vhgah",       //87. ישעיה מ' כ"ה
	"\'t z\"y ktezjh",      //88. יחזקאל ט"ז א'
	"s\"f j\"f ktezjh",     //89. יחזקאל כ"ח כ"ד
	"\'t y\"h vhgah",       //90. ישעיה י"ט א'
	"d\"f \'s ohypua",      //91. שופטים ד' כ"ג
	"\'t \'f ktezjh",       //92. יחזקאל כ' א'
	"\'t s\"k ktezjh",      //93. יחזקאל ל"ד א'
	"t\"f \'t vhgah",       //94. ישעיה א' כ"א
	"\'y t\"x vhgah",       //95. ישעיה ס"א ט'
	"c\"f z\"h ktezjh",     //96. יחזקאל י"ז כ"ב
	"c\"b s\"h t ktuna",    //97. שמואל א י"ד נ"ב
	"\'y v\"n ktezjh",      //98. יחזקאל מ"ה ט'
	"\'t c\"f c ohfkn",     //99. מלכים ב כ"ב א'
	"z\"k u\"k ktezjh",     //100. יחזקאל ל"ו ל"ז
	"z\"y \'u vhnrh",       //101. ירמיה ו' ט"ז
	"\'y d\"h vhrfz",       //102. זכריה י"ג ט'
	"\'t j\"k ktezjh",      //103. יחזקאל ל"ח א'
};

const char holidays[49][17] = 
{"",
	"vbav atr",          //1. ראש השנה
	"vhksd oum",         //2. צום גדליה
	"ruphf ouh crg",     //3. ערב יום כיפור
	"ruphf ouh",         //4. יום כיפור
	"wufux crg",         //5. ערב סוכות
	"wufux",             //6. סוכות
	"n\"vujs t",         //7. א דחוה''מ
	"n\"vujs c",         //8. ב דחוה''מ
	"n\"vujs d",         //9. ג דחוה''מ
	"n\"vujs s",         //10. ד דחוה''מ
	"n\"vujs v",         //11. ה דחוה''מ
	"vcr tbgauv",        //12. הושענא רבה
	"vruw wjna",         //13. שמחת תורה
	"iuatr",             //14. ראשון
	"hba",               //15. שני
	"hahka",             //16. שלישי
	"hghcr",             //17. רביעי
	"hahnj",             //18. חמישי
	"haha",              //19. שישי
	"hghca",             //20. שביעי
	"hbhna",             //21. שמיני
	"wcyc vrag",         //22. עשרה בטבת
	"wubkhtk vbav atr",  //23. ראש השנה לאילנות
	"rwxt whbgw",        //24. תענית אסתר
	"ohrup",             //25. פורים
	"ohrup iaua",        //26. שושן פורים
	"jxp crg",           //27. ערב פסח
	"jxp",               //28. פסח
	"jxp ka hghca",      //29. שביעי של פסח
	"iurfzv ouh",        //30. יום הזכרון
	"wutnmgv ouh",       //31. יום העצמאות
	"rnugc d\"k crg",    //32. ערב ל"ג בעומר
	"rnugc d\"k",        //33. ל"ג בעומר
	"ohkaurh ouh",       //34. יום ירושלים
	"wuguca crg",        //35. ערב שבועות
	"wuguca",            //36. שבועות
	"zunwc z\"h",        //37. י''ז בתמוז
	"ctc vgaw crg",      //38. ערב תשעה באב
	"ctc vgaw",          //39. תשעה באב
	"vbav atr crg",      //40. ערב ראש השנה
	"iye ohrup",         //41. פורים קטן
	"iye ohrup iaua",    //42. שושן פורים קטן
	"akuan ohrup",       //43. פורים משולש
	"iurfhzv ouh",       //44. יום הזיכרון (יום השואה)
	"hba jxp",           //45. פסח שני
	"ctc u\"y",          //46. ט"ו באב
	"wrmg hbhna",        //47. שמיני עצרת
	"jxp ka hbhna",      //48. שמיני של פסח
};

const char shabat_plus_list[17][12] = 
{ " ", 
	"t wuct herp",  //1. פרקי אבות א
	"c wuct herp",  //2. פרקי אבות ב
	"d wuct herp",  //3. פרקי אבות ג
	"s wuct herp",  //4. פרקי אבות ד
	"v wuct herp",  //5. פרקי אבות ה
	"u wuct herp",  //6. פרקי אבות ו
	"ohkea wca",    //7. שבת שקלים
	"vrp wca",      //8. שבת פרה
	"asujv wca",    //9. שבת החודש
	"vfubj wca",    //10. שבת חנוכה
	"rufz wca",     //11. שבת זכור
	"kusdv wca",    //12. שבת הגדול
	"iuzj wca",     //13. שבת חזון
	"unjb wca",     //14. שבת נחמו
	"vcuaw wca",    //15. שבת תשובה
	"vrha wca",     //16. שבת שירה
};

const char heb_map[27] = 
{
	't',  // א
	'c',  // ב
	'd',  // ג
	's',  // ד
	'v',  // ה
	'u',  // ו
	'z',  // ז
	'j',  // ח
	'y',  // ט
	'h',  // י
	'l',  // ך
	'f',  // כ
	'k',  // ל
	'o',  // ם
	'n',  // מ
	'i',  // ן
	'b',  // נ
	'x',  // ס
	'g',  // ע
	'q',  // ף
	'p',  // פ
	'_',  // ץ
	'm',  // צ
	'e',  // ק
	'r',  // ר
	'a',  // ש
	'w',  // ת
};

const char omer_words[54][7]
{
	"",        // 0. 
	"sj",      // 1. חד
	"ihrw",    // 2. תרין
	"twkw",    // 3. תלתא
	"tgcRt",   // 4. ארבעא
	"tanj",    // 5. חמשא
	"twa",     // 6. שתא
	"tgca",    // 7. שבעא
	"thbnw",   // 8. תמניא
	"tgaw",    // 9. תשעא
	"trag",    // 10. עשרא
	"rag",     // 11. עשר
	"hrw",     // 12. תרי
	"wkw",     // 13. תלת
	"wgcRt",   // 14. ארבעת
	"wanj",    // 15. חמשת
	"wwa",     // 16. שתת
	"wgca",    // 17. שבעת
	"wbnw",    // 18. תמנת
	"wgaw",    // 19. תשעת
	"ihrag",   // 20. עשרין
	"ihwkw",   // 21. תלתין
	"ihgcRt",  // 22. ארבעין
	"tbshtv",  // 23. האידנא
	"hnuh",    // 24. יומי
	"trnugc",  // 25. בעומרא
	"iubts",   // 26. דאנון
	"tguca",   // 27. שבועא
	"tnuh",    // 28. יומא
	"hguca",   // 29. שבועי
	"hnka",    // 30. שלמי
	"sjt",     // 31. אחד
	"ohba",    // 32. שנים
	"vauka",   // 33. שלושה
	"vgcRt",   // 34. ארבעה
	"vahnj",   // 35. חמישה
	"vaha",    // 36. שישה
	"vgca",    // 37. שבעה
	"vbuna",   // 38. שמונה
	"vgaw",    // 39. תשעה
	"vrag",    // 40. עשרה
	"rnugc",   // 41. בעומר
	"hba",     // 42. שני
	"ohrag",   // 43. עשרים
	"ohauka",  // 44. שלושים
	"ohgcRt",  // 45. ארבעים
	"ouhv",    // 46. היום
	"ohnh",    // 47. ימים
	"rnugk",   // 48. לעומר
	"ova",     // 49. שהם
	"guca",    // 50. שבוע
	"ouh",     // 51. יום
	"wuguca",  // 52. שבועות
	"u",       // 53. ו
};
/*
const char omer_list_teiman[98][33]
{
	"trnugc tnuh sj tbshtv",            // האידנא חד יומא בעומרא
	"",                                 // 
	"trnugc hnuh ihrw tbshtv",          // האידנא תרין יומי בעומרא
	"",                                 // 
	"trnugc hnuh twkw tbshtv",          // האידנא תלתא יומי בעומרא
	"",                                 // 
	"trnugc hnuh tgcrt tbshtv",         // האידנא ארבעא יומי בעומרא
	"",                                 // 
	"trnugc hnuh tanj tbshtv",          // האידנא חמשא יומי בעומרא
	"",                                 // 
	"trnugc hnuh twa tbshtv",           // האידנא שתא יומי בעומרא
	"",                                 // 
	"trnugc hnuh tgca tbshtv",          // האידנא שבעא יומי בעומרא
	"tguca sj iubhts",                  // דאינון חד שבועא
	"trnugc hnuh thbnw tbshtv",         // האידנא תמניא יומי בעומרא
	"tnuh sju tguca sj iubhts",         // דאינון חד שבועא וחד יומא
	"trnugc hnuh tgaw tbshtv",          // האידנא תשעא יומי בעומרא
	"hnuh ihrwu tguca sj iubhts",       // דאינון חד שבועא ותרין יומי
	"trnugc hnuh trag tbshtv",          // האידנא עשרא יומי בעומרא
	"hnuh twkwu tguca sj iubhts",       // דאינון חד שבועא ותלתא יומי
	"trnugc hnuh rag sj tbshtv",        // האידנא חד עשר יומי בעומרא
	"hnuh tgcrtu tguca sj iubhts",      // דאינון חד שבועא וארבעא יומי
	"trnugc hnuh rag hrw tbshtv",       // האידנא תרי עשר יומי בעומרא
	"hnuh tanju tguca sj iubhts",       // דאינון חד שבועא וחמשא יומי
	"trnugc hnuh rag wkw tbshtv",       // האידנא תלת עשר יומי בעומרא
	"hnuh twau tguca sj iubhts",        // דאינון חד שבועא ושתא יומי
	"trnugc hnuh rag wgcrt tbshtv",     // האידנא ארבעת עשר יומי בעומרא
	"hguca ihrw iubhts",                // דאינון תרין שבועי
	"trnugc hnuh rag wanj tbshtv",      // האידנא חמשת עשר יומי בעומרא
	"tnuh sju hguca ihrw iubhts",       // דאינון תרין שבועי וחד יומא
	"trnugc hnuh rag wwa tbshtv",       // האידנא שתת עשר יומי בעומרא
	"hnuh ihrwu hguca ihrw iubhts",     // דאינון תרין שבועי ותרין יומי
	"trnugc hnuh rag wgca tbshtv",      // האידנא שבעת עשר יומי בעומרא
	"hnuh twkwu hguca ihrw iubhts",     // דאינון תרין שבועי ותלתא יומי
	"trnugc hnuh rag wbnw tbshtv",      // האידנא תמנת עשר יומי בעומרא
	"hnuh tgcrtu hguca ihrw iubhts",    // דאינון תרין שבועי וארבעא יומי
	"trnugc hnuh rag wgaw tbshtv",      // האידנא תשעת עשר יומי בעומרא
	"hnuh tanju hguca ihrw iubhts",     // דאינון תרין שבועי וחמשא יומי
	"trnugc hnuh ihrag tbshtv",         // האידנא עשרין יומי בעומרא
	"hnuh twau hguca ihrw iubhts",      // דאינון תרין שבועי ושתא יומי
	"trnugc hnuh sju ihrag tbshtv",     // האידנא עשרין וחד יומי בעומרא
	"hguca twkw iubhts",                // דאינון תלתא שבועי
	"trnugc hnuh ihrwu ihrag tbshtv",   // האידנא עשרין ותרין יומי בעומרא
	"tnuh sju hguca twkw iubhts",       // דאינון תלתא שבועי וחד יומא
	"trnugc hnuh twkwu ihrag tbshtv",   // האידנא עשרין ותלתא יומי בעומרא
	"hnuh ihrwu hguca twkw iubhts",     // דאינון תלתא שבועי ותרין יומי
	"trnugc hnuh tgcrtu ihrag tbshtv",  // האידנא עשרין וארבעא יומי בעומרא
	"hnuh twkwu hguca twkw iubhts",     // דאינון תלתא שבועי ותלתא יומי
	"trnugc hnuh tanju ihrag tbshtv",   // האידנא עשרין וחמשא יומי בעומרא
	"hnuh tgcrtu hguca twkw iubhts",    // דאינון תלתא שבועי וארבעא יומי
	"trnugc hnuh twau ihrag tbshtv",    // האידנא עשרין ושתא יומי בעומרא
	"hnuh tanju hguca twkw iubhts",     // דאינון תלתא שבועי וחמשא יומי
	"trnugc hnuh tgcau ihrag tbshtv",   // האידנא עשרין ושבעא יומי בעומרא
	"hnuh twau hguca twkw iubhts",      // דאינון תלתא שבועי ושתא יומי
	"trnugc hnuh thbnwu ihrag tbshtv",  // האידנא עשרין ותמניא יומי בעומרא
	"hguca tgcrt iubhts",               // דאינון ארבעא שבועי
	"trnugc hnuh tgawu ihrag tbshtv",   // האידנא עשרין ותשעא יומי בעומרא
	"tnuh sju hguca tgcrt iubhts",      // דאינון ארבעא שבועי וחד יומא
	"trnugc hnuh ihwkw tbshtv",         // האידנא תלתין יומי בעומרא
	"hnuh ihrwu hguca tgcrt iubhts",    // דאינון ארבעא שבועי ותרין יומי
	"trnugc hnuh sju ihwkw tbshtv",     // האידנא תלתין וחד יומי בעומרא
	"hnuh twkwu hguca tgcrt iubhts",    // דאינון ארבעא שבועי ותלתא יומי
	"trnugc hnuh ihrwu ihwkw tbshtv",   // האידנא תלתין ותרין יומי בעומרא
	"hnuh tgcrtu hguca tgcrt iubhts",   // דאינון ארבעא שבועי וארבעא יומי
	"trnugc hnuh twkwu ihwkw tbshtv",   // האידנא תלתין ותלתא יומי בעומרא
	"hnuh tanju hguca tgcrt iubhts",    // דאינון ארבעא שבועי וחמשא יומי
	"trnugc hnuh tgcrtu ihwkw tbshtv",  // האידנא תלתין וארבעא יומי בעומרא
	"hnuh twau hguca tgcrt iubhts",     // דאינון ארבעא שבועי ושתא יומי
	"trnugc hnuh tanju ihwkw tbshtv",   // האידנא תלתין וחמשא יומי בעומרא
	"hguca tanj iubhts",                // דאינון חמשא שבועי
	"trnugc hnuh twau ihwkw tbshtv",    // האידנא תלתין ושתא יומי בעומרא
	"tnuh sju hguca tanj iubhts",       // דאינון חמשא שבועי וחד יומא
	"trnugc hnuh tgcau ihwkw tbshtv",   // האידנא תלתין ושבעא יומי בעומרא
	"hnuh ihrwu hguca tanj iubhts",     // דאינון חמשא שבועי ותרין יומי
	"trnugc hnuh thbnwu ihwkw tbshtv",  // האידנא תלתין ותמניא יומי בעומרא
	"hnuh twkwu hguca tanj iubhts",     // דאינון חמשא שבועי ותלתא יומי
	"trnugc hnuh tgawu ihwkw tbshtv",   // האידנא תלתין ותשעא יומי בעומרא
	"hnuh tgcrtu hguca tanj iubhts",    // דאינון חמשא שבועי וארבעא יומי
	"trnugc hnuh ihgcrt tbshtv",        // האידנא ארבעין יומי בעומרא
	"hnuh tanju hguca tanj iubhts",     // דאינון חמשא שבועי וחמשא יומי
	"trnugc hnuh sju ihgcrt tbshtv",    // האידנא ארבעין וחד יומי בעומרא
	"hnuh twau hguca tanj iubhts",      // דאינון חמשא שבועי ושתא יומי
	"trnugc hnuh ihrwu ihgcrt tbshtv",  // האידנא ארבעין ותרין יומי בעומרא
	"hguca twa iubhts",                 // דאינון שתא שבועי
	"trnugc hnuh twkwu ihgcrt tbshtv",  // האידנא ארבעין ותלתא יומי בעומרא
	"tnuh sju hguca twa iubhts",        // דאינון שתא שבועי וחד יומא
	"trnugc hnuh tgcrtu ihgcrt tbshtv", // האידנא ארבעין וארבעא יומי בעומרא
	"hnuh ihrwu hguca twa iubhts",      // דאינון שתא שבועי ותרין יומי
	"trnugc hnuh tanju ihgcrt tbshtv",  // האידנא ארבעין וחמשא יומי בעומרא
	"hnuh twkwu hguca twa iubhts",      // דאינון שתא שבועי ותלתא יומי
	"trnugc hnuh twau ihgcrt tbshtv",   // האידנא ארבעין ושתא יומי בעומרא
	"hnuh tgcrtu hguca twa iubhts",     // דאינון שתא שבועי וארבעא יומי
	"trnugc hnuh tgcau ihgcrt tbshtv",  // האידנא ארבעין ושבעא יומי בעומרא
	"hnuh tanju hguca twa iubhts",      // דאינון שתא שבועי וחמשא יומי
	"trnugc hnuh thbnwu ihgcrt tbshtv", // האידנא ארבעין ותמניא יומי בעומרא
	"hnuh twau hguca twa iubhts",       // דאינון שתא שבועי ושתא יומי
	"trnugc hnuh tgawu ihgcrt tbshtv",  // האידנא ארבעין ותשעא יומי בעומרא
	"hnka hguca tgca iubhts"            // דאינון שבעא שבועי שלמי
};

const char omer_list[98][30]
{
	"rnugk sjt ouh ouhv",             // היום יום אחד לעומר
	"",                               // 
	"rnugk ohnh hba ouhv",            // היום שני ימים לעומר
	"",                               // 
	"rnugk ohnh vauka ouhv",          // היום שלושה ימים לעומר
	"",                               // 
	"rnugk ohnh vgcrt ouhv",          // היום ארבעה ימים לעומר
	"",                               // 
	"rnugk ohnh vahnj ouhv",          // היום חמישה ימים לעומר
	"",                               // 
	"rnugk ohnh vaha ouhv",           // היום שישה ימים לעומר
	"",                               // 
	"rnugk ohnh vgca ouhv",           // היום שבעה ימים לעומר
	"sjt guca ova",                   // שהם שבוע אחד
	"rnugk ohnh vbuna ouhv",          // היום שמונה ימים לעומר
	"sjt ouhu sjt guca ova",          // שהם שבוע אחד ויום אחד
	"rnugk ohnh vgaw ouhv",           // היום תשעה ימים לעומר
	"ohnh hbau sjt guca ova",         // שהם שבוע אחד ושני ימים
	"rnugk ohnh vrag ouhv",           // היום עשרה ימים לעומר
	"ohnh vaukau sjt guca ova",       // שהם שבוע אחד ושלושה ימים
	"rnugk ouh rag sjt ouhv",         // היום אחד עשר יום לעומר
	"ohnh vgcrtu sjt guca ova",       // שהם שבוע אחד וארבעה ימים
	"rnugk ouh rag ohba ouhv",        // היום שנים עשר יום לעומר
	"ohnh vahnju sjt guca ova",       // שהם שבוע אחד וחמישה ימים
	"rnugk ouh rag vauka ouhv",       // היום שלושה עשר יום לעומר
	"ohnh vahau sjt guca ova",        // שהם שבוע אחד ושישה ימים
	"rnugk ouh rag vgcrt ouhv",       // היום ארבעה עשר יום לעומר
	"wuguca hba ova",                 // שהם שני שבועות
	"rnugk ouh rag vahnj ouhv",       // היום חמישה עשר יום לעומר
	"sjt ouhu wuguca hba ova",        // שהם שני שבועות ויום אחד
	"rnugk ouh rag vaha ouhv",        // היום שישה עשר יום לעומר
	"ohnh hbau wuguca hba ova",       // שהם שני שבועות ושני ימים
	"rnugk ouh rag vgca ouhv",        // היום שבעה עשר יום לעומר
	"ohnh vaukau wuguca hba ova",     // שהם שני שבועות ושלושה ימים
	"rnugk ouh rag vbuna ouhv",       // היום שמונה עשר יום לעומר
	"ohnh vgcrtu wuguca hba ova",     // שהם שני שבועות וארבעה ימים
	"rnugk ouh rag vgaw ouhv",        // היום תשעה עשר יום לעומר
	"ohnh vahnju wuguca hba ova",     // שהם שני שבועות וחמישה ימים
	"rnugk ouh ohrag ouhv",           // היום עשרים יום לעומר
	"ohnh vahau wuguca hba ova",      // שהם שני שבועות ושישה ימים
	"rnugk ouh ohragu sjt ouhv",      // היום אחד ועשרים יום לעומר
	"wuguca vauka ova",               // שהם שלושה שבועות
	"rnugk ouh ohragu ohba ouhv",     // היום שנים ועשרים יום לעומר
	"sjt ouhu wuguca vauka ova",      // שהם שלושה שבועות ויום אחד
	"rnugk ouh ohragu vauka ouhv",    // היום שלושה ועשרים יום לעומר
	"ohnh hbau wuguca vauka ova",     // שהם שלושה שבועות ושני ימים
	"rnugk ouh ohragu vgcrt ouhv",    // היום ארבעה ועשרים יום לעומר
	"ohnh vaukau wuguca vauka ova",   // שהם שלושה שבועות ושלושה ימים
	"rnugk ouh ohragu vahnj ouhv",    // היום חמישה ועשרים יום לעומר
	"ohnh vgcrtu wuguca vauka ova",   // שהם שלושה שבועות וארבעה ימים
	"rnugk ouh ohragu vaha ouhv",     // היום שישה ועשרים יום לעומר
	"ohnh vahnju wuguca vauka ova",   // שהם שלושה שבועות וחמישה ימים
	"rnugk ouh ohragu vgca ouhv",     // היום שבעה ועשרים יום לעומר
	"ohnh vahau wuguca vauka ova",    // שהם שלושה שבועות ושישה ימים
	"rnugk ouh ohragu vbuna ouhv",    // היום שמונה ועשרים יום לעומר
	"wuguca vgcrt ova",               // שהם ארבעה שבועות
	"rnugk ouh ohragu vgaw ouhv",     // היום תשעה ועשרים יום לעומר
	"sjt ouhu wuguca vgcrt ova",      // שהם ארבעה שבועות ויום אחד
	"rnugk ouh ohauka ouhv",          // היום שלושים יום לעומר
	"ohnh hbau wuguca vgcrt ova",     // שהם ארבעה שבועות ושני ימים
	"rnugk ouh ohaukau sjt ouhv",     // היום אחד ושלושים יום לעומר
	"ohnh vaukau wuguca vgcrt ova",   // שהם ארבעה שבועות ושלושה ימים
	"rnugk ouh ohaukau ohba ouhv",    // היום שנים ושלושים יום לעומר
	"ohnh vgcrtu wuguca vgcrt ova",   // שהם ארבעה שבועות וארבעה ימים
	"rnugk ouh ohaukau vauka ouhv",   // היום שלושה ושלושים יום לעומר
	"ohnh vahnju wuguca vgcrt ova",   // שהם ארבעה שבועות וחמישה ימים
	"rnugk ouh ohaukau vgcrt ouhv",   // היום ארבעה ושלושים יום לעומר
	"ohnh vahau wuguca vgcrt ova",    // שהם ארבעה שבועות ושישה ימים
	"rnugk ouh ohaukau vahnj ouhv",   // היום חמישה ושלושים יום לעומר
	"wuguca vahnj ova",               // שהם חמישה שבועות
	"rnugk ouh ohaukau vaha ouhv",    // היום שישה ושלושים יום לעומר
	"sjt ouhu wuguca vahnj ova",      // שהם חמישה שבועות ויום אחד
	"rnugk ouh ohaukau vgca ouhv",    // היום שבעה ושלושים יום לעומר
	"ohnh hbau wuguca vahnj ova",     // שהם חמישה שבועות ושני ימים
	"rnugk ouh ohaukau vbuna ouhv",   // היום שמונה ושלושים יום לעומר
	"ohnh vaukau wuguca vahnj ova",   // שהם חמישה שבועות ושלושה ימים
	"rnugk ouh ohaukau vgaw ouhv",    // היום תשעה ושלושים יום לעומר
	"ohnh vgcrtu wuguca vahnj ova",   // שהם חמישה שבועות וארבעה ימים
	"rnugk ouh ohgcrt ouhv",          // היום ארבעים יום לעומר
	"ohnh vahnju wuguca vahnj ova",   // שהם חמישה שבועות וחמישה ימים
	"rnugk ouh ohgcrtu sjt ouhv",     // היום אחד וארבעים יום לעומר
	"ohnh vahau wuguca vahnj ova",    // שהם חמישה שבועות ושישה ימים
	"rnugk ouh ohgcrtu ohba ouhv",    // היום שנים וארבעים יום לעומר
	"wuguca vaha ova",                // שהם שישה שבועות
	"rnugk ouh ohgcrtu vauka ouhv",   // היום שלושה וארבעים יום לעומר
	"sjt ouhu wuguca vaha ova",       // שהם שישה שבועות ויום אחד
	"rnugk ouh ohgcrtu vgcrt ouhv",   // היום ארבעה וארבעים יום לעומר
	"ohnh hbau wuguca vaha ova",      // שהם שישה שבועות ושני ימים
	"rnugk ouh ohgcrtu vahnj ouhv",   // היום חמישה וארבעים יום לעומר
	"ohnh vaukau wuguca vaha ova",    // שהם שישה שבועות ושלושה ימים
	"rnugk ouh ohgcrtu vaha ouhv",    // היום שישה וארבעים יום לעומר
	"ohnh vgcrtu wuguca vaha ova",    // שהם שישה שבועות וארבעה ימים
	"rnugk ouh ohgcrtu vgca ouhv",    // היום שבעה וארבעים יום לעומר
	"ohnh vahnju wuguca vaha ova",    // שהם שישה שבועות וחמישה ימים
	"rnugk ouh ohgcrtu vbuna ouhv",   // היום שמונה וארבעים יום לעומר
	"ohnh vahau wuguca vaha ova",     // שהם שישה שבועות ושישה ימים
	"rnugk ouh ohgcrtu vgaw ouhv",    // היום תשעה וארבעים יום לעומר
	"wuguca vgca ova",                // שהם שבעה שבועות
};

const char omer_list_sfarad[98][31]
{
	"rnugk sjt ouh ouhv",             // היום יום אחד לעומר
	"",                               // 
	"rnugk ohnh hba ouhv",            // היום שני ימים לעומר
	"",                               // 
	"rnugk ohnh vauka ouhv",          // היום שלושה ימים לעומר
	"",                               // 
	"rnugk ohnh vgcRt ouhv",          // היום ארבעה ימים לעומר
	"",                               // 
	"rnugk ohnh vahnj ouhv",          // היום חמישה ימים לעומר
	"",                               // 
	"rnugk ohnh vaha ouhv",           // היום שישה ימים לעומר
	"",                               // 
	"ohnh vgca ouhv",                 // היום שבעה ימים 
	"rnugk sjt guca ova",             // שהם שבוע אחד לעומר
	"ova ohnh vbuna ouhv",            // היום שמונה ימים שהם
	"rnugk sjt ouhu sjt guca",        // שבוע אחד ויום אחד לעומר
	"ova ohnh vgaw ouhv",             // היום תשעה ימים שהם
	"rnugk ohnh hbau sjt guca",       // שבוע אחד ושני ימים לעומר
	"ova ohnh vrag ouhv",             // היום עשרה ימים שהם
	"rnugk ohnh vaukau sjt guca",     // שבוע אחד ושלושה ימים לעומר
	"ova ouh rag sjt ouhv",           // היום אחד עשר יום שהם
	"rnugk ohnh vgcRtu sjt guca",     // שבוע אחד וארבעה ימים לעומר
	"ova ouh rag ohba ouhv",          // היום שנים עשר יום שהם
	"rnugk ohnh vahnju sjt guca",     // שבוע אחד וחמישה ימים לעומר
	"ova ouh rag vauka ouhv",         // היום שלושה עשר יום שהם
	"rnugk ohnh vahau sjt guca",      // שבוע אחד ושישה ימים לעומר
	"ouh rag vgcRt ouhv",             // היום ארבעה עשר יום 
	"rnugk wuguca hba ova",           // שהם שני שבועות לעומר
	"ova ouh rag vahnj ouhv",         // היום חמישה עשר יום שהם
	"rnugk sjt ouhu wuguca hba",      // שני שבועות ויום אחד לעומר
	"ova ouh rag vaha ouhv",          // היום שישה עשר יום שהם
	"rnugk ohnh hbau wuguca hba",     // שני שבועות ושני ימים לעומר
	"ova ouh rag vgca ouhv",          // היום שבעה עשר יום שהם
	"rnugk ohnh vaukau wuguca hba",   // שני שבועות ושלושה ימים לעומר
	"ova ouh rag vbuna ouhv",         // היום שמונה עשר יום שהם
	"rnugk ohnh vgcRtu wuguca hba",   // שני שבועות וארבעה ימים לעומר
	"ova ouh rag vgaw ouhv",          // היום תשעה עשר יום שהם
	"rnugk ohnh vahnju wuguca hba",   // שני שבועות וחמישה ימים לעומר
	"ova ouh ohrag ouhv",             // היום עשרים יום שהם
	"rnugk ohnh vahau wuguca hba",    // שני שבועות ושישה ימים לעומר
	"ouh ohragu sjt ouhv",            // היום אחד ועשרים יום 
	"rnugk wuguca vauka ova",         // שהם שלושה שבועות לעומר
	"ova ouh ohragu ohba ouhv",       // היום שנים ועשרים יום שהם
	"rnugk sjt ouhu wuguca vauka",    // שלושה שבועות ויום אחד לעומר
	"ova ouh ohragu vauka ouhv",      // היום שלושה ועשרים יום שהם
	"rnugk ohnh hbau wugucA vauka",   // שלושה שבועות ושני ימים לעומר
	"ova ouh ohragu vgcRt ouhv",      // היום ארבעה ועשרים יום שהם
	"rnugk ohnh vaukau wugucA vauka", // שלושה שבועות ושלושה ימים לעומר
	"ova ouh ohragu vahnj ouhv",      // היום חמישה ועשרים יום שהם
	"rnugk ohnh vgcRtu wugucA vauka", // שלושה שבועות וארבעה ימים לעומר
	"ova ouh ohragu vaha ouhv",       // היום שישה ועשרים יום שהם
	"rnugk ohnh vahnju wugucA vauka", // שלושה שבועות וחמישה ימים לעומר
	"ova ouh ohragu vgca ouhv",       // היום שבעה ועשרים יום שהם
	"rnugk ohnh vahau wugucA vauka",  // שלושה שבועות ושישה ימים לעומר
	"ouh ohragu vbuna ouhv",          // היום שמונה ועשרים יום 
	"rnugk wuguca vgcRt ova",         // שהם ארבעה שבועות לעומר 
	"ova ouh ohragu vgaw ouhv",       // היום תשעה ועשרים יום שהם
	"rnugk sjt ouhu wuguca vgcRt",    // ארבעה שבועות ויום אחד לעומר
	"ova ouh ohauka ouhv",            // היום שלושים יום שהם
	"rnugk ohnh hbau wuguca vgcRt",   // ארבעה שבועות ושני ימים לעומר
	"ova ouh ohaukau sjt ouhv",       // היום אחד ושלושים יום שהם
	"rnugk ohnh vaukau wugucA vgcRt", // ארבעה שבועות ושלושה ימים לעומר
	"ova ouh ohaukau ohba ouhv",      // היום שנים ושלושים יום שהם
	"Rnugk ohnh vgcRtu wugucA vgcRt", // ארבעה שבועות וארבעה ימים לעומר
	"ova ouh ohaukau vauka ouhv",     // היום שלושה ושלושים יום שהם
	"rnugk ohnh vahnju wugucA vgcRt", // ארבעה שבועות וחמישה ימים לעומר
	"ova ouh ohaukau vgcRt ouhv",     // היום ארבעה ושלושים יום שהם
	"rnugk ohnh vahau wugucA vgcRt",  // ארבעה שבועות ושישה ימים לעומר
	"ouh ohaukau vahnj ouhv",         // היום חמישה ושלושים יום 
	"rnugk wuguca vahnj ova",         // שהם חמישה שבועות לעומר
	"ova ouh ohaukau vaha ouhv",      // היום שישה ושלושים יום שהם
	"rnugk sjt ouhu wuguca vahnj",    // חמישה שבועות ויום אחד לעומר
	"ova ouh ohaukau vgca ouhv",      // היום שבעה ושלושים יום שהם
	"rnugk ohnh hbau wuguca vahnj",   // חמישה שבועות ושני ימים לעומר
	"ova ouh ohaukau vbuna ouhv",     // היום שמונה ושלושים יום שהם
	"rnugk ohnh vaukau wugucA vahnj", // חמישה שבועות ושלושה ימים לעומר
	"ova ouh ohaukau vgaw ouhv",      // היום תשעה ושלושים יום שהם
	"rnugk ohnh vgcRtu wugucA vahnj", // חמישה שבועות וארבעה ימים לעומר
	"ova ouh ohgcrt ouhv",            // היום ארבעים יום שהם
	"rnugk ohnh vahnju wugucA vahnj", // חמישה שבועות וחמישה ימים לעומר
	"ova ouh ohgcrtu sjt ouhv",       // היום אחד וארבעים יום שהם
	"rnugk ohnh vahau wugucA vahnj",  // חמישה שבועות ושישה ימים לעומר
	"ouh ohgcrtu ohba ouhv",          // היום שנים וארבעים יום 
	"rnugk wuguca vaha ova",          // שהם שישה שבועות לעומר
	"ova ouh ohgcrtu vauka ouhv",     // היום שלושה וארבעים יום שהם
	"rnugk sjt ouhu wuguca vaha",     // שישה שבועות ויום אחד לעומר
	"ova ouh ohgcrtu vgcRt ouhv",     // היום ארבעה וארבעים יום שהם
	"rnugk ohnh hbau wuguca vaha",    // שישה שבועות ושני ימים לעומר
	"ova ouh ohgcrtu vahnj ouhv",     // היום חמישה וארבעים יום שהם
	"rnugk ohnh vaukau wugucA vaha",  // שישה שבועות ושלושה ימים לעומר
	"ova ouh ohgcrtu vaha ouhv",      // היום שישה וארבעים יום שהם
	"rnugk ohnh vgcRtu wugucA vaha",  // שישה שבועות וארבעה ימים לעומר
	"ova ouh ohgcrtu vgca ouhv",      // היום שבעה וארבעים יום שהם
	"rnugk ohnh vahnju wugucA vaha",  // שישה שבועות וחמישה ימים לעומר
	"ova ouh ohgcrtu vbuna ouhv",     // היום שמונה וארבעים יום שהם
	"rnugk ohnh vahau wugucA vaha",   // שישה שבועות ושישה ימים לעומר
	"ouh ohgcrtu vgaw ouhv",          // היום תשעה וארבעים יום 
	"rnugk wuguca vgca ova",          // שהם שבעה שבועות לעומר
};
*/
const char chabad_list[32][29]
{
"",                                                   //0. 
"jmb ishs",                                           //1. דידן נצח
"o\"cnrv wrhyp ouh",                                  //2. יום פטירת הרמב"ם
"vbj whbcrv wrhyp ouh",                               //3. יום פטירת הרבנית חנה
"esm jnmv r\"unst wuekwxv ouh",                       //4. יום הסתלקות אדמו"ר הצמח צדק
"hcrv htuahb ouh",                                    //5. יום נישואי הרבי
"rhvcv ouh",                                          //6. יום הבהיר
"a\"rvn r\"unst wskuv ouh",                           //7. יום הולדת אדמו"ר מהר"ש
"c\"aruvn r\"unst wskuv ouh",                         //8. יום הולדת אדמו"ר מהורש"ב
"ohnhnw hfnuw wchah wusxhhwv",                        //9. התייסדות ישיבת תומכי תמימים
"esm jnmv r\"unst wskuv ouh",                         //10. יום הולדת אדמו"ר הצמח צדק
"iezv r\"unst wuekwxv ouh",                           //11. יום הסתלקות אדמו"ר הזקן
"iezv r\"unst vkutdv dj",                             //12. חג הגאולה אדמו"ר הזקן
"\'_hccukn hcrv wskuv ouh",                           //13. יום הולדת הרבי מלובביץ'
"a\"rvn r\"unst wuekwxv ouh",                         //14. יום הסתלקות אדמו"ר מהר"ש
"m\"hhruvn r\"unst vkutdv dj",                        //15. חג הגאולה אדמו"ר מהוריי"צ
"c\"aruvn r\"unst wuekwxv ouh",                       //16. יום הסתלקות אדמו"ר מהורש"ב
"teaun vhj whbcrv wuekwxv ouh",                       //17. יום הסתלקות הרבנית חיה מושקא
"hgmntv r\"unst vkutdv dj",                           //18. חג הגאולה אדמו"ר האמצעי
"ejmh huk hcr wuekwxv ouh",                           //19. יום הסתלקות רבי לוי יצחק
"iezv r\"unst vkutdv dj",                             //20. חג הגאולה אדמו"ר הזקן
"wushxjk vbav atr",                                   //21. ראש השנה לחסידות
"wskuv ouhu vkutdv dj",                               //22. חג הגאולה ויום הולדת
"m\"hhruvn r\"unst",                                  //23. אדמו"ר מהוריי"צ
"m\"hhruvn r\"unst wuekwxv ouh",                      //24. יום הסתלקות אדמו"ר מהוריי"צ
"hcrv wuthab wkce",                                   //25. קבלת נשיאות הרבי
"m\"hhruvn r\"unst vkutdv dj",                        //26. חג הגאולה אדמו"ר מהוריי"צ
"tkukhvv ouh",                                        //27. יום ההילולא
"wuekwxv ouhu wskuv ouh",                             //28. יום הולדת ויום הסתלקות
"hgmntv r\"unst",                                     //29. אדמו"ר האמצעי
"iezv r\"unst wskuv ouh",                             //30. יום הולדת אדמו"ר הזקן
"cuy oa kgcv wskuv ouh",                              //31. יום הולדת הבעל שם טוב
};
#endif