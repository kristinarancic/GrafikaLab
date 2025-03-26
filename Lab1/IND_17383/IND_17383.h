
// IND_17383.h : main header file for the IND_17383 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND17383App:
// See IND_17383.cpp for the implementation of this class
//

class CIND17383App : public CWinApp
{
public:
	CIND17383App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND17383App theApp;
