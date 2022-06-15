//
//  components.cpp
//  Color-OpenGL
//
//  Created by anh quynh on 14/06/2022.
//

#include "components.h"

#include <header/shader.h>
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//method
Component::Component(Shader* shader, float vertices[], int numOfElement, int numOfVertexunsigned) : shader(shader)
{
    cout<<"contruct SHADE" << endl;
    
    this->shader = shader;
    this->numOfVertexunsigned = numOfVertexunsigned;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,numOfElement * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
       // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Component::Draw(unsigned int idTexture, glm::vec3 model_trans, glm::vec3 scale)
{
    shader->use();
    
    // create transformations
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    model = glm::translate(model, model_trans);
    model = glm::scale(model, scale);

    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    
    //bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTexture);
//
    cout<< "this->numOfVertexunsigned: " << this->numOfVertexunsigned <<endl;
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numOfVertexunsigned);
}
