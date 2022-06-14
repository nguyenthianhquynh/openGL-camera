//
//  cube.hpp
//  materialCube
//
//  Created by anh quynh on 11/06/2022.
//

#ifndef cube_hpp
#define cube_hpp

#include <stdio.h>
#include <header/shader.h>
#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned);
    
    //void drawCube(unsigned int idTx,glm::vec3 model_trans);
    
    void updateCude();
};

#endif /* cube_hpp */
