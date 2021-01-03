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
            if (vmod[i].find("tree") != std::string::npos){
                trees.push_back(vname[i]);
            }
        }
        else if(vcol[i] == "Spheric")
        {
            Model<Spheric> cube(vmod[i], vmat[i]);
            sphericModel.insert(std::pair<std::string, Model<Spheric>>(vname[i], cube));
        }
    }
}

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

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

void Planet::quest(Text &text, const glimac::SDLWindowManager &windowManager){

    
    //at every moment
    if((sphericModel.find("ball")->second).canInteract(mCamera.getPosition()))
    {
        (sphericModel.find("ball")->second).move(glm::rotate(mTransformationsMatrix,glm::radians(1.5f),mCamera.getLeftVector()));
    }
    if((sphericModel.find("ball")->second).isIn(rectangleModel.find("cage1")->second) || (sphericModel.find("ball")->second).isIn(rectangleModel.find("cage2")->second)){
        std::cout<<"gg bg"<<std::endl;
    }
    for (size_t i =0; i < trees.size(); i++){
        if (windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find(trees[i])->second).canInteract(mCamera.getPosition())){
            (cylinderModel.find(trees[i])->second).disappear();
        }
    }

    //Discovery / phase 
    if (phase0){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            text.write("Il n'y a plus de lumiere ! Retrouve les piles !", 210.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            text.write("J'ai perdu ma pile ! Je pense qu'elle n'est pas loin !", 195.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
    }

    //battery hunt / phase 1
    if (phase1){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            text.write("Il n'y a plus de lumiere ! Retrouve les piles !", 210.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            text.write("J'ai perdu ma pile ! Je pense qu'elle n'est pas loin !", 195.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("battery1")->second).canInteract(mCamera.getPosition()))
        {
            batteryCtr++;
            (cylinderModel.find("battery1")->second).disappear();
            std::cout << batteryCtr<<std::endl;
        }
        if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("battery2")->second).canInteract(mCamera.getPosition()))
        {
            batteryCtr++;
            (cylinderModel.find("battery2")->second).disappear();
            std::cout << batteryCtr<<std::endl;
        }
    }


    //tree phase
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("Merci beaucoup", 320.0, 120.0, 0.42, glm::vec3(1., 1., 1.));
    //     text.write("Ce radin de Bleu ne me donne jamais de bois, vole lui sa hache", 120.0, 90.0, 0.42, glm::vec3(1., 1., 1.));
    //     text.write("et coupe moi 3 arbres pour chauffer ma maison", 200.0, 60.0, 0.42, glm::vec3(1., 1., 1.));
    // }
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("Hop hop hop rends moi ma hache voleur !", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
    // }
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
    // }

    //football phase
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("Merci beaucoup de ton aide ! Va t'amuser !", 192.0, 90.0, 0.42, glm::vec3(1., 1., 1.));
    //     text.write("Tiens essaye de mettre un but pour voir !", 200.0, 60.0, 0.42, glm::vec3(1., 1., 1.));
    // }
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("Mais tu vas me rendre ma hache oui ?!", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
    // }
    // if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
    // {
    //     text.write("T'aimes le foot ? Moi j'aime l'espace et j'ai une grosse fusee", 125.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
    // }


    
}

void Planet::processInput(const glimac::SDLWindowManager &windowManager, float deltaTime, glm::vec2 mousePosition, Audio footAudio)
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
    //ZQSD
    if(windowManager.isKeyPressed(SDLK_q) && !collision(glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getDirection()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getDirection());
        footAudio.pause();
        footAudio.play();
    }
    if(windowManager.isKeyPressed(SDLK_z) && !collision(glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getLeftVector()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getLeftVector());
        footAudio.pause();
        footAudio.play();
    }
    if(windowManager.isKeyPressed(SDLK_d) && !collision(glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getDirection()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,-glm::radians(t),mCamera.getDirection());
        footAudio.pause();
        footAudio.play();
    }
    if(windowManager.isKeyPressed(SDLK_s) && !collision(glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getLeftVector()),mCamera.getPosition()))
    {
        mTransformationsMatrix = glm::rotate(mTransformationsMatrix,glm::radians(t),mCamera.getLeftVector());
        footAudio.pause();
        footAudio.play();
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