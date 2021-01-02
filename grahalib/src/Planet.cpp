#include <iostream>
#include <GL/glew.h>

#include "Planet.hpp"

void Planet::parse(std::string &meshesFile)
{
	//Open file
	std::string line;
    std::ifstream file(meshesFile, std::ios::out);
    if(!file.is_open())
    {
        std::cerr << "Unable to open meshes file" << std::endl;
    }

    //Read file
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

    //Use data from file to create all our models
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
}

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


void Planet::processInput(const glimac::SDLWindowManager &windowManager, float deltaTime, glm::vec2 mousePosition)
{
    if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
    {
        mCamera.rotateLeft((mousePosition[0] - windowManager.getMousePosition()[0])/10.f);
        mCamera.rotateUp((mousePosition[1] - windowManager.getMousePosition()[1])/10.f);
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
    if(windowManager.isKeyPressed(SDLK_q) && !collision(glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getDirection()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getDirection());
    }
    if(windowManager.isKeyPressed(SDLK_z) && !collision(glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getLeftVector()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getLeftVector());
    }
    if(windowManager.isKeyPressed(SDLK_d) && !collision(glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getDirection()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getDirection());
    }
    if(windowManager.isKeyPressed(SDLK_s) && !collision(glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getLeftVector()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getLeftVector());
    }

    if((rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
    {
        (rectangleModel.find("monster_pink")->second).move(glm::rotate(mTransformationsMatrix,glm::radians(1.5f),mCamera.getLeftVector()));
    }
    if((sphericModel.find("ball")->second).canInteract(mCamera.getPosition()))
    {
        (sphericModel.find("ball")->second).move(glm::rotate(mTransformationsMatrix,glm::radians(1.5f),mCamera.getLeftVector()));
    }
}

bool Planet::collision(glm::mat4 newTransformationsMatrix, glm::vec3 position)
{
    std::map<std::string, Model<Rectangle>>::iterator irec;
    std::map<std::string, Model<Cylinder>>::iterator icyl;
    std::map<std::string, Model<Spheric>>::iterator isph;
    for(irec = rectangleModel.begin(); irec != rectangleModel.end(); irec++)
    {
        if((irec->second).collide(newTransformationsMatrix,position))
        {
            return true;
        }
    }
    for(icyl = cylinderModel.begin(); icyl != cylinderModel.end(); icyl++)
    {
        if((icyl->second).collide(newTransformationsMatrix,position))
        {
            return true;
        }
    }
    for(isph = sphericModel.begin(); isph != sphericModel.end(); isph++)
    {
        if((isph->second).collide(newTransformationsMatrix,position))
        {
            return true;
        }
    }
    return false;
}

void Planet::drawModels(glm::mat4 &ProjMatrix)
{
    glEnable(GL_DEPTH_TEST);
    
    mProgram.use();

    //LIGHTS
    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightDirection"),1,glm::value_ptr(glm::vec3(-1.,-1.,0.))); //Position of the light, don't forget to multiply by the view matrix
    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightIntensity"),1,glm::value_ptr(glm::vec3(1.,1.,1.))); //Color of the light

    std::map<std::string, Model<Rectangle>>::iterator irec;
    std::map<std::string, Model<Cylinder>>::iterator icyl;
    std::map<std::string, Model<Spheric>>::iterator isph;
    //DRAW
    for(irec = rectangleModel.begin(); irec != rectangleModel.end(); irec++)
    {
        (irec->second).DrawColors(mProgram,mCamera.getViewMatrix(),ProjMatrix, mTransformationsMatrix);
    }
    for(icyl = cylinderModel.begin(); icyl != cylinderModel.end(); icyl++)
    {
        (icyl->second).DrawColors(mProgram,mCamera.getViewMatrix(),ProjMatrix, mTransformationsMatrix);
    }
    for(isph = sphericModel.begin(); isph != sphericModel.end(); isph++)
    {
        (isph->second).DrawColors(mProgram,mCamera.getViewMatrix(),ProjMatrix, mTransformationsMatrix);
    }

    mTransformationsMatrix = glm::mat4(1);
    glDisable(GL_DEPTH_TEST);
}

void Planet::deleteBuffers()
{
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
}