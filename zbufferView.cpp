// zbufferView.cpp : implementation of the CZbufferView class
//

#include "stdafx.h"
#include "zbuffer.h"

#include "zbufferDoc.h"
#include "zbufferView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZbufferView

IMPLEMENT_DYNCREATE(CZbufferView, CView)

BEGIN_MESSAGE_MAP(CZbufferView, CView)
	//{{AFX_MSG_MAP(CZbufferView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZbufferView construction/destruction

CZbufferView::CZbufferView()
{
	// TODO: add construction code here
	m_pDC = NULL;
}

CZbufferView::~CZbufferView()
{
}

BOOL CZbufferView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CZbufferView drawing

void CZbufferView::OnDraw(CDC* pDC)
{
	CZbufferDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	Render();
}

/////////////////////////////////////////////////////////////////////////////
// CZbufferView printing

BOOL CZbufferView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZbufferView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZbufferView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CZbufferView diagnostics

#ifdef _DEBUG
void CZbufferView::AssertValid() const
{
	CView::AssertValid();
}

void CZbufferView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZbufferDoc* CZbufferView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZbufferDoc)));
	return (CZbufferDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZbufferView message handlers

BOOL CZbufferView::Init()
{ 
	m_pDC = new CClientDC(this);
    //Failure to Get DC
    if(m_pDC == NULL)
    {
        //::MessageBox("Error Obtaining DC");
        return FALSE;
    }

    //Failure to set the pixel format
    if(!SetupPixelFormat())
    {
        return FALSE;
    }

	//Create Rendering Context
	m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());

	//Failure to Create Rendering Context
	if(m_hRC == 0)
	{
		return FALSE;
	}
	
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
	{
		return FALSE;
	}

	glClearColor(0,0,0,0);
	glClearDepth(1);

	Pipeline::CreatePipeline(X_MAX,Y_MAX,&m_pipeline);

	srand(time(NULL));

	return TRUE;
}

BOOL CZbufferView::SetupPixelFormat()
{
	 static PIXELFORMATDESCRIPTOR pfd =
	 {
		 sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
         1,                              // version number
         PFD_DRAW_TO_WINDOW |            // support window
         PFD_SUPPORT_OPENGL |            // support OpenGL
         PFD_DOUBLEBUFFER,                // double buffered
         PFD_TYPE_RGBA,                  // RGBA type
         24,                             // 24-bit color depth
         0, 0, 0, 0, 0, 0,               // color bits ignored
         0,                              // no alpha buffer
         0,                              // shift bit ignored
         0,                              // no accumulation buffer
         0, 0, 0, 0,                     // accum bits ignored
         16,                             // 16-bit z-buffer
         0,                              // no stencil buffer
         0,                              // no auxiliary buffer
         PFD_MAIN_PLANE,                 // main layer
         0,                              // reserved
         0, 0, 0                         // layer masks ignored 
	 };

	 int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	 if ( m_nPixelFormat == 0 )
	 {
		 return FALSE;
	 }
	 if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	 {
		 return FALSE;
	 }
	 return TRUE;
}

int CZbufferView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	Init();

	return 0;
}

void CZbufferView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, cx, 0, cy);

	// switch back to the modelview matrix and clear it
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CZbufferView::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Color black;
	black.r = black.b = 0;
	black.g = 0;
	m_pipeline.ClearBuffer(Pipeline::COLOR_BUFFER, black);
	m_pipeline.ClearBuffer(Pipeline::DEPTH_BUFFER, black);

	CZbufferDoc *pDoc = GetDocument();
	vector<Obj_Face> faces = pDoc->m_parser.getFaces();
	vector<Obj_Vertex> vertices = pDoc->m_parser.getVertexes();

	if(faces.size() >0 && vertices.size() >0){
		VertexBuffer vb;
		VertexBuffer::CreateVertexBuffer(vertices.size(),&vb);
		IndexBuffer ib;
		IndexBuffer::CreateIndexBuffer(faces.size()*3,&ib);

		Vertex *v = new Vertex[vertices.size()];
		int i;
		for(i=0;i<vertices.size();i++){
			Vertex temp;
			temp.pos = Vector3f(vertices[i].x,vertices[i].y,vertices[i].z);
			v[i] = temp;
		}
		vb.SetVertexData(v,0,vertices.size());

		int *index = new int[faces.size()*3];
		for(i=0;i<faces.size();i++){
			index[3*i] = faces[i].vIndexes[0] - 1;
			index[3*i+1] = faces[i].vIndexes[1] - 1;
			index[3*i+2] = faces[i].vIndexes[2] - 1;
		}
		ib.SetIndexData(index,0,faces.size()*3);

		VertexBuffer test;
		test = vb;

		m_pipeline.SetVertexBuffer(&vb);
		m_pipeline.SetIndexBuffer(&ib);

		Matrix proj = Matrix::Perspective(PI/2 , 640.0/480.0 , 1 , 1200);
		m_pipeline.SetTransform(Pipeline::PROJECTION,&proj);
		Matrix view = Matrix::LookAt(Vector3f(0,0,-2),Vector3f(0,1,0),Vector3f(0,0,1000));
		m_pipeline.SetTransform(Pipeline::VIEW,&view);

		m_pipeline.SetViewport(0,0,640,480,1000,0);

		m_pipeline.DrawBuffer(0,faces.size()*3,Pipeline::TRIANGLES);

		delete[] v;
		delete[] index;
	}
	m_pipeline.Draw(m_pDC->GetSafeHdc());
}


void CZbufferView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(wglMakeCurrent (0,0) == FALSE)
    {
        MessageBox(_T("Could not make RC non-current"));
    }

	if(wglDeleteContext (m_hRC)==FALSE)
    {
        MessageBox(_T("Could not delete RC"));
    }
	if(m_pDC)
    {
        delete m_pDC;
		m_pDC = NULL;
    }
}

void CZbufferView::DrawPixel(int x,int y,COLORREF color)
{
	GLubyte pixel[3];
	pixel[0] = GetRValue(color);
	pixel[1] = GetGValue(color);
	pixel[2] = GetBValue(color);
	glRasterPos2i(x,y);
	glDrawPixels(1,1,GL_BGR_EXT,GL_UNSIGNED_BYTE,pixel);
}
