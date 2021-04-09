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
        
        CheckEtd(FlightPlan);

    }
    
          
}

void ACDMPlugin::OnTimer(int Counter) {
    DisplayUserMessage("A-CDM", "A-CDM", "beep boop", TRUE, TRUE, TRUE, TRUE, FALSE);
}




void ACDMPlugin::CheckEtd(CFlightPlan FlightPlan)
{
    std::string cs = FlightPlan.GetCallsign();    
    std::string etd = FlightPlan.GetFlightPlanData().GetEstimatedDepartureTime();
    std::string rmk = FlightPlan.GetFlightPlanData().GetRemarks();
    
    std::string sDof = "DOF/990101";
    std::string dofY;
    std::string dofM;
    std::string dofD;

    etd = std::string(4 - etd.length(), '0') + etd;

    std::string sEtdH = etd.substr(0, 2);
    std::string sEtdM = etd.substr(2);

    int dofPos = 0;
    std::smatch base_match;

    if (std::regex_search(rmk, base_match, REGEX_DOF)) {
       dofPos = base_match.position(0);
       sDof = base_match[0].str();
       rmk = rmk.substr(0, dofPos) + rmk.substr((dofPos + 10));
    }

    dofY = sDof.substr(4, 2);
    dofM = sDof.substr(6, 2);
    dofD = sDof.substr(8, 2);

    struct tm tmEtd = { 0 };

    tmEtd.tm_year = std::stoi("20" + dofY) - 1900;
    tmEtd.tm_mon = std::stoi(dofM) - 1;
    tmEtd.tm_mday = std::stoi(dofD);
    tmEtd.tm_hour = std::stoi(sEtdH);
    tmEtd.tm_min = std::stoi(sEtdM);

    time_t ttEtd = mktime(&tmEtd);
    time_t etdP = ttEtd + ACDM_TOBT_LATEST;
    time_t etdM = ttEtd - ACDM_TOBT_EARLIEST;
    time_t now = time(0);

    if (now <= etdM || now >= etdP) {
        now += ACDM_TOBT_LATEST;
        struct tm* nowTm = gmtime(&now);

        std::string delayH = std::to_string(nowTm->tm_hour);
        std::string delayM = std::to_string(nowTm->tm_min);

        delayH = std::string(2 - delayH.length(), '0') + delayH;
        delayM = std::string(2 - delayM.length(), '0') + delayM;

        std::string newDofY = std::to_string(nowTm->tm_year + 1900 - 2000);
        std::string newDofM = std::to_string(nowTm->tm_mon + 1);
        std::string newDofD = std::to_string(nowTm->tm_mday);

        newDofY = std::string(2 - newDofY.length(), '0') + newDofY;
        newDofM = std::string(2 - newDofM.length(), '0') + newDofM;
        newDofD = std::string(2 - newDofD.length(), '0') + newDofD;

        std::string newDof = "DOF/" + newDofY + newDofM + newDofD;

        rmk = rmk.substr(0, dofPos) + newDof + rmk.substr((dofPos));

        if (rmk.compare(dofPos + 10, 1, " ") != 0) {
            rmk = rmk.substr(0, dofPos + 10) + " " +rmk.substr(dofPos + 10);
        }

        FlightPlan.GetFlightPlanData().SetEstimatedDepartureTime( (delayH + delayM).c_str() );
        FlightPlan.GetFlightPlanData().SetRemarks(rmk.c_str() );
        FlightPlan.GetFlightPlanData().AmendFlightPlan();
    }
}

