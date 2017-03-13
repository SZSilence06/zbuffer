#ifndef SCANLINE_H
#define SCANLINE_H

#include <vector>
#include <list>
#include "Pipeline.h"

using namespace std;

typedef struct{
	float a,b,c,d;
	int id;
	int dy;
	Color color;
}ZB_Polygon;

typedef struct{
	float x;
	float dx;
	int dy;
	int id;
}ZB_Edge;

typedef struct{
	float a,b,c,d;
	int id;
	int dy;
	Color color;
}ZB_ActivePolygon;

typedef struct{
	float xl,xr;
	float dxl,dxr;
	int dyl,dyr;
	float zl;
	float dzx;
	float dzy;
	int id;
}ZB_ActiveEdge;


void Init(vector<Triangle>& faces, vector< list< ZB_Polygon > >& Polygon_list,	vector< list<ZB_Edge> >& Edge_list);
void Scan(vector<Triangle> faces,Color* ColorBuffer,int* ZBuffer,int width,int height);
void UpdateActive(	vector< list<ZB_Polygon> > &Polygon_list,
	vector< list<ZB_Edge> > &Edge_list,
	list<ZB_ActivePolygon> &AcPoly_list,
	list<ZB_ActiveEdge> &AcEdge_list,
	int current
	);
void UpdateBuffer(vector< list<ZB_Polygon> > &Polygon_list,
	vector< list<ZB_Edge> > &Edge_list,
	list<ZB_ActivePolygon> &AcPoly_list,
	list<ZB_ActiveEdge> &AcEdge_list,
	int y,
	int width,
	Color *ColorBuffer,
	int *ZBuffer
	);

#endif