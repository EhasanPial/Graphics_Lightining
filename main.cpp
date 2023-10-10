//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "cube.h"
#include "dLight.h"
#include "SpotLight.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float z);
void beach(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void building(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void road(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
//BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  3.5f,  3.0f+0.05f),
    glm::vec3(-1.5f,  3.5f,  3.0f + 0.05f),
    glm::vec3(-1.5f,  3.5f,  -3.0f + 0.05f),
    glm::vec3(1.5f,  3.5f,  -3.0f + 0.05f)
};



glm::vec3 lightPositions[] = {
        glm::vec3(2.25f, 3.5f, -1.7f),
        glm::vec3(0.0f, -4.5f, 0.0f)
};

glm::vec3 lightDirections[] = {
    glm::vec3(0.0, 1.0f, 0.0f)
};

DirectionalLight dLight = DirectionalLight(-0.2f, -1.0f, -0.3f);

SpotLight spotLight(lightPositions[1], lightDirections[0], 8.5f, 15.5f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);
 

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    1.0f, 0.0f, 0.0f,     // ambient
    1.0f, 0.0f, 0.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool dLightbool = true;
bool spotLightbool = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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

    // build and compile our shader zprogram
    // ------------------------------------
    //Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShadingTest.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        0.25f, 0.25f, -0.25f, 0.3f, 0.8f, 0.5f,
        -0.25f, 0.25f, -0.25f, 0.5f, 0.4f, 0.3f,
        -0.25f, -0.25f, -0.25f, 0.2f, 0.7f, 0.3f,
        0.25f, -0.25f, -0.25f, 0.6f, 0.2f, 0.8f,
        0.25f, 0.25f, 0.25f, 0.8f, 0.3f, 0.6f,
        -0.25f, 0.25f, 0.25f, 0.4f, 0.4f, 0.8f,
        -0.25f, -0.25f, 0.25f, 0.2f, 0.3f, 0.6f,
        0.25f, -0.25f, 0.25f, 0.7f, 0.5f, 0.4f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    
   // Cube cube = Cube();
    //Cube cube2 = Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

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

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        dLight.setUpDirectionalLight(lightingShader);

        spotLight.setUpLight(lightingShader);


        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // bed 
        {
            float bx = -1.98f, by = 0.75f, bz = -1.0f;

            // bed er head 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx + 0, by + 0.75 / 2, bz - (2 - 0.25 / 2)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, 1, .5));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8f, 0.2f, 0.2f);

             

            //bed wood
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx, by, bz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.5f, scale_Z * 8.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0f, 0.99215f, 0.81568); 

            // balish

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx, by + 0.125, bz - (2 - 0.25 / 2) + 0.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.0f, scale_Y * .5f, scale_Z * 0.8));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9235, 0.490, 0.1764);  


            //foam 
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx, by, bz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.25f, scale_Z * 8.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0f, 0.99215f, 0.81568); 


            //legs0
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx + 1 - 0.125f, by - 0.75f / 2, bz + 2 - 0.125f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 1.5f, 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8f, 0.2f, 0.2f); 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx - (1 - 0.125f), by - 0.75f / 2, bz + 2 - 0.125f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 1.5f, 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8f, 0.2f, 0.2f); 


            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx - (1 - 0.125f), by - 0.75f / 2, bz - (2 - 0.125f)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 1.5f, 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8f, 0.2f, 0.2f); 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx + (1 - 0.125f), by - 0.75f / 2, bz - (2 - 0.125f)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 1.5f, 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8f, 0.2f, 0.2f); 
        }


        // Table 
        {
            float bx_table = 1.98f, by_table = 2.0f, bz_table = -1.98f;


            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table, by_table, bz_table));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.25f, scale_Z * 4.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8235, 0.490, 0.1764);
 

            //table er tokta
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table - 0.9, by_table - 1.0f, bz_table - 0.9));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 4.0f, 0.25));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6274, 0.32157, 0.17647);
 

            //table er tokta
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table - (-0.9), by_table - 1.0f, bz_table - 0.9));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 4.0f, 0.25));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6274, 0.32157, 0.17647);
 

            //table er tokta
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table - (-0.9), by_table - 1.0f, bz_table - (-0.9)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 4.0f, 0.25));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6274, 0.32157, 0.17647);
 

            //table er tokta
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table - (0.9), by_table - 1.0f, bz_table - (-0.9)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 4.0f, 0.25));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6274, 0.32157, 0.17647);


            //table er tokta
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_table - (-0.9), by_table - 1.0f, bz_table - (0.9)));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25, 4.0f, 0.25));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6274, 0.32157, 0.17647); 



        }


        // chair 
        {
            float bx_chair = 1.98f, by_chair = 0.75f, bz_chair = -0.75f;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair, bz_chair));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * .05f, scale_Z * 2.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8235, 0.490, 0.1764); 

            //  back1
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair + .48f, by_chair, bz_chair + .48f));//
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 3.0f, scale_Z * 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6470, 0.1647, 0.1647); 


            // back2
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair - .48f, by_chair, bz_chair + .48f));//
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 3.0f, scale_Z * 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6470, 0.1647, 0.1647); 

            // front1
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair + .42f, by_chair - 0.375f, bz_chair - .42f));// 
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 1.5f, scale_Z * 0.25f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6470, 0.1647, 0.1647); 

            //  front2
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair - .42f, by_chair - 0.375f, bz_chair - .42f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 1.5f, scale_Z * 0.25f));
            model =  translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6470, 0.1647, 0.1647); 

            // chair back
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair + 0.625f, bz_chair + 0.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * 1.0f, scale_Z * .05f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.8235, 0.490, 0.1764); 
        }
        

        // wardoff 2 
        {
            float bx_wardrobe = -0.2f, by_war = 0.0f, bz_war = 2.0f;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_wardrobe - 3.0f + 0.5, by_war + .7f, bz_war));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.0f, scale_Y * 2.8f, scale_Z * 1.9));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.6470, 0.1647, 0.1647);
           


            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_wardrobe - 3.0f + 0.75, by_war + .175f, bz_war));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1f, scale_Y * .5f, scale_Z * 1.7f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9235, 0.490, 0.1764);  

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_wardrobe - 3.0f + 0.75, by_war + .175f * 3, bz_war));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1f, scale_Y * .5f, scale_Z * 1.7));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9235, 0.490, 0.1764); 
           

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_wardrobe - 3.0f + 0.75, by_war + .175f * 5, bz_war));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1, scale_Y * .5f, scale_Z * 1.7f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9235, 0.490, 0.1764); 
            

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_wardrobe - 3.0f + 0.75, by_war + .175f * 7, bz_war));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1, scale_Y * .5f, scale_Z * 1.7f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9235, 0.490, 0.1764); 

        }


         // Floor
        {
            //ceiling
            float bx_chair = 0.0f, by_chair = 0.0f, bz_chair = 0.0f;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair + 4.0f, bz_chair));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 12.0f, scale_Y * .05f, scale_Z * 12.0f));
            model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

            drawCube(cubeVAO, lightingShader,model, 1.0f, 0.8431f, 0.7019f);

            //rows
            for (int i = 0; i < 6; i++)
            {
                float trans = 0.0;
                for (int j = 0; j < 6; j++) {
                    translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair - 3.0 + 0.25 + trans, by_chair, bz_chair - 3.0 + 0.25 + i));
                    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y * .05f, scale_Z));
                    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
                    drawCube(cubeVAO, lightingShader,model, 0.34509, 0.094117, 0.27058);
                    
                    trans += 1;
                }

            }
            for (int i = 0; i < 6; i++)
            {
                float trans = 0.5f;
                for (int j = 0; j < 6; j++) {
                    translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair - 3.0 + 0.25 + trans, by_chair, bz_chair - 3.0 + 0.5 + i + 0.25));
                    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y * .05f, scale_Z));
                    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
                    drawCube(cubeVAO, lightingShader,model, 1.0f, 0.8431f, 0.7019f);
                    trans += 1;
                }

            }  
            //back wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair + 2.0f, bz_chair - 3.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 12.0f, scale_Y * 8.0f, scale_Z * .05f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.62352f, 0.886274, 0.7490);

          


            //window outside
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair + 2.0f, bz_chair - 3.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * 2.0f, scale_Z * 0.1));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.62352f, 0.886274, 1.0f);
 

            //window inside
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair, by_chair + 2.0f, bz_chair - 3.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.80f, scale_Y * 1.80f, scale_Z * 0.2));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.9803, 0.9764, 0.964705);
         


            //left wall 

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair - 3.0f, by_chair + 2.0f, bz_chair));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .05f, scale_Y * 8.0f, scale_Z * 12.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.50196, 0.0, 0.37647);
 

            //right wall

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bx_chair + 3.0f, by_chair + 2.0f, bz_chair));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .05f, scale_Y * 8.0f, scale_Z * 12.0f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.50196, 0.0, 0.37647); 

 
        }


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 3.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);
    
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
   
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (dLightbool)
        {
            dLight.turnOff();
            dLightbool =! dLightbool;
        }
        else
        {
            dLight.turnOn();
            dLightbool = !dLightbool;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotLightbool)
        {
            spotLight.turnOff();
            spotLightbool = !spotLightbool;
        }
        else
        {
            spotLight.turnOn();
            spotLightbool = !spotLightbool;
        }
    }
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
