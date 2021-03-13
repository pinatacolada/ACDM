#pragma once
#include "EuroScopePlugIn.h"
#include <ctime>
#include <chrono>
#include <string>
#include <list>

using namespace EuroScopePlugIn;

class ACDMPlugin : public CPlugIn
{
	public:
		ACDMPlugin();
		virtual ~ACDMPlugin();
        std::chrono::system_clock::time_point nextTick;
        CFlightPlanList depSeq;
        std::list<std::string> cdmSeq;

        void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
            EuroScopePlugIn::CRadarTarget RadarTarget,
            int ItemCode,
            int TagData,
            char sItemString[16],
            int* pColorCode,
            COLORREF* pRGB,
            double* pFontSize);
        
        std::string CheckEtd(CFlightPlan FlightPlan);

        
};



