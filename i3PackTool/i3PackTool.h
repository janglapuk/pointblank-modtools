// i3PackTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ci3PackToolApp:
// See i3PackTool.cpp for the implementation of this class
//

class Ci3PackToolApp : public CWinApp
{
public:
	Ci3PackToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ci3PackToolApp theApp;