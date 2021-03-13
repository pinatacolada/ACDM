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
	// Register tag function
	RegisterTagItemType("ACDM Dummy Item", TAG_ITEM_ACDM_DUMMY);
    nextTick = std::chrono::system_clock::now() + std::chrono::seconds(5);
    //depSeq = RegisterFpList("Departure Sequence");
    //depSeq.AddColumnDefinition("CALLSIGN", 7, TRUE, "", TAG_ITEM_ACDM_DUMMY, NULL, NULL, NULL, NULL);
    //depSeq.ShowFpList(TRUE);

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

    //auto tick = std::chrono::system_clock::now();
    if (std::chrono::system_clock::now() >= nextTick) {
        nextTick = std::chrono::system_clock::now() + std::chrono::seconds(5);





        std::string adep = FlightPlan.GetFlightPlanData().GetOrigin();
        to_upper(adep);

        if (adep.compare("LPPT") == 0) {
            
            FlightPlan.GetFlightPlanData().SetEstimatedDepartureTime(CheckEtd(FlightPlan).c_str());


            /*std::list<std::string>::iterator it;
            for (it = cdmSeq.begin(); it != cdmSeq.end(); it++) {
                if(*it)
            }*/

            /*const char* etd = FlightPlan.GetFlightPlanData().GetEstimatedDepartureTime();
            const char* callsign = FlightPlan.GetCallsign();

            char text[17];

            strcpy(text, callsign);
            strcat(text, " ETD: ");
            strcat(text, etd);

            DisplayUserMessage("A-CDM", "A-CDM", text, TRUE, TRUE, TRUE, TRUE, FALSE);*/

            //depSeq.AddFpToTheList(FlightPlan);
        }
    }
          
}

std::string ACDMPlugin::CheckEtd(CFlightPlan FlightPlan)
{
    std::string cs = FlightPlan.GetCallsign();

    time_t now = time(0); 
    
    std::string etd = FlightPlan.GetFlightPlanData().GetEstimatedDepartureTime();
 
    //std::string padEtd = etd;
    std::string padEtd = std::string(4 - etd.length(), '0') + etd;

    std::string sEtdH = padEtd.substr(0, 2);
    std::string sEtdM = padEtd.substr(padEtd.length() - 2);
    

    struct tm fplEtd = { 0 };

    fplEtd.tm_hour = std::stoi(sEtdH);
    fplEtd.tm_min = std::stoi(sEtdM);

    time_t fplTime = mktime(&fplEtd);

    time_t etdP30 = fplTime + 1800;
    time_t etdM30 = fplTime - 1800;

    

    if (now <= etdM30 || now >= etdP30) {
        now += 1800;
        struct tm* nowTm = gmtime(&now);


        std::string delayH = std::to_string(nowTm->tm_hour);
        std::string delayM = std::to_string(nowTm->tm_min);

        delayH = std::string(2 - delayH.length(), '0') + delayH;
        delayM = std::string(2 - delayM.length(), '0') + delayM;



        std::string msgFinal = "NEW TSAT " + cs +": "+ delayH + delayM;
        DisplayUserMessage("A-CDM", "A-CDM", msgFinal.c_str(), TRUE, TRUE, TRUE, TRUE, FALSE);

        return delayH + delayM;

    }
    DisplayUserMessage("A-CDM", "A-CDM", "NO CHANGE", TRUE, TRUE, TRUE, TRUE, FALSE);
    return padEtd;
}

