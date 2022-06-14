//
//  Triangle.cpp
//  materialCube
//
//  Created by anh quynh on 11/06/2022.
//

#include "Triangle.h"
#include <header/shader.h>

using namespace std;

Triangle::Triangle(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned = 3) : Shape(shader,vertices,numOfElement,numOfVertexunsigned)
{
    //cout<<"CONTRUCTOR" << test << endl;
}

void Triangle::drawTri(glm::vec3 model_trans) {
    unsigned int i = 0;
    Draw(i,model_trans);
}
