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
#include "components.h"

class Cube : public Component
{
public:
    Cube(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned);
};

#endif /* cube_hpp */
