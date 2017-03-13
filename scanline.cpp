#include "stdafx.h"
#include "scanline.h"

float Max(float a,float b,float c)
{
	if(a>b && a>c)
		return a;
	return b>c?b:c;
}

float Min(float a,float b,float c)
{
	if(a<b && a<c)
		return a;
	return b<c?b:c;
}

void Scan(vector<Triangle> faces,Color* ColorBuffer,int* ZBuffer,int width,int height)
{
	vector< list<ZB_Polygon> > Polygon_list;
	vector< list<ZB_Edge> >Edge_list;
	list<ZB_ActivePolygon> AcPoly_list;
	list<ZB_ActiveEdge> AcEdge_list;

	int i;
	for(i=0;i<height;i++){
		list<ZB_Polygon> l1;
		Polygon_list.push_back(l1);

		list<ZB_Edge> l2;
		Edge_list.push_back(l2);
	}

	Init(faces,Polygon_list,Edge_list);

	for(i=0;i<height;i++){
		UpdateActive(Polygon_list,Edge_list,AcPoly_list,AcEdge_list,i);
		if( i == 300){
			int test = 1;
		}
		UpdateBuffer(Polygon_list,Edge_list,AcPoly_list,AcEdge_list,i,width,ColorBuffer,ZBuffer);
	}
}

void Init(vector<Triangle>& faces, vector< list< ZB_Polygon > >& Polygon_list,	vector< list<ZB_Edge> >& Edge_list)
{
	int i;
	for(i=0;i<faces.size();i++){
		Triangle tri = faces[i];
		float x1 = tri.v1.pos.x;
		float y1 = tri.v1.pos.y;
		float z1 = tri.v1.pos.z;
		float x2 = tri.v2.pos.x;
		float y2 = tri.v2.pos.y;
		float z2 = tri.v2.pos.z;
		float x3 = tri.v3.pos.x;
		float y3 = tri.v3.pos.y;
		float z3 = tri.v3.pos.z;

		Vector3f v1(x1,y1,z1);
		Vector3f v2(x2,y2,z2);
		Vector3f v3(x3,y3,z3);

		Vector3f v12 = v2 - v1;
		Vector3f v13 = v3 - v1;
		Vector3f v23 = v3 - v2;
		Vector3f fn = Vector3f::Cross(v12,v13);
		float d = fn.x * x1 + fn.y * y1 +fn.z * z1;

		Color red;
		red.r = rand()%256;
		red.g = rand()%256;
		red.b = rand()%256;

		float maxy = Max(y1,y2,y3);
		float miny = Min(y1,y2,y3);

		ZB_Polygon face;
		face.a = fn.x;
		face.b = fn.y;
		face.c = fn.z;
		face.d = -d;
		face.color = red;
		face.id = i;
		face.dy = (int)(maxy+0.5) - (int)(miny+0.5);

		if(face.dy > 0){
			if((int)(miny+0.5) >=0 && (int)(miny+0.5) < Y_MAX){
				Polygon_list[(int)(miny+0.5)].push_back(face);
			}
		}

		ZB_Edge e12,e13,e23;
		if(y1 < y2){
			e12.x = x1;
			e12.dy = (int)(y2+0.5) - (int)(y1+0.5);
		}
		else{
			e12.x = x2;
			e12.dy = (int)(y1+0.5) - (int)(y2+0.5);
		}
		if(v12.y){
			e12.dx = v12.x / v12.y;
		}
		else{
			e12.dx = 999999;
		}
		e12.id = i;
		if(e12.dy > 0){
			int ypos = (int)(min(y1,y2)+0.5);
			if(ypos >=0 && ypos < Y_MAX){
				Edge_list[ypos].push_back(e12);
			}
		}

		if(y1 < y3){
			e13.x = x1;
			e13.dy = (int)(y3+0.5) - (int)(y1+0.5);
		}
		else{
			e13.x = x3;
			e13.dy = (int)(y1+0.5) - (int)(y3+0.5);
		}
		if(v13.y){
			e13.dx = v13.x / v13.y;
		}
		else{
			e13.dx = 999999;
		}
		e13.id = i;
		if(e13.dy > 0){
			int ypos = (int)(min(y1,y3)+0.5);
			if(ypos >= 0 && ypos < Y_MAX){
				Edge_list[ypos].push_back(e13);
			}
		}

		if(y2 < y3){
			e23.x = x2;
			e23.dy = (int)(y3+0.5) - (int)(y2+0.5);
		}
		else{
			e23.x = x3;
			e23.dy = (int)(y2+0.5) - (int)(y3+0.5);
		}
		if(v23.y){
			e23.dx = v23.x / v23.y;
		}
		else{
			e23.dx = 999999;
		}
		e23.id = i;
		if(e23.dy > 0){
			int ypos = (int)(min(y2,y3)+0.5);
			if(ypos >=0 && ypos < Y_MAX){
				Edge_list[ypos].push_back(e23);
			}
		}
	}
}

void UpdateActive(	vector< list<ZB_Polygon> > &Polygon_list,
	vector< list<ZB_Edge> > &Edge_list,
	list<ZB_ActivePolygon> &AcPoly_list,
	list<ZB_ActiveEdge> &AcEdge_list,
	int current
	)
{
	list<ZB_Polygon>::iterator iter;
	for(iter = Polygon_list[current].begin();iter!=Polygon_list[current].end();iter++){
		ZB_Polygon poly = *iter;
		ZB_ActivePolygon ac_poly;
			
		ac_poly.a = poly.a;
		ac_poly.b = poly.b;
		ac_poly.c = poly.c;
		ac_poly.d = poly.d;
		ac_poly.color = poly.color;
		ac_poly.id = poly.id;
		ac_poly.dy = poly.dy;

		AcPoly_list.push_back(ac_poly);

		int j=0;
		ZB_Edge edge[2];
			
		list<ZB_Edge>::iterator iter2;
		for(iter2 = Edge_list[current].begin();iter2!=Edge_list[current].end();iter2++){
			ZB_Edge e = *iter2;
			if(e.id != ac_poly.id){
				continue;
			}
			edge[j++] = e;
		}
		ZB_ActiveEdge ac_edge;
		ac_edge.dzx = -ac_poly.a/ac_poly.c;
		ac_edge.dzy = -ac_poly.b/ac_poly.c;
		ac_edge.id = ac_poly.id;
		if(edge[1].x < edge[0].x || (edge[1].x == edge[0].x && edge[0].dx >= edge[1].dx)){
			ac_edge.xl = edge[1].x;
			ac_edge.dxl = edge[1].dx;
			ac_edge.dyl = edge[1].dy;
			ac_edge.zl = (ac_poly.a*ac_edge.xl + ac_poly.b * current + ac_poly.d)/(-ac_poly.c);
			ac_edge.xr = edge[0].x;
			ac_edge.dxr = edge[0].dx;
			ac_edge.dyr = edge[0].dy;
		}
		else if(edge[0].x < edge[1].x || (edge[0].x == edge[1].x && edge[0].dx <= edge[1].dx)){
			ac_edge.xl = edge[0].x;
			ac_edge.dxl = edge[0].dx;
			ac_edge.dyl = edge[0].dy;
			ac_edge.zl = (ac_poly.a*ac_edge.xl + ac_poly.b * current + ac_poly.d)/(-ac_poly.c);
			ac_edge.xr = edge[1].x;
			ac_edge.dxr = edge[1].dx;
			ac_edge.dyr = edge[1].dy;
		}

		AcEdge_list.push_back(ac_edge);
	}
}

void UpdateBuffer(  vector< list<ZB_Polygon> > &Polygon_list,
	vector< list<ZB_Edge> > &Edge_list,
	list<ZB_ActivePolygon> &AcPoly_list,
	list<ZB_ActiveEdge> &AcEdge_list,
	int y,
	int width,
	Color *ColorBuffer,
	int *ZBuffer
	)
{	
	list<ZB_ActiveEdge>::iterator iter;
	list<ZB_ActivePolygon>::iterator iter2;
	for(iter=AcEdge_list.begin();iter!=AcEdge_list.end();iter++){
		if(AcEdge_list.empty()){
			break;
		}
		ZB_ActiveEdge edge = *iter;
		for(iter2 = AcPoly_list.begin();iter2!=AcPoly_list.end();iter2++){
			if(iter2->id == edge.id){
				break;
			}
		}
		int x;
		float z = edge.zl;
		for(x = edge.xl; x <= edge.xr; x++){
			if( x < 0 || x >= X_MAX || y<0 || y>=Y_MAX){
				continue;
			}
			if(*(ZBuffer + y*width + x) > z){
				*(ZBuffer + y*width + x) = z;
				*(ColorBuffer +y*width +x) = iter2->color;
			}
			z += edge.dzx;
		}

		iter2->dy--;
		if(iter2->dy <= 0){
			AcPoly_list.erase(iter2);
			AcEdge_list.erase(iter++);
			iter--;
		}
		else{
			iter->dyl--;
			iter->dyr--;
			if(iter->dyl <= 0 && y+1 < Y_MAX){
				list<ZB_Edge>::iterator iter3;
				for(iter3 = Edge_list[y+1].begin();iter3!=Edge_list[y+1].end();iter3++){
					if(iter3->id == iter->id){						
						iter->dxl = iter3->dx;
						iter->dyl = iter3->dy;
						iter->xl = iter3->x;
						iter->zl = (iter2->a * iter->xl + iter2->b * y + iter2->d)/(-iter2->c);
						break;
					}
				}
			}
			else if(iter->dyr <= 0 && y+1 < Y_MAX){
				list<ZB_Edge>::iterator iter3;
				for(iter3 = Edge_list[y+1].begin();iter3!=Edge_list[y+1].end();iter3++){
					if(iter3->id == iter->id){
						iter->zl += iter->dzx * iter->dxl + iter->dzy;
						iter->dxr = iter3->dx;
						iter->dyr = iter3->dy;
						iter->xr = iter3->x;
						break;
					}
				}
			}
			else{
				iter->xl += iter->dxl;
				iter->xr += iter->dxr;
				iter->zl += iter->dzx * iter->dxl + iter->dzy;
			}
		}
	}
}
