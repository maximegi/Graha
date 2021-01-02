#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <fstream>
#include <string>
#include <sstream>
#include <map>

//texte
#include <ft2build.h>
#include FT_FREETYPE_H

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

#include "Text.hpp"
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
    //exchange Y and Z axis to change from blender space to OpenGL
    return glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(
    glm::rotate(glm::rotate(glm::mat4(1),glm::radians(-150.f),glm::vec3(0.,1.,0.)),glm::radians(25.f),glm::vec3(1.,0.,0.)), // to begin at the right place
    glm::vec3(std::stof(vec[0]),std::stof(vec[2]),-std::stof(vec[1]))), //translate
    glm::radians(std::stof(vec[5])),glm::vec3(0.,1.,0.)), //rotateY
    glm::radians(-std::stof(vec[4])),glm::vec3(0.,0.,1.)), //rotateZ
    glm::radians(std::stof(vec[3])),glm::vec3(1.,0.,0.)), //rotateX
    glm::vec3(std::stof(vec[6]), std::stof(vec[8]), std::stof(vec[7]))); //scale
}

bool collision(std::map<std::string, Model<Rectangle>> &rectangleModel, std::map<std::string, Model<Cylinder>> &cylinderModel, std::map<std::string, Model<Spheric>> &sphericModel,
                glm::mat4 transformationsMatrix, glm::vec3 position)
{
    std::map<std::string, Model<Rectangle>>::iterator irec;
    std::map<std::string, Model<Cylinder>>::iterator icyl;
    std::map<std::string, Model<Spheric>>::iterator isph;
    for(irec = rectangleModel.begin(); irec != rectangleModel.end(); irec++)
    {
        if((irec->second).collide(transformationsMatrix,position))
        {
            return true;
        }
    }
    for(icyl = cylinderModel.begin(); icyl != cylinderModel.end(); icyl++)
    {
        if((icyl->second).collide(transformationsMatrix,position))
        {
            return true;
        }
    }
    for(isph = sphericModel.begin(); isph != sphericModel.end(); isph++)
    {
        if((isph->second).collide(transformationsMatrix,position))
        {
            return true;
        }
    }
    return false;
}

struct ModelProgram {
    Program m_Program;

    GLuint locationLightDirection;
    GLuint locationLightIntensity;

    ModelProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl")) {
        locationLightDirection = glGetUniformLocation(m_Program.getGLId(), "uLightDirection");
        locationLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    FilePath applicationPath(argv[0]);
    ModelProgram modelProgram(applicationPath);
    Text text(applicationPath);
   

    //INITIALIZATION

    text.initialization();

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
    std::vector<std::string> vname;
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
        else if(vline[0] == "name")
        {
            vname.push_back(vline[1]);
        }
    }
    file.close();
    std::map<std::string, Model<Rectangle>> rectangleModel;
    std::map<std::string, Model<Cylinder>> cylinderModel;
    std::map<std::string, Model<Spheric>> sphericModel;
    for(size_t i = 0; i < vmod.size(); ++i)
    {
        if(vcol[i] == "Rectangle")
        {   
            Model<Rectangle> cube(vmod[i], vmat[i]);
            rectangleModel.insert(std::pair<std::string, Model<Rectangle>>(vname[i], cube));
        }
        else if(vcol[i] == "Cylinder")
        {
            Model<Cylinder> cube(vmod[i], vmat[i]);
            cylinderModel.insert(std::pair<std::string, Model<Cylinder>>(vname[i], cube));
        }
        else if(vcol[i] == "Spheric")
        {
            Model<Spheric> cube(vmod[i], vmat[i]);
            sphericModel.insert(std::pair<std::string, Model<Spheric>>(vname[i], cube));
        }
    }
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
            t = 15*deltaTime;
        }
        else
        {
            t = 10*deltaTime;
        }
        if(windowManager.isKeyPressed(SDLK_q) && !collision(rectangleModel,cylinderModel,sphericModel,glm::rotate(transformationsMatrix,glm::radians(t),camera.getDirection()),camera.getPosition()))
        {
            transformationsMatrix = glm::rotate(transformationsMatrix,glm::radians(t),camera.getDirection());
        }
        if(windowManager.isKeyPressed(SDLK_z) && !collision(rectangleModel,cylinderModel,sphericModel,glm::rotate(transformationsMatrix,-glm::radians(t),camera.getLeftVector()),camera.getPosition()))
        {
            transformationsMatrix = glm::rotate(transformationsMatrix,-glm::radians(t),camera.getLeftVector());
        }
        if(windowManager.isKeyPressed(SDLK_d) && !collision(rectangleModel,cylinderModel,sphericModel,glm::rotate(transformationsMatrix,-glm::radians(t),camera.getDirection()),camera.getPosition()))
        {
            transformationsMatrix = glm::rotate(transformationsMatrix,-glm::radians(t),camera.getDirection());
        }
        if(windowManager.isKeyPressed(SDLK_s) && !collision(rectangleModel,cylinderModel,sphericModel,glm::rotate(transformationsMatrix,glm::radians(t),camera.getLeftVector()),camera.getPosition()))
        {
            transformationsMatrix = glm::rotate(transformationsMatrix,glm::radians(t),camera.getLeftVector());
        }

        if((rectangleModel.find("monster_pink")->second).canInteract(camera.getPosition()))
        {
            (rectangleModel.find("monster_pink")->second).move(glm::rotate(transformationsMatrix,glm::radians(1.5f),camera.getLeftVector()));
        }
        if((sphericModel.find("ball")->second).canInteract(camera.getPosition()))
        {
            (sphericModel.find("ball")->second).move(glm::rotate(transformationsMatrix,glm::radians(1.5f),camera.getLeftVector()));
        }


    //MODELS
        modelProgram.m_Program.use();

        //LIGHTS
        glUniform3fv(modelProgram.locationLightDirection,1,glm::value_ptr(glm::vec3(-1.,-1.,0.))); //Position of the light, don't forget to multiply by the view matrix
        glUniform3fv(modelProgram.locationLightIntensity,1,glm::value_ptr(glm::vec3(1.,1.,1.))); //Color of the light

        std::map<std::string, Model<Rectangle>>::iterator irec;
        std::map<std::string, Model<Cylinder>>::iterator icyl;
        std::map<std::string, Model<Spheric>>::iterator isph;
        //DRAW
        for(irec = rectangleModel.begin(); irec != rectangleModel.end(); irec++)
        {
            (irec->second).DrawColors(modelProgram.m_Program,camera.getViewMatrix(),ProjMatrix, transformationsMatrix);
        }
        for(icyl = cylinderModel.begin(); icyl != cylinderModel.end(); icyl++)
        {
            (icyl->second).DrawColors(modelProgram.m_Program,camera.getViewMatrix(),ProjMatrix, transformationsMatrix);
        }
        for(isph = sphericModel.begin(); isph != sphericModel.end(); isph++)
        {
            (isph->second).DrawColors(modelProgram.m_Program,camera.getViewMatrix(),ProjMatrix, transformationsMatrix);
        }

        glDisable(GL_BLEND);
        

    //TEXTS
        text.write("coucou sa va", 0.0, 0.0, 1.0, glm::vec3(1.0, 0.0, 0.2));

        //END OF RENDERING CODE

        transformationsMatrix = glm::mat4(1);
        mousePosition = windowManager.getMousePosition();

        // Update the display
        windowManager.swapBuffers();
    }
    std::map<std::string, Model<Rectangle>>::iterator irec;
    std::map<std::string, Model<Cylinder>>::iterator icyl;
    std::map<std::string, Model<Spheric>>::iterator isph;
    for(irec = rectangleModel.begin(); irec != rectangleModel.end(); irec++)
    {
        (irec->second).deleteBuffers();
    }
    for(icyl = cylinderModel.begin(); icyl != cylinderModel.end(); icyl++)
    {
        (icyl->second).deleteBuffers();
    }
    for(isph = sphericModel.begin(); isph != sphericModel.end(); isph++)
    {
        (isph->second).deleteBuffers();
    }
    return EXIT_SUCCESS;
}