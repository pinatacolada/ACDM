#include "pch.h"
#include "ACDMPlugin.h"
#include "Constants.h"

ACDMPlugin::ACDMPlugin() : CPlugIn(COMPATIBILITY_CODE, PLUGIN_NAME.c_str(), PLUGIN_VERSION.c_str(), PLUGIN_AUTHOR.c_str(), PLUGIN_COPYRIGHT.c_str()) {
	// Register tag function
	RegisterTagItemType("Flight Type Indicator", TAG_ITEM_FLIGHT_TYPE);
}


ACDMPlugin::~ACDMPlugin() { }

void ACDMPlugin::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
    EuroScopePlugIn::CRadarTarget RadarTarget,
    int ItemCode,
    int TagData,
    char sItemString[16],
    int* pColorCode,
    COLORREF* pRGB,
    double* pFontSize)
{
    string origin = FlightPlan.GetFlightPlanData().GetOrigin();
    string dest = FlightPlan.GetFlightPlanData().GetDestination();
    char WTC;

    if (ItemCode != TAG_ITEM_FLIGHT_TYPE)
    {
        return;
    }

    WTC = FlightPlan.GetFlightPlanData().GetAircraftWtc();

    if (WTC == 'J')
    {
        strcpy(sItemString, "J");
        return;
    }
    else if (WTC == 'H')
    {
        strcpy(sItemString, "H");
        return;
    }

    if (strcmp(origin.c_str(), "ZZZZ") == 0 || strcmp(dest.c_str(), "ZZZZ") == 0)
    {
        strcpy(sItemString, "");
        return;
    }
    else {
        if (strncmp(origin.c_str(), "LP", 2) == 0)
        {
            if (strncmp(dest.c_str(), "LP", 2) == 0)
            {
                strcpy(sItemString, "D");
                return;
            }
            else
            {
                strcpy(sItemString, "P");
                return;
            }
        }
        else
        {
            if (strncmp(dest.c_str(), "LP", 2) == 0)
            {
                strcpy(sItemString, "C");
                return;
            }
            else
            {
                strcpy(sItemString, "S");
                return;
            }
        }
    }    
}