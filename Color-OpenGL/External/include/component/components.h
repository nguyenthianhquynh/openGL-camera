//
//  components.hpp
//  Color-OpenGL
//
//  Created by anh quynh on 14/06/2022.
//

#ifndef components_hpp
#define components_hpp

#include <stdio.h>

#include <header/shader.h>

class Component {
public:
    Shader* shader;
    unsigned int VBO, VAO;
    int numOfVertexunsigned;

public:
    //contructor
    Component(Shader* shader, float vertices[], int numOfElement,int numOfVertexunsigned);
    
    virtual void Draw(unsigned int idTexture, glm::vec3 model_trans, glm::vec3 scale = glm::vec3(1.0f));
};

#endif /* components_hpp */
