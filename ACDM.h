// ACDM.h : main header file for the ACDM DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CACDMApp
// See ACDM.cpp for the implementation of this class
//

class CACDMApp : public CWinApp
{
public:
	CACDMApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
