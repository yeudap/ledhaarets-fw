#ifndef PLACE_H
#define PLACE_H

#include <Arduino.h>
#include <stdio.h>

#include "DataBase/orHachaim/or_tel_aviv_db.h"
#include "DataBase/orHachaim/or_yerushalaim_db.h"
#include "DataBase/orHachaim/or_heyfa_db.h"
#include "DataBase/orHachaim/or_beer_sheva_db.h"

#include "DataBase/kiseRahamim/kise_tel_aviv_db.h"
#include "DataBase/kiseRahamim/kise_yerushalaim_db.h"
#include "DataBase/kiseRahamim/kise_heyfa_db.h"
#include "DataBase/kiseRahamim/kise_beer_sheva_db.h"

#include "DataBase/luchotChai/chai_tel_aviv_db.h"         
#include "DataBase/luchotChai/chai_yerushalaim_db.h"      
#include "DataBase/luchotChai/chai_heyfa_db.h"            
#include "DataBase/luchotChai/chai_beer_sheva_db.h"     

#include "DataBase/luchotChai/chai_bne_brak_db.h"         
#include "DataBase/luchotChai/chai_ashdod_db.h"           
#include "DataBase/luchotChai/chai_beitar_db.h"           
#include "DataBase/luchotChai/chai_elad_db.h"             
#include "DataBase/luchotChai/chai_modiin_ilit_db.h"      
#include "DataBase/luchotChai/chai_beit_shemesh_kirya_db.h"
#include "DataBase/luchotChai/chai_beit_shemesh_rama_db.h"
#include "DataBase/luchotChai/chai_maale_adumim_db.h"     
#include "DataBase/luchotChai/chai_modiin_db.h"           
#include "DataBase/luchotChai/chai_hevron_db.h"           
#include "DataBase/luchotChai/chai_ofakim_db.h"           
#include "DataBase/luchotChai/chai_netivot_db.h"          
#include "DataBase/luchotChai/chai_ashkelon_db.h"         
#include "DataBase/luchotChai/chai_kiryat_gat_db.h"       
#include "DataBase/luchotChai/chai_kiryat_malachi_db.h"   
#include "DataBase/luchotChai/chai_yeruham_db.h"          
#include "DataBase/luchotChai/chai_eilat_db.h"            
#include "DataBase/luchotChai/chai_beit_shean_db.h"       
#include "DataBase/luchotChai/chai_krayot_db.h"           
#include "DataBase/luchotChai/chai_rechasim_db.h"         
#include "DataBase/luchotChai/chai_kiryat_shmona_db.h"    
#include "DataBase/luchotChai/chai_tveriya_db.h"          
#include "DataBase/luchotChai/chai_tveriya_ilit_db.h"     
#include "DataBase/luchotChai/chai_meiron_db.h"           
#include "DataBase/luchotChai/chai_zfat_db.h"             
#include "DataBase/luchotChai/chai_ako_db.h"              
#include "DataBase/luchotChai/chai_naharia_db.h"    
#include "DataBase/luchotChai/chai_rehovot_db.h"    
#include "DataBase/luchotChai/chai_ariel_db.h"    
#include "DataBase/luchotChai/chai_yavniel_db.h"    
#include "DataBase/luchotChai/chai_karnei_shomron_db.h"    
#include "DataBase/luchotChai/chai_philadelphia_db.h"           
#include "DataBase/luchotChai/chai_new_york_db.h"
#include "DataBase/luchotChai/chai_paris_db.h"    
#include "DataBase/luchotChai/chai_chicago_db.h"    
#include "DataBase/luchotChai/chai_london_db.h"    
#include "DataBase/luchotChai/chai_los_angeles_db.h"    
#include "DataBase/luchotChai/chai_alon_more_db.h"
#include "DataBase/luchotChai/chai_beer_yaakov_db.h"
#include "DataBase/luchotChai/chai_beit_dagan_db.h"
#include "DataBase/luchotChai/chai_beit_el_db.h"
#include "DataBase/luchotChai/chai_beit_hilkia_db.h"
#include "DataBase/luchotChai/chai_binyamina_db.h"
#include "DataBase/luchotChai/chai_dimona_db.h"
#include "DataBase/luchotChai/chai_einav_db.h"
#include "DataBase/luchotChai/chai_emanuel_db.h"
#include "DataBase/luchotChai/chai_gan_yavne_db.h"
#include "DataBase/luchotChai/chai_gedera_db.h"
#include "DataBase/luchotChai/chai_givaat_zeev_db.h"
#include "DataBase/luchotChai/chai_hadera_db.h"
#include "DataBase/luchotChai/chai_harish_db.h"
#include "DataBase/luchotChai/chai_hashmonaim_db.h"
#include "DataBase/luchotChai/chai_hazor_haglilit_db.h"
#include "DataBase/luchotChai/chai_karmiel_db.h"
#include "DataBase/luchotChai/chai_kfar_habad_db.h"
#include "DataBase/luchotChai/chai_kfar_saba_raanana_db.h"
#include "DataBase/luchotChai/chai_kiryat_ata_db.h"
#include "DataBase/luchotChai/chai_kiryat_sefer_db.h"
#include "DataBase/luchotChai/chai_komemiut_db.h"
#include "DataBase/luchotChai/chai_lud_db.h"
#include "DataBase/luchotChai/chai_maalot_db.h"
#include "DataBase/luchotChai/chai_matula_db.h"
#include "DataBase/luchotChai/chai_merkaz_shapira_db.h"
#include "DataBase/luchotChai/chai_mevaseret_db.h"
#include "DataBase/luchotChai/chai_migdal_haemek_db.h"
#include "DataBase/luchotChai/chai_mizfe_ramon_db.h"
#include "DataBase/luchotChai/chai_netania_db.h"
#include "DataBase/luchotChai/chai_nof_hagalil_db.h"
#include "DataBase/luchotChai/chai_or_yehuda_db.h"
#include "DataBase/luchotChai/chai_pardes_hana_db.h"
#include "DataBase/luchotChai/chai_pduel_db.h"
#include "DataBase/luchotChai/chai_petah_tikva_db.h"
#include "DataBase/luchotChai/chai_ramle_db.h"
#include "DataBase/luchotChai/chai_rishon_lezion_db.h"
#include "DataBase/luchotChai/chai_rosh_haain_db.h"
#include "DataBase/luchotChai/chai_sderot_db.h"
#include "DataBase/luchotChai/chai_shaalabim_db.h"
#include "DataBase/luchotChai/chai_telz_ston_db.h"
#include "DataBase/luchotChai/chai_tel_zion_db.h"
#include "DataBase/luchotChai/chai_tifrah_db.h"
#include "DataBase/luchotChai/chai_yavne_db.h"
#include "DataBase/luchotChai/chai_zichron_yaakov_db.h"
#include "DataBase/luchotChai/chai_kdumim_db.h"
#include "DataBase/luchotChai/chai_tal_menashe_db.h"
#include "DataBase/luchotChai/chai_maale_efraim_db.h"
#include "DataBase/luchotChai/chai_afula_db.h"
#include "DataBase/luchotChai/chai_alfe_menashe_db.h"

#include "Preferences/UserPreferences.h"
extern UserPreferences userPreferences;


class Place
{
public:
    char name[20];
    double latitude;
    double longitude;
    double timezone;
    double height;
    int nerot;
    bool homa;
    bool hul;
    const uint8_t* orHachaim;
    const uint8_t* kiseRahamim;
    const uint8_t* luchotChai;
    bool kaiz;

        
    Place(){}
    Place(const char* _name, double lat, double lon, double tz, double h, int ner, bool _homa, bool _hul, const uint8_t* _or, const uint8_t* kise, const uint8_t* chai)
    {
        strcpy(name, _name);
        latitude = lat;
        longitude = lon;
        timezone = tz;
        height = h;
        nerot = ner;
        homa = _homa;
        hul = _hul;
        orHachaim = _or;
        kiseRahamim = kise;
        luchotChai = chai;
        kaiz = false;
    }
    double getTimezone();
};

void initPlace();

#endif
