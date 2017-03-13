#include "stdafx.h"
#include  "ObjParser.h"
#include <stdio.h>
#include <strstream>

ObjParser::ObjParser(){}

vector<Obj_Vertex> ObjParser::getVertexes(){
    return vertexes;
}

vector<Obj_Face> ObjParser::getFaces(){
    return faces;
}

void ObjParser::readObjFile(string file_path){
    FILE *file = fopen(file_path.c_str(),"r");
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    vertexes.clear();
    faces.clear();

    if(file == NULL){
		AfxMessageBox(TEXT("Open File Failed!"));
    }

    while(fgets(buf,BUF_SIZE,file) && (!feof(file))){
        if(buf[0] == 'v'){
            if(buf[1] == ' '  ||  buf[1] == '\t'){
                double x,y,z;
                sscanf(buf+2,"%lf %lf %lf",&x,&y,&z);
                Obj_Vertex v(x,y,z);
                vertexes.push_back(v);
            }
        }
        else if(buf[0] == 'f'){
            if(buf[1] == ' '  ||  buf[1] == '\t'){
                vector<int> vIndexes;
                int index;
                istrstream strio(buf+2,BUF_SIZE-2);
                while(strio>>index){
                    if(index > 0){
                        vIndexes.push_back(index);
                    }
                    else if(index<0){
                        index = vertexes.size() + index;
                        if(index<0){
                           AfxMessageBox(TEXT("Invalid file!"));
                        }
                        vIndexes.push_back(index);
                    }
                    else{
                        AfxMessageBox(TEXT("Invalid file!"));
                    }
                }
                Obj_Face face;
                face.setVIndexes(vIndexes);
                faces.push_back(face);
            }
        }
    }
}