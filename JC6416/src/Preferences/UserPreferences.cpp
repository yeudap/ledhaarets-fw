
#include "UserPreferences.h"
#include <States/States.h>

UserPreferences userPreferences;

void setupPreferences()
{
    userPreferences.init("clk");
    userPreferences.loadAll();
}