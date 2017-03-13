// zbuffer.h : main header file for the ZBUFFER application
//

#if !defined(AFX_ZBUFFER_H__039BC9C7_285E_48A6_8687_B84FEFB447CA__INCLUDED_)
#define AFX_ZBUFFER_H__039BC9C7_285E_48A6_8687_B84FEFB447CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CZbufferApp:
// See zbuffer.cpp for the implementation of this class
//

class CZbufferApp : public CWinApp
{
public:
	CZbufferApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZbufferApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CZbufferApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZBUFFER_H__039BC9C7_285E_48A6_8687_B84FEFB447CA__INCLUDED_)
