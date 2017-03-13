#include "stdafx.h"
#include "Pipeline.h"
#include "scanline.h"
#include <vector>

using namespace std;

VBuffer::VBuffer()
{
	m_size = m_count = 0;
}

VBuffer::VBuffer(int size)
{
	m_size = size;
	m_count = 0;
	m_vbuffer = new Vertex[size];
}

VBuffer::~VBuffer()
{
	if(m_size>0){
		delete[] m_vbuffer;
	}
}

IBuffer::IBuffer()
{
	m_size = m_count = 0;
}

IBuffer::IBuffer(int size)
{
	m_size = size;
	m_count = 0;
	m_ibuffer = new int[size];
}

IBuffer::~IBuffer()
{
	if(m_size>0){
		delete[] m_ibuffer;
	}
}

/**************VertexBuffer****************************/
VertexBuffer::VertexBuffer()
{
	m_size = 0;
	m_vbuffer = new VBuffer;
	m_vbuffer->m_count++;
}

VertexBuffer::VertexBuffer(const VertexBuffer& vertexbuffer)
{
	(*this) = vertexbuffer;
}

VertexBuffer::~VertexBuffer()
{
	m_vbuffer->m_count--;
	if(m_vbuffer->m_count <=0){
		delete m_vbuffer;
	}
}

void VertexBuffer::operator =(const VertexBuffer &vertexbuffer)
{
	m_vbuffer->m_count--;
	if(m_vbuffer->m_count <=0){
		delete m_vbuffer;
	}

	m_vbuffer = vertexbuffer.m_vbuffer;
	m_size = vertexbuffer.m_size;
	m_vbuffer->m_count++;
}

int VertexBuffer::CreateVertexBuffer(int size,VertexBuffer *out)
{
	out->m_vbuffer->m_count--;
	if(out->m_vbuffer->m_count<=0){
		delete out->m_vbuffer;
	}
	out->m_vbuffer = new VBuffer(size);
	out->m_vbuffer->m_count++;
	out->m_size = size;
	return 1;
}

void VertexBuffer::SetVertexData(Vertex *v,int start,int size)
{
	ASSERT(start>=0);
	ASSERT(start<m_size);
	ASSERT(start+size-1<m_size);

	memcpy(m_vbuffer->m_vbuffer+start,v,size*sizeof(Vertex));
}

void VertexBuffer::GetVertexData(Vertex *v,int start,int size)
{
	ASSERT(start>=0);
	ASSERT(start<m_size);
	ASSERT(start+size-1<m_size);

	memcpy((void*)v,m_vbuffer->m_vbuffer+start,size*sizeof(Vertex));
}

/*************IndexBuffer******************************/
IndexBuffer::IndexBuffer()
{
	m_size = 0;
	m_ibuffer = new IBuffer;
	m_ibuffer->m_count++;
}

IndexBuffer::IndexBuffer(const IndexBuffer& indexbuffer)
{
	(*this) = indexbuffer;
}

IndexBuffer::~IndexBuffer()
{
	m_ibuffer->m_count--;
	if(m_ibuffer->m_count <=0){
		delete m_ibuffer;
	}
}

void IndexBuffer::operator =(const IndexBuffer &indexbuffer)
{
	m_ibuffer->m_count--;
	if(m_ibuffer->m_count <=0){
		delete m_ibuffer;
	}

	m_ibuffer = indexbuffer.m_ibuffer;
	m_size = indexbuffer.m_size;
	m_ibuffer->m_count++;
}

int IndexBuffer::CreateIndexBuffer(int size,IndexBuffer *out)
{
	out->m_ibuffer->m_count--;
	if(out->m_ibuffer->m_count<=0){
		delete out->m_ibuffer;
	}
	out->m_ibuffer = new IBuffer(size);
	out->m_ibuffer->m_count++;
	out->m_size = size;
	return 1;
}

void IndexBuffer::SetIndexData(int *index,int start,int size)
{
	ASSERT(start>=0);
	ASSERT(start<m_size);
	ASSERT(start+size-1<m_size);

	memcpy(m_ibuffer->m_ibuffer+start,index,size*sizeof(int));
}

void IndexBuffer::GetIndexData(int *index,int start,int size)
{
	ASSERT(start>=0);
	ASSERT(start<m_size);
	ASSERT(start+size-1<m_size);

	memcpy((void*)index,m_ibuffer->m_ibuffer+start,size*sizeof(int));
}

/**************************Pipeline****************************************/
Pipeline::Pipeline()
{
	m_Width = m_Height = 0;
}

void Pipeline::CreatePipeline(int width,int height,Pipeline *out)
{
	ASSERT(width >=0 );
	ASSERT(height >=0 );
	ASSERT(width <= X_MAX);
	ASSERT(height <= Y_MAX);

	out->m_Width = width;
	out->m_Height = height;

	out->m_World = out->m_Projection = out->m_View = out->m_Viewport = Matrix::Identity();
}

void Pipeline::ClearBuffer(BUFFER_TYPE type,Color color)
{
	int i,j;
	switch(type){
	case COLOR_BUFFER:
		for(i=0;i<m_Height;i++){
			for(j=0;j<m_Width;j++){
				m_ColorBuffer[i][j] = color;
			}
		}
		break;
	case DEPTH_BUFFER:
		for(i=0;i<m_Height;i++){
			for(j=0;j<m_Width;j++){
				m_ZBuffer[i][j] = Z_MAX;
			}
		}
	}
}

void Pipeline::SetVertexBuffer(VertexBuffer *vbuffer)
{
	m_VertexBuffer = *vbuffer;
}

void Pipeline::SetIndexBuffer(IndexBuffer *ibuffer)
{
	m_IndexBuffer = *ibuffer;
}

void Pipeline::SetViewport(int x,int y,int width,int height,int MaxZ,int MinZ)
{
	m_Viewport = Matrix::Viewport(x,y,width,height,MinZ,MaxZ);
}

void Pipeline::SetTransform(TRANSFORM_TYPE type,Matrix *matrix)
{
	switch(type){
	case PROJECTION:
		m_Projection = *matrix;
		break;
	case VIEW:
		m_View = *matrix;
		break;
	case WORLD:
		m_World = *matrix;
		break;
	default:
		ASSERT(0);
	}
}

void Pipeline::TransformVertex(Vertex *v,Vertex *out)
{
	Vector4f vp =  Vector4f(v->pos.x, v->pos.y, v->pos.z, 1.0f);
	vp = vp * m_World * m_View * m_Projection * m_Viewport;

	Matrix m;
	m._data[0][0] = 1;
	m._data[2][2] = 1;
	m._data[3][3] = 1;
	m._data[1][1] = -1;
	m._data[3][1] = m_Height;

	vp = vp * m;
	*out = *v;
	out->pos.x = vp._data[0]/vp._data[3];
	out->pos.y = vp._data[1]/vp._data[3];
	out->pos.z = vp._data[2]/vp._data[3];
}

void Pipeline::DrawBuffer(int start,int size,DRAW_MODE mode)
{
	vector<Triangle> triangles;
	switch(mode){
	case TRIANGLES:
		{
			int i = 0;
			Vertex v[3];
			Vertex vv[3];
			int index[3];
			for(i=0;i+2<=size;i+=3){
				ASSERT(start+2 < m_IndexBuffer.m_size && start >=0);
				m_IndexBuffer.GetIndexData(index,start+i,3);
				ASSERT(index[0] < m_VertexBuffer.m_size && index[0]>=0);
				m_VertexBuffer.GetVertexData(v,index[0],1);

				ASSERT(index[1] < m_VertexBuffer.m_size && index[1]>=0);
				m_VertexBuffer.GetVertexData(v+1,index[1],1);

				ASSERT(index[2] < m_VertexBuffer.m_size && index[2]>=0);
				m_VertexBuffer.GetVertexData(v+2,index[2],1);

				TransformVertex(&v[0],&vv[0]);
				TransformVertex(&v[1],&vv[1]);
				TransformVertex(&v[2],&vv[2]);
				
				Triangle tri(vv[0],vv[1],vv[2]);
				triangles.push_back(tri);
			}
		}
	}
	Scan(triangles,(Color*)m_ColorBuffer,(int*)m_ZBuffer,m_Width,m_Height);
	int test = 0;
}

void Pipeline::Draw(HDC hdc)
{
	glRasterPos2i(0,0);
	glDrawPixels(m_Width,m_Height,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *)m_ColorBuffer);

	glFlush();
	SwapBuffers(hdc);
}