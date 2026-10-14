#ifndef HEBREWDATE_H
#define HEBREWDATE_H

#include <cstdint>
#include "Dict.h"
#include <stdio.h>


class HebrewDate
{
public:
    uint8_t yom;
    uint8_t hodesh;
    uint8_t shana;
    uint16_t shana_0 = 5780;
    HebrewDate() : yom(0), hodesh(0), shana(0) {}
    HebrewDate(uint8_t _yom, uint8_t _hodesh, uint8_t _shana) : yom(_yom), hodesh(_hodesh), shana(_shana){}
    void toString(char* buffer, uint8_t format) 
    { 
        if(format == 1) {sprintf(buffer, "%s %s %s", shana_list_s[shana], hodesh_list_s[hodesh], ot_list[yom]);}
        else if(format == 2) {sprintf(buffer, "%s %s %s", shana_list_l[shana], hodesh_list_l[hodesh], yom_list[yom]);}
        else if(format == 3) {sprintf(buffer, "%s %s", hodesh_list_l[hodesh], yom_list[yom]);}
        // else if(format == 4) {sprintf(buffer, "%d %s %d", shana+shana_0, hodesh_list_l[hodesh], yom);}
    }
};

#endif