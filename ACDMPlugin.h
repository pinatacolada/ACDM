#pragma once
#include "EuroScopePlugIn.h"

using namespace EuroScopePlugIn;

class ACDMPlugin : public CPlugIn
{
	public:
		ACDMPlugin();
		virtual ~ACDMPlugin();

        void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
            EuroScopePlugIn::CRadarTarget RadarTarget,
            int ItemCode,
            int TagData,
            char sItemString[16],
            int* pColorCode,
            COLORREF* pRGB,
            double* pFontSize);
};

