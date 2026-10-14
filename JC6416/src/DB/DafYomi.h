#ifndef DAFYOMI_H
#define DAFYOMI_H

#include "Dict.h"
#include <cstdint>
#include <stdio.h>

class DafYomi
{
public:
    uint8_t masechet;
    uint8_t daf;
    DafYomi() : masechet(0), daf(0) {}
    DafYomi(uint8_t _masechet, uint8_t _daf) : masechet(_masechet), daf(_daf) {}
    void toString(char* buffer, bool title) 
    { 
        if(title) { sprintf(buffer, "%s %s %s", ot_list[daf], masechet_list[masechet], ":hnuhv qs"); }   
        else { sprintf(buffer, "%s %s", ot_list[daf], masechet_list[masechet]); }   
    }
};

#endif