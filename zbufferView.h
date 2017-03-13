// zbufferView.h : interface of the CZbufferView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFERVIEW_H__71127D98_9DF6_4FCE_95FA_09540953F13B__INCLUDED_)
#define AFX_ZBUFFERVIEW_H__71127D98_9DF6_4FCE_95FA_09540953F13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pipeline.h"

class CZbufferView : public CView
{
protected: // create from serialization only
	CZbufferView();
	DECLARE_DYNCREATE(CZbufferView)

// Attributes
public:
	CZbufferDoc* GetDocument();
	HGLRC m_hRC;
	CDC* m_pDC;                 //Device Context…Ë±∏√Ë ˆ±Ì
    BOOL Init();                //Initialize OpenGL
    BOOL SetupPixelFormat();    //Set up the Pixel Format
    void Render();            //Render the Scene
	void DrawPixel(int x,int y,COLORREF color);

	Pipeline m_pipeline;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZbufferView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZbufferView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CZbufferView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in zbufferView.cpp
inline CZbufferDoc* CZbufferView::GetDocument()
   { return (CZbufferDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZBUFFERVIEW_H__71127D98_9DF6_4FCE_95FA_09540953F13B__INCLUDED_)
