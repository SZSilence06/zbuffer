#ifndef PIPELINE_H
#define PIPELINE_H

#include "Matrix.h"
#include <vector>

#define X_MAX                640
#define Y_MAX                480
#define Z_MAX                99999999


typedef struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
}Color;

typedef struct Vertex
{
	Vector3f pos;
}Vertex;

class Triangle
{
public:
	Vertex v1,v2,v3;
	Color color;
	Triangle(){}
	Triangle(Vertex v1,Vertex v2,Vertex v3):v1(v1),v2(v2),v3(v3){}
};

class VBuffer{
private:
	Vertex* m_vbuffer;
	int m_size;
	int m_count;      //引用计数
public:
	VBuffer();
	VBuffer(int size);
	~VBuffer();
	
	friend class VertexBuffer;
};

class VertexBuffer
{
private:
	VBuffer *m_vbuffer;
	int m_size;
public:
	VertexBuffer();
	VertexBuffer(const VertexBuffer& vertexbuffer);
	~VertexBuffer();
	static int CreateVertexBuffer(int size,VertexBuffer *out);
	void SetVertexData(Vertex *v,int start,int size);
	void GetVertexData(Vertex *v,int start,int size);

	void operator =(const VertexBuffer& vertexbuffer);
	friend class Pipeline;
};

class IBuffer{
private:
	int* m_ibuffer;
	int m_size;
	int m_count;      //引用计数
public:
	IBuffer();
	IBuffer(int size);
	~IBuffer();
	
	friend class IndexBuffer;
};

class IndexBuffer
{
private:
	IBuffer *m_ibuffer;
	int m_size;
public:
	IndexBuffer();
	IndexBuffer(const IndexBuffer& indexbuffer);
	~IndexBuffer();
	static int CreateIndexBuffer(int size,IndexBuffer *out);
	void SetIndexData(int *index,int start,int size);
	void GetIndexData(int *index,int start,int size);

	void operator =(const IndexBuffer& indexbuffer);
	friend class Pipeline;
};

class Pipeline
{
private:
	Matrix m_World;
	Matrix m_Projection;
	Matrix m_View;
	Matrix m_Viewport;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	int m_Width;
	int m_Height;
	Color m_ColorBuffer[Y_MAX][X_MAX];
	float m_ZBuffer[Y_MAX][X_MAX];

public:
	enum DRAW_MODE{
		POINT,
		LINE,
		TRIANGLES,
		TRIANGLE_FAN,
		TRIANGLE_STRIP
	};

	enum TRANSFORM_TYPE{
		PROJECTION,
		VIEW,
		WORLD
	};

	enum BUFFER_TYPE{
		COLOR_BUFFER,
		DEPTH_BUFFER
	};

private:
	void TransformVertex(Vertex *v,Vertex *out);

public:
	Pipeline();
	void ClearBuffer(BUFFER_TYPE type,Color color);
	void SetVertexBuffer(VertexBuffer *vbuffer);
	void SetIndexBuffer(IndexBuffer *ibuffer);
	void SetTransform(TRANSFORM_TYPE type,Matrix *matrix);
	void SetViewport(int x,int y,int width,int height,int MaxZ,int MinZ);
	void DrawBuffer(int start,int size,DRAW_MODE mode);
	void Draw(HDC hdc);

	static void CreatePipeline(int width,int height,Pipeline *out);
};

#endif