#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

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

//fonctions necessaire au parse a ranger apres
std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
//je voulais mettre &vec mais ca marche pas ???
glm::mat4 vectorstr2mat4(std::vector<std::string> vec)
{
    return glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1),glm::vec3(std::stof(vec[0]),std::stof(vec[1]),std::stof(vec[2]))),glm::radians(std::stof(vec[5])),glm::vec3(0.,0.,1.)),glm::radians(std::stof(vec[4])),glm::vec3(0.,1.,0.)),glm::radians(std::stof(vec[3])),glm::vec3(1.,0.,0.)),glm::vec3(std::stof(vec[6]), std::stof(vec[7]), std::stof(vec[8])));
}

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
    glm::mat4 ModelMatrix = glm::scale(glm::mat4(1),glm::vec3(7.5,7.5,7.5));
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),((float)WINDOW_W)/((float)WINDOW_H),0.1f,100.f);
    glm::mat4 transformationsMatrix;

    //CAMERA
    FirstPersonCamera camera;
    glm::vec2 mousePosition = windowManager.getMousePosition();

    //MODEL PARSE
    std::string line;
    std::ifstream file("assets/meshes.txt", std::ios::out);
    if(!file.is_open())
    {
        std::cerr << "Unable to open meshes" << std::endl;
    }
    std::vector<std::string> vmod;
    std::vector<glm::mat4> vmat;
    std::vector<std::string> vcol;
    while(getline(file, line))
    {
        std::vector<std::string> vline = split(line, '~');
        if(vline[0] == "mod")
        {
            vmod.push_back(vline[1]);
        }
        else if(vline[0] == "mat")
        {
            vmat.push_back(vectorstr2mat4(split(vline[1], ',')));
        }
        else if(vline[0] == "col")
        {
            vcol.push_back(vline[1]);
        }
    }
    file.close();
    for(size_t i = 0; i < vmod.size(); ++i)
    {
        if(vcol[i] == "Rectangle")
        {   
            //Model<Rectangle> house(vmod[i], vmat[i]);
        }
        else if(vcol[i] == "Cylinder")
        {
            //Model<Cylinder> cube(vmod[i], vmat[i]);
        }
        else if(vcol[i] == "Spheric")
        {
            //Model<Spheric> cube(vmod[i], vmat[i]);
        }
    }

    Model<Spheric> planet(vmod[1],vmat[1]);
    Model<Rectangle> house(vmod[0], vmat[0]);

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
            if(planet.collision(camera.getPosition()))
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
        planet.DrawColors(program,camera.getViewMatrix(),ProjMatrix);//,transformationsMatrix);
        house.DrawColors(program, camera.getViewMatrix(),ProjMatrix);
        //END OF RENDERING CODE

        transformationsMatrix = glm::mat4(1);
        mousePosition = windowManager.getMousePosition();

        // Update the display
        windowManager.swapBuffers();
    }
    planet.deleteBuffers();
    house.deleteBuffers();

    return EXIT_SUCCESS;
}