#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <vector>
using namespace std;

class Obj_Face;
class ObjParser;

class Obj_Vertex{
public:
    double x,y,z;
public:
    //constructors
    Obj_Vertex(double x,double y,double z):x(x),y(y),z(z){}

   friend class ObjParser;
   friend class Obj_Face;
};

class Obj_Face{
public:
    vector<int>vIndexes;
public:
    //constructors
    Obj_Face(){}

    //methods
    void setVIndexes(vector<int>vIndexes){this ->vIndexes = vIndexes;}
    int getCountOfVertexes(){return vIndexes.size();}

    friend class ObjParser;
};

class ObjParser{
private:
    vector<Obj_Vertex> vertexes;
    vector<Obj_Face> faces;
public:
    //constructors
    ObjParser();

    //methods
    vector<Obj_Vertex> getVertexes();
    vector<Obj_Face> getFaces();
    void readObjFile(string file_path);
};

#endif // OBJPARSER_H