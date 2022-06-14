#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <header/model.h>
#include <header/shader.h>
#include <header/camera.h>
#include <header/constant.h>

#include "component/Triangle.cpp"
#include "component/cube.cpp"
#include <iostream>

#include <list>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void Update();

unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


glm::mat4 modelCube = glm::mat4(1.0f);
glm::mat4 modelTempCube = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);

// camera
glm::vec3 positionCamera = glm::vec3(0.0f, 0.0f, 5.0f);
Camera camera(positionCamera);

glm::vec3 positionCharacter =  glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 positionGrass =  glm::vec3(0.0f,-4.0f, 0.0f);


float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    //list<Shape> shapes;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    unsigned int diffuseMap = loadTexture("./src/img/container001-red-small.png");
    unsigned int diffuseMap1 = loadTexture("./src/img/grass.png");
    
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("./src/shader/cube.vs", "./src/shader/cube.fs");
    Shader lightCubeShader("./src/shader/light.vs", "./src/shader/light.fs");
    Shader grassShader("./src/shader/light.vs", "./src/shader/light.fs");
    Shader shader("./src/shader/2D.vs", "./src/shader/2D.fs");
    Shader shaderTri("./src/shader/2D.vs", "./src/shader/2D.fs");
    Shader shaderCubeBasic("./src/shader/CubeBasic.vs", "./src/shader/CubeBasic.fs");


    //=========
    Triangle *tri = new Triangle(&shaderTri,verticesTri,sizeof(vertices)/sizeof(vertices[0]),3);
    Cube *cubeBasic = new Cube(&shaderCubeBasic,vertices,sizeof(vertices)/sizeof(vertices[0]),36);

    std::vector<Shape*> cubes;
    cubes.push_back(cubeBasic);
    cubes.push_back(cubeBasic);
    cubes.push_back(cubeBasic);


    
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texCoords attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


   //====================
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    model = glm::translate(model, positionGrass);
    model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f)); //

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //========= light properties
        //
        tri->Draw(0, glm::vec3(0.0));
        
        glm::vec3 model_trans = glm::vec3(-1.0f, 1.0f, -5.0f);
        for (int i = 0; i < cubes.size(); i++) {
            cout<< "i:" << (i+1)/1.5 <<endl;
            model_trans = glm::vec3(((i - 1)/0.6), 1.0f, -5.0f);
            cubes.at(i)->Draw(diffuseMap,model_trans);
        }
        

        lightingShader.use();
        lightingShader.setVec3("light.direction", -0.2f, 1.0f, -0.3f);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lightingShader.setMat4("projection", projection);
        glm::mat4 viewCube = camera.GetViewMatrix();
        lightingShader.setMat4("view", viewCube);

        modelCube = glm::translate(modelCube, glm::vec3(0.0));
        lightingShader.setMat4("model", modelCube);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // render the cube
        glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //========= light properties


        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        Update();
        lightCubeShader.setMat4("view", viewCube);
        lightCubeShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap1);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 30, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    //glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

glm::vec3 cameraFront = glm::vec3(0.0,0.0,1.0);
glm::vec3 cameraUp = glm::vec3(0.0,1.0,0.0);
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        positionCharacter.x -= 0.1f;
        modelCube = glm::translate(modelTempCube,positionCharacter);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        positionCharacter.x += 0.1f;
        modelCube = glm::translate(modelTempCube, positionCharacter);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        positionCharacter.z -= 0.1f;
        modelCube = glm::translate(modelTempCube,positionCharacter);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        positionCharacter.z += 0.1f;
        modelCube = glm::translate(modelTempCube, positionCharacter);
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        camera.Position.y += 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        camera.Position.y -= 0.1f;
    }
}

void Update(){
    camera.Position.x = positionCharacter.x;
    camera.Position.z = positionCharacter.z + 5;
    //glm::vec3(,positionCharacter.y + 2.5f,positionCharacter.z + 5);
    camera.Front = glm::normalize(positionCharacter - camera.Position);

    //std::cout<< "Front" << camera.Front.x <<";"<< camera.Front.y<< ";" << camera.Front.z  << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

