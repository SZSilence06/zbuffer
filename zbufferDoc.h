// zbufferDoc.h : interface of the CZbufferDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFERDOC_H__039447FD_5CB1_4248_A7C1_A565D768761D__INCLUDED_)
#define AFX_ZBUFFERDOC_H__039447FD_5CB1_4248_A7C1_A565D768761D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjParser.h"



class CZbufferDoc : public CDocument
{
protected: // create from serialization only
	CZbufferDoc();
	DECLARE_DYNCREATE(CZbufferDoc)

// Attributes
public:
	ObjParser m_parser;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZbufferDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZbufferDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CZbufferDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZBUFFERDOC_H__039447FD_5CB1_4248_A7C1_A565D768761D__INCLUDED_)
