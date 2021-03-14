#include "pch.h"
#include "ACDMPlugin.h"
#include "Constants.h"
#include "helpers.h"
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <future>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <type_traits>
#include <iomanip>

ACDMPlugin::ACDMPlugin() : CPlugIn(COMPATIBILITY_CODE, PLUGIN_NAME.c_str(), PLUGIN_VERSION.c_str(), PLUGIN_AUTHOR.c_str(), PLUGIN_COPYRIGHT.c_str()) {
	RegisterTagItemType("ACDM Dummy Item", TAG_ITEM_ACDM_DUMMY);
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

    if (!FlightPlan.IsValid()) {
        return;
    }


        std::string adep = FlightPlan.GetFlightPlanData().GetOrigin();
        to_upper(adep);

        if (adep.compare("LPPT") == 0) {
            
            FlightPlan.GetFlightPlanData().SetEstimatedDepartureTime(CheckEtd(FlightPlan).c_str());

        }
    
          
}

std::string ACDMPlugin::CheckEtd(CFlightPlan FlightPlan)
{
    std::string cs = FlightPlan.GetCallsign();    
    std::string etd = FlightPlan.GetFlightPlanData().GetEstimatedDepartureTime();
    etd = std::string(4 - etd.length(), '0') + etd;

    std::string sEtdH = etd.substr(0, 2);
    std::string sEtdM = etd.substr(etd.length() - 2);

    time_t now = time(0);

    struct tm* tmEtd = gmtime(&now);

    tmEtd->tm_hour = std::stoi(sEtdH);
    tmEtd->tm_min = std::stoi(sEtdM);

    time_t ttEtd = mktime(tmEtd);

    time_t etdP = ttEtd + 1800;
    time_t etdM = ttEtd - 1800;

    if (now <= etdM || now >= etdP) {
        now += 1800;
        struct tm* nowTm = gmtime(&now);

        std::string delayH = std::to_string(nowTm->tm_hour);
        std::string delayM = std::to_string(nowTm->tm_min);

        delayH = std::string(2 - delayH.length(), '0') + delayH;
        delayM = std::string(2 - delayM.length(), '0') + delayM;

        return delayH + delayM;

    }
    return etd;
}

