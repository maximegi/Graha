#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

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

#include "FreeflyCamera.hpp"

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
                              applicationPath.dirPath() + "assets/shaders/normals.fs.glsl"));
    program.use();

    GLuint locationMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLuint locationMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLuint locationModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    GLuint locationNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    // GLuint locationLightDirection = glGetUniformLocation(program.getGLId(), "uLightDirection");
    // GLuint locationLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");
    // GLuint locationShininess = glGetUniformLocation(program.getGLId(), "uShininess");

    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //INITIALIZATION

    Model volcano("assets/models/cube.obj");

    Sphere sphere(1,32,16);
    GLuint Nvertices = sphere.getVertexCount();

    GLuint vbo;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,Nvertices*sizeof(ShapeVertex),sphere.getDataPointer(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex,position)));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex,normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex,texCoords)));
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //MATRIXES
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),((float)WINDOW_W)/((float)WINDOW_H),0.1f,100.f);
    glm::mat4 ModelMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    //CAMERA
    FreeflyCamera camera;
    glm::vec2 mousePosition = windowManager.getMousePosition();


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
        //glClearColor(0.1,0.2,0.4,0.3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //EVENTS
        if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft((mousePosition[0] - windowManager.getMousePosition()[0])/10.f);
            camera.rotateUp((mousePosition[1] - windowManager.getMousePosition()[1])/10.f);
        }

        if(windowManager.isKeyPressed(SDLK_q))
        {
            camera.moveLeft(0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_z))
        {
            camera.moveFront(0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_d))
        {
            camera.moveLeft(-0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_s))
        {
            camera.moveFront(-0.1f);
        }

        //LIGHTS
        // glUniform3fv(locationLightDirection,1,glm::value_ptr(glm::vec3(-1.,-1.,0.))); //Position of the light, don't forget to multiply by the view matrix
        // glUniform3fv(locationLightIntensity,1,glm::value_ptr(glm::vec3(1.,1.,1.))); //Color of the light


        //matrixes
        ModelMatrix = glm::translate(glm::mat4(1),glm::vec3(0.,0.,-5.));
        MVMatrix = camera.getViewMatrix()*ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));

        glUniformMatrix4fv(locationModelMatrix,1,GL_FALSE,glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
        
        // glUniform1f(locationShininess,32.f);

        //DRAW
        volcano.Draw(program);

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES,0,Nvertices);
        glBindVertexArray(0);

        //END OF RENDERING CODE

        mousePosition = windowManager.getMousePosition();

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}