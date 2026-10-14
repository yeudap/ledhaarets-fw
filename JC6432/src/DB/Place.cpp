#include "Place.h"

Place* place;
int numOfOfeks;
int lonPre;
int lonInt;
int lonFrac;
int latPre;
int latInt;
int latFrac;

Place places[] = {
	Place(),
	Place("chct kw", 32.06, 34.77, 2, 20, 22, false, false, or_tel_aviv_db, kise_tel_aviv_db, chai_tel_aviv_db),
	Place("ohkaurh", 31.769, 35.213, 2, 800, 40, true, false, or_yerushalaim_db, kise_yerushalaim_db, chai_yerushalaim_db),
	Place("vphj", 32.8, 35.0, 2, 300, 30, false, false, or_heyfa_db, kise_heyfa_db, chai_heyfa_db),
	Place("gca rtc", 31.24, 34.79, 2, 270, 20, false, false, or_beer_sheva_db, kise_beer_sheva_db, chai_beer_sheva_db),
	Place("vsuvh rut", 32.03, 34.856, 2, 80, 20, false, false, nullptr, nullptr, chai_or_yehuda_db),
	Place("wkht", 29.55, 34.95, 2, 150, 20, false, false, nullptr, nullptr, chai_eilat_db),
	Place("vrun iukt", 32.234, 35.33, 2, 654, 20, false, false, nullptr, nullptr, chai_alon_more_db),
	Place("sgkt", 32.05, 34.95, 2, 150, 30, false, false, nullptr, nullptr, chai_elad_db),
	Place("vabn hpkt", 32.18, 35.01, 2, 245, 20, false, false, nullptr, nullptr, chai_alfe_menashe_db),
	Place("kthrt", 32.104, 35.21, 2, 670, 20, false, false, nullptr, nullptr, chai_ariel_db),
	Place("susat", 31.79, 34.641, 2, 35, 22, false, false, nullptr, nullptr, chai_ashdod_db),
	Place("iukeat", 31.65, 34.56, 2, 60, 20, false, false, nullptr, nullptr, chai_ashkelon_db),
	Place("cegh rtc", 31.93, 34.83, 2, 80, 20, false, false, nullptr, nullptr, chai_beer_yaakov_db),
	Place("kt whc", 31.941, 35.222, 2, 869, 20, false, false, nullptr, nullptr, chai_beit_el_db),
	Place("ids whc", 32.002, 34.829, 2, 34, 20, false, false, nullptr, nullptr, chai_beit_dagan_db),
	Place("vhekj whc", 31.793, 34.809, 2, 75, 20, false, false, nullptr, nullptr, chai_beit_hilkia_db),
	Place("ita whc", 32.5, 35.5, 2, -120, 20, false, false, nullptr, nullptr, chai_beit_shean_db),
	Place("vhre ana whc", 31.74, 34.995, 2, 310, 40, false, false, nullptr, nullptr, chai_beit_shemesh_kirya_db),
	Place("vnr ana whc", 31.754, 35.075, 2, 375, 40, false, false, nullptr, nullptr, chai_beit_shemesh_rama_db),
	Place("whkhg rwhc", 31.69, 35.12, 2, 660, 40, false, false, nullptr, nullptr, chai_beitar_db),
	Place("erc hbc", 32.08, 34.82, 2, 60, 22, false, false, nullptr, nullptr, chai_bne_brak_db),
	Place("vbhnhbc", 32.52, 34.95, 2, 30, 20, false, false, nullptr, nullptr, chai_binyamina_db),
	Place("oh wc", 32.02, 34.74, 2, 50, 20, false, false, nullptr, nullptr, chai_tel_aviv_db),
	Place("ctz wgcd", 31.86, 35.16, 2, 680, 40, false, false, nullptr, nullptr, chai_givaat_zeev_db),
	Place("vrsd", 31.81, 34.77, 2, 76, 20, false, false, nullptr, nullptr, chai_gedera_db),
	Place("vbch id", 31.794, 34.72, 2, 60, 20, false, false, nullptr, nullptr, chai_gan_yavne_db),
	Place("vbunhs", 31.06, 35.03, 2, 580, 20, false, false, nullptr, nullptr, chai_dimona_db),
	Place("cegh iurfz", 32.57, 34.95, 2, 170, 30, false, false, nullptr, nullptr, chai_zichron_yaakov_db),
	Place("iurcj", 31.52, 35.1, 2, 987, 22, false, false, nullptr, nullptr, chai_hevron_db),
	Place("vrsj", 32.44, 34.91, 2, 40, 30, false, false, nullptr, nullptr, chai_hadera_db),
	Place("iukuj", 32.01, 34.75, 2, 50, 30, false, false, nullptr, nullptr, chai_tel_aviv_db),
	Place("whkhkdv rumj", 32.983, 35.542, 2, 400, 30, false, false, nullptr, nullptr, chai_hazor_haglilit_db),
	Place("ahrj", 32.46, 35.04, 2, 115, 30, false, false, nullptr, nullptr, chai_harish_db),
	Place("ohtbunaj", 31.93, 35.02, 2, 255, 22, false, false, nullptr, nullptr, chai_hashmonaim_db),
	Place("vhrcy", 32.79, 35.531, 2, -200, 30, false, false, nullptr, nullptr, chai_tveriya_db),
	Place("whkhg vhrcy", 32.783, 35.513, 2, 130, 30, false, false, nullptr, nullptr, chai_tveriya_ilit_db),
	Place("vabn ky", 32.483, 35.16, 2, 400, 20, false, false, nullptr, nullptr, chai_tal_menashe_db),
	Place("iuyx zky", 31.78, 35.1, 2, 720, 30, false, false, nullptr, nullptr, chai_telz_ston_db),
	Place("ktbch", 32.7, 35.5, 2, 0, 20, false, false, nullptr, nullptr, chai_yavniel_db),
	Place("vbch", 31.875, 34.735, 2, 33, 20, false, false, nullptr, nullptr, chai_yavne_db),
	Place("ojurh", 30.99, 34.91, 2, 500, 20, false, false, nullptr, nullptr, chai_yeruham_db),
	Place("scj rpf", 31.99, 34.85, 2, 45, 30, false, false, nullptr, nullptr, chai_kfar_habad_db),
	Place("tcx rpf", 32.18, 34.91, 2, 0, 20, false, false, nullptr, nullptr, chai_kfar_saba_raanana_db),
	Place("kthnrf", 32.92, 35.3, 2, 0, 20, false, false, nullptr, nullptr, chai_karmiel_db),
	Place("suk", 31.95, 34.89, 2, 0, 20, false, false, nullptr, nullptr, chai_lud_db),
	Place("iuhm wracn", 31.8, 35.15, 2, 751, 20, false, false, nullptr, nullptr, chai_mevaseret_db),
	Place("engv ksdn", 32.67, 35.23, 2, 0, 20, false, false, nullptr, nullptr, chai_migdal_haemek_db),
	Place("ihghsun", 31.895, 35.012, 2, 280, 20, false, false, nullptr, nullptr, chai_modiin_db),
	Place("whkhg ihghsun", 31.93, 35.04, 2, 300, 30, false, false, nullptr, nullptr, chai_modiin_ilit_db),
	Place("vkuyn", 33.276, 35.572, 2, 585, 40, false, false, nullptr, nullptr, chai_matula_db),
	Place("iurhn", 32.98, 35.43, 2, 720, 30, false, false, nullptr, nullptr, chai_meiron_db),
	Place("ohnust vkgn", 31.77, 35.3, 2, 500, 20, false, false, nullptr, nullptr, chai_maale_adumim_db),
	Place("ohrpt vkgn", 32.07, 35.4, 2, 210, 20, false, false, nullptr, nullptr, chai_maale_efraim_db),
	Place("wukgn", 33.015, 35.278, 2, 600, 30, false, false, nullptr, nullptr, chai_maalot_db),
	Place("iunr vpmn", 30.61, 34.8, 2, 892, 20, false, false, nullptr, nullptr, chai_mizfe_ramon_db),
	Place("trhpa zfrn", 31.697, 34.7, 2, 53, 20, false, false, nullptr, nullptr, chai_merkaz_shapira_db),
	Place("vhrvb", 33.01, 35.1, 2, 25, 20, false, false, nullptr, nullptr, chai_naharia_db),
	Place("khkdv qub", 32.7, 35.32, 2, 500, 30, false, false, nullptr, nullptr, chai_nof_hagalil_db),
	Place("wuchwb", 31.42, 34.59, 2, 142, 30, false, false, nullptr, nullptr, chai_netivot_db),
	Place("vhbwb", 32.34, 34.86, 2, 22, 22, false, false, nullptr, nullptr, chai_netania_db),
	Place("cbhg", 32.283, 35.125, 2, 320, 20, false, false, nullptr, nullptr, chai_einav_db),
	Place("ufg", 32.93, 35.08, 2, 0, 20, false, false, nullptr, nullptr, chai_ako_db),
	Place("ktubng", 32.16, 35.13, 2, 406, 40, false, false, nullptr, nullptr, chai_emanuel_db),
	Place("vkupg", 32.6, 35.29, 2, 60, 30, false, false, nullptr, nullptr, chai_afula_db),
	Place("ktusp", 32.06, 35.05, 2, 376, 20, false, false, nullptr, nullptr, chai_pduel_db),
	Place("vbj xsrp", 32.46, 34.96, 2, 0, 20, false, false, nullptr, nullptr, chai_pardes_hana_db),
	Place("vuew jwp", 32.09, 34.88, 2, 0, 40, false, false, nullptr, nullptr, chai_petah_tikva_db),
	Place("wpm", 32.962, 35.496, 2, 640, 30, false, false, nullptr, nullptr, chai_zfat_db),
	Place("ohnuse", 32.21, 35.15, 2, 490, 20, false, false, nullptr, nullptr, chai_kdumim_db),
	Place("wuhnnue", 31.65, 34.73, 2, 76, 20, false, false, nullptr, nullptr, chai_komemiut_db),
	Place("wuhre", 32.83, 35.09, 2, 0, 20, false, false, nullptr, nullptr, chai_krayot_db),
	Place("twt whre", 32.801, 35.093, 2, 30, 30, false, false, nullptr, nullptr, chai_kiryat_ata_db),
	Place("wd whre", 31.61, 34.77, 2, 159, 22, false, false, nullptr, nullptr, chai_kiryat_gat_db),
	Place("hftkn whre", 31.73, 34.75, 2, 70, 20, false, false, nullptr, nullptr, chai_kiryat_malachi_db),
	Place("rpx whre", 31.93, 35.04, 2, 300, 30, false, false, nullptr, nullptr, chai_kiryat_sefer_db),
	Place("vbuna whre", 33.203, 35.568, 2, 170, 30, false, false, nullptr, nullptr, chai_kiryat_shmona_db),
	Place("iurnua hbre", 32.17, 35.097, 2, 351, 20, false, false, nullptr, nullptr, chai_karnei_shomron_db),
	Place("ihgv atr", 32.08, 34.95, 2, 90, 20, false, false, nullptr, nullptr, chai_rosh_haain_db),
	Place("vbhp atr", 32.96, 35.49, 2, 130, 20, false, false, nullptr, nullptr, chai_hazor_haglilit_db),
	Place("iuhmk iuatr", 31.96, 34.8, 2, 0, 20, false, false, nullptr, nullptr, chai_rishon_lezion_db),
	Place("wucujr", 31.89, 34.81, 2, 76, 22, false, false, nullptr, nullptr, chai_rehovot_db),
	Place("ohxfr", 32.74, 35.08, 2, 154, 30, false, false, nullptr, nullptr, chai_rechasim_db),
	Place("vknr", 31.92, 34.86, 2, 0, 20, false, false, nullptr, nullptr, chai_ramle_db),
	Place("vbbgr", 32.16, 34.85, 2, 71, 20, false, false, nullptr, nullptr, chai_kfar_saba_raanana_db),
	Place("wursa", 31.52, 34.59, 2, 0, 20, false, false, nullptr, nullptr, chai_sderot_db),
	Place("ohckga", 31.86, 34.98, 2, 194, 20, false, false, nullptr, nullptr, chai_shaalabim_db),
	Place("iuhm kw", 31.88, 35.238, 2, 800, 40, false, false, nullptr, nullptr, chai_tel_zion_db),
	Place("jrpw", 31.32, 34.67, 2, 173, 30, false, false, nullptr, nullptr, chai_tifrah_db),
	Place("eruh uhb", 40.688, -73.976, -5, 13, 18, false, true, nullptr, nullptr, chai_new_york_db),
	Place("xk'dbt xuk", 34.058, -118.39, -8, 57, 18, false, true, nullptr, nullptr, chai_los_angeles_db),
	Place("vhpkskhp", 40.14, -75.11, -5, 14, 18, false, true, nullptr, nullptr, chai_philadelphia_db),
	Place("udeha", 42.0, -87.7, -6, 0, 18, false, true, nullptr, nullptr, chai_chicago_db),
	Place("iusbuk", 51.5, 0.1, 0, 45, 15, false, true, nullptr, nullptr, chai_london_db),
	Place("zhrp", 48.86, 2.34, 1, 423, 20, false, true, nullptr, nullptr, chai_paris_db),
};



void initPlace()
{
    places[0] = Place(userPreferences.textPlace.c_str(), userPreferences.nz_lat, userPreferences.nz_long, userPreferences.timezone, userPreferences.nz_height, userPreferences.nerot, userPreferences.homa, userPreferences.nz_hul, nullptr,  nullptr,  nullptr);
    numOfOfeks = sizeof(places) / sizeof(places[0]);

    uint8_t idx = userPreferences.ofek;
    if(idx<0 || idx>=numOfOfeks) 
    {
        idx = userPreferences.ofek.getDefault(); 
		userPreferences.ofek= idx;
    }
    place = &places[idx];

    // Serial.println("initPlace: ");
    // Serial.print("userPreferences.nz_long: ");
    // Serial.println(userPreferences.nz_long);
    // Serial.print("userPreferences.nz_lat: ");
    // Serial.println(userPreferences.nz_lat);
    // Serial.print("userPreferences.timezone: ");
    // Serial.println(userPreferences.timezone);

    double_t val;
    val = userPreferences.nz_long;
    lonPre = 1;
    if (val < 0) { lonPre = -1; }
    val *= lonPre; // abs
    lonInt = int(val);
    lonFrac = (val-lonInt)*100;

    val = userPreferences.nz_lat;
    latPre = 1;
    if (val < 0) { latPre = -1; }
    val *= latPre; // abs
    latInt = int(val);
    latFrac = (val-latInt)*100;
}

double Place::getTimezone()
{
    if (this->kaiz) { return this->timezone+1; }
    return timezone;
}
