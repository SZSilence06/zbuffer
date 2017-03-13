// zbufferDoc.cpp : implementation of the CZbufferDoc class
//

#include "stdafx.h"
#include "zbuffer.h"

#include "zbufferDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZbufferDoc

IMPLEMENT_DYNCREATE(CZbufferDoc, CDocument)

BEGIN_MESSAGE_MAP(CZbufferDoc, CDocument)
	//{{AFX_MSG_MAP(CZbufferDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZbufferDoc construction/destruction

CZbufferDoc::CZbufferDoc()
{
	// TODO: add one-time construction code here

}

CZbufferDoc::~CZbufferDoc()
{
}

BOOL CZbufferDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CZbufferDoc serialization

void CZbufferDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		CFile *file = ar.GetFile();
		std::string path = file->GetFilePath().GetBuffer(0);
		m_parser.readObjFile(path);


	}
}

/////////////////////////////////////////////////////////////////////////////
// CZbufferDoc diagnostics

#ifdef _DEBUG
void CZbufferDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZbufferDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZbufferDoc commands
