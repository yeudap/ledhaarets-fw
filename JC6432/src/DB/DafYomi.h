#ifndef DAFYOMI_H
#define DAFYOMI_H

#include "Dict.h"
#include <cstdint>
#include <stdio.h>
#include "strings.h"
extern Strings strings;

class DafYomi
{
public:
    uint8_t masechet;
    uint8_t daf;
    DafYomi() : masechet(0), daf(0) {}
    DafYomi(uint8_t _masechet, uint8_t _daf) : masechet(_masechet), daf(_daf) {}
    void toString(char* buffer, uint8_t type) 
    { 
        if(type == 1) { sprintf(buffer, "%s %s :%s", ot_list[daf], masechet_list[masechet], strings.dafHayomiBavli); }   
        else if(type == 2) { sprintf(buffer, "%s %s :%s", ot_list[daf], masechet_list[masechet], strings.dafHayomiYerushalmi); }   
        else { sprintf(buffer, "%s %s", ot_list[daf], masechet_list[masechet]); }   
    }
};

#endif