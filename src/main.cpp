#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>

//glimac
#include <common.hpp>
#include <FilePath.hpp>
#include <Geometry.hpp>
#include <Program.hpp>
#include <Sphere.hpp>

//glm
#include <glm.hpp>

//modellib
#include <Model.hpp>
#include <Rectangle.hpp>
#include <Cylinder.hpp>
#include <Spheric.hpp>

#include "FirstPersonCamera.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    const unsigned int WINDOW_W = 1200;
    const unsigned int WINDOW_H = 900;

    SDLWindowManager windowManager(WINDOW_W, WINDOW_H, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    FilePath applicationPath(argv[0]);
    Program program(loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl"));
    program.use();


    GLuint locationLightDirection = glGetUniformLocation(program.getGLId(), "uLightDirection");
    GLuint locationLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //INITIALIZATION

    //MATRIXES
    glm::mat4 ModelMatrix = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1),glm::vec3(0.f,1.f,-5.f)),glm::radians(45.f),glm::vec3(0.,0.,1.)),glm::radians(45.f),glm::vec3(1.,0.,0.)),glm::vec3(3.,3.,3.));
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),((float)WINDOW_W)/((float)WINDOW_H),0.1f,100.f);
    glm::mat4 transformationsMatrix;

    //CAMERA
    FirstPersonCamera camera;
    glm::vec2 mousePosition = windowManager.getMousePosition();

    //MODEL
    std::string cubeFile = "assets/models/sphere/sphere.obj";
    Model<Spheric> cube(cubeFile,ModelMatrix);

    //TIME
    float deltaTime = 0.f;
    float lastFrame = 0.f;

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        //RENDERING CODE
        glClearColor(0.1,0.2,0.4,0.3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float currentFrame = windowManager.getTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //EVENTS
        if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft((mousePosition[0] - windowManager.getMousePosition()[0])/10.f);
            camera.rotateUp((mousePosition[1] - windowManager.getMousePosition()[1])/10.f);
        }
        float t;
        if(windowManager.isKeyPressed(SDLK_LSHIFT))
        {
            t = 5*deltaTime;
        }
        else
        {
            t = 3*deltaTime;
        }
        if(windowManager.isKeyPressed(SDLK_q))
        {
            camera.moveLeft(t);
        }
        if(windowManager.isKeyPressed(SDLK_z))
        {
            camera.moveFront(t);
        }
        if(windowManager.isKeyPressed(SDLK_d))
        {
            camera.moveLeft(-t);
        }
        if(windowManager.isKeyPressed(SDLK_s))
        {
            camera.moveFront(-t);
        }
        if(windowManager.isKeyPressed(SDLK_SPACE))
        {
            camera.moveUp(t);
        }

        if(windowManager.isKeyPressed(SDLK_p))
        {
            if(cube.collision(camera.getPosition()))
            {
                std::cout << "Dedans" << std::endl;
            }
            else
            {
                std::cout << "Dehors" << std::endl;
            }
        }
        if(windowManager.isKeyPressed(SDLK_t))
        {
            transformationsMatrix = glm::translate(glm::mat4(1),glm::vec3(0.,0.,-0.1));
        }

        //LIGHTS
        glUniform3fv(locationLightDirection,1,glm::value_ptr(glm::vec3(-1.,-1.,0.))); //Position of the light, don't forget to multiply by the view matrix
        glUniform3fv(locationLightIntensity,1,glm::value_ptr(glm::vec3(1.,1.,1.))); //Color of the light


    //CUBE
        //DRAW
        cube.DrawColors(program,camera.getViewMatrix(),ProjMatrix);//,transformationsMatrix);

        //END OF RENDERING CODE

        transformationsMatrix = glm::mat4(1);
        mousePosition = windowManager.getMousePosition();

        // Update the display
        windowManager.swapBuffers();
    }
    cube.deleteBuffers();

    return EXIT_SUCCESS;
}