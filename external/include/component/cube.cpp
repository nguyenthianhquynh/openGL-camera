//
//  cube.cpp
//  materialCube
//
//  Created by anh quynh on 11/06/2022.
//

#include "cube.h"
using namespace std;

Cube::Cube(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned) : Shape(shader,vertices,numOfElement,numOfVertexunsigned)
{
    cout<<"NEW CUBE" << endl;

}

void Cube::updateCude(){

}

