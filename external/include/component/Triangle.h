//
//  Triangle.hpp
//  materialCube
//
//  Created by anh quynh on 11/06/2022.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include <header/shader.h>

#include "Shape.cpp"

class Triangle : public Shape
{
public:
    Triangle(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned);
    
    void drawTri(glm::vec3 model_trans);
};

#endif /* Triangle_hpp */
