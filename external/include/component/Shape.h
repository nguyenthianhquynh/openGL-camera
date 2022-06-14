//
//  Shape.hpp
//  materialCube
//
//  Created by anh quynh on 11/06/2022.
//

#ifndef Shape_hpp
#define Shape_hpp

#include <stdio.h>
#include <header/shader.h>

class Shape {
public:
    Shader* shader;
    unsigned int VBO, VAO;
    int numOfVertexunsigned;

public:
    //contructor
    Shape(Shader* shader, float vertices[], int numOfElement,int numOfVertexunsigned);
    
    virtual void Draw(unsigned int idTexture, glm::vec3 model_trans);
};

#endif /* Shape_hpp */
