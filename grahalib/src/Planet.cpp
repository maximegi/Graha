#include <iostream>
#include <GL/glew.h>
#include <Image.hpp>

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

void Planet::initSkybox()
{
	std::vector<float> skyboxVertices = {
    // positions
		//front
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	    -20.0f, -20.0f, -20.0f,  1.f/3.f,  1.f/4.f,
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	     20.0f,  20.0f, -20.0f,  2.f/3.f,  2.f/4.f,
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	    //left
	    -20.0f, -20.0f,  20.0f,  1.f/3.f,  0.f/4.f,
	    -20.0f, -20.0f, -20.0f,  1.f/3.f,  1.f/4.f,
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	    -20.0f,  20.0f,  20.0f,  2.f/3.f,  0.f/4.f,
	    -20.0f, -20.0f,  20.0f,  1.f/3.f,  0.f/4.f,
	    //right
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	     20.0f, -20.0f,  20.0f,  1.f/3.f,  3.f/4.f,
	     20.0f,  20.0f,  20.0f,  2.f/3.f,  3.f/4.f,
	     20.0f,  20.0f,  20.0f,  2.f/3.f,  3.f/4.f,
	     20.0f,  20.0f, -20.0f,  2.f/3.f,  2.f/4.f,
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	     //back
	    -20.0f, -20.0f,  20.0f,  1.f/3.f,  4.f/4.f,
	    -20.0f,  20.0f,  20.0f,  2.f/3.f,  4.f/4.f,
	     20.0f,  20.0f,  20.0f,  2.f/3.f,  3.f/4.f,
	     20.0f,  20.0f,  20.0f,  2.f/3.f,  3.f/4.f,
	     20.0f, -20.0f,  20.0f,  1.f/3.f,  3.f/4.f,
	    -20.0f, -20.0f,  20.0f,  1.f/3.f,  4.f/4.f,
	    //top
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	     20.0f,  20.0f, -20.0f,  2.f/3.f,  2.f/4.f,
	     20.0f,  20.0f,  20.0f,  3.f/3.f,  2.f/4.f,
	     20.0f,  20.0f,  20.0f,  3.f/3.f,  2.f/4.f,
	    -20.0f,  20.0f,  20.0f,  3.f/3.f,  1.f/4.f,
	    -20.0f,  20.0f, -20.0f,  2.f/3.f,  1.f/4.f,
	    //bottom
	    -20.0f, -20.0f, -20.0f,  1.f/3.f,  1.f/4.f,
	    -20.0f, -20.0f,  20.0f,  0.f/3.f,  1.f/4.f,
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	     20.0f, -20.0f, -20.0f,  1.f/3.f,  2.f/4.f,
	    -20.0f, -20.0f,  20.0f,  0.f/3.f,  1.f/4.f,
	     20.0f, -20.0f,  20.0f,  0.f/3.f,  2.f/4.f
	};

    glGenBuffers(1,&mVboSky);
    glBindBuffer(GL_ARRAY_BUFFER,mVboSky);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1,&mVaoSky);
    glBindVertexArray(mVaoSky);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,mVboSky);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(const GLvoid*)(3*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    glGenTextures(1, &mTextureSkybox);
    glBindTexture(GL_TEXTURE_2D, mTextureSkybox);

    std::unique_ptr<glimac::Image> skyboxImage = loadImage(mApplicationPath.dirPath() + "assets/textures/SkyBox.jpg");
    if(skyboxImage == NULL)
    {
        std::cerr << "error: skybox image could not be loaded" << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skyboxImage->getWidth(), skyboxImage->getHeight(), 0, GL_RGBA, GL_FLOAT, skyboxImage->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Planet::quest(Text &text, const glimac::SDLWindowManager &windowManager, Audio &wood, Audio &new_object, Audio &rose, Audio &blue, Audio &yellow, Audio &applause){    
    //at every moment
    if((sphericModel.find("ball")->second).canInteract(mCamera.getPosition()))
    {
        (sphericModel.find("ball")->second).move(glm::rotate(mTransformationsMatrix,glm::radians(2.f),glm::cross(mCamera.getPosition() - (sphericModel.find("ball")->second).mCollision.getPosition() ,glm::vec3(0.,1.,0.))));
    }
    if (mNeedTalkToPink)
    {
        text.write("Va voir Rose", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
    }
    //Discovery / phase 0
    if (mPhase0){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            rose.pause();
            rose.play();
            text.write("Il n'y a plus de lumiere ! Retrouve les piles !", 210.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
            mProgram = glimac::loadProgram(mApplicationPath.dirPath() + "assets/shaders/3D.vs.glsl", mApplicationPath.dirPath() + "assets/shaders/spotlight.fs.glsl");
            mPhase0 = false;
            mPhase1 = true;
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            blue.pause();
            blue.play();
            text.write("Belle journée aujourd'hui, n'est-ce pas ?", 195.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            yellow.pause();
            yellow.play();
            text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
    }

    //battery hunt / phase 1
    if (mPhase1){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            rose.pause();
            rose.play();
            text.write("Il n'y a plus de lumiere ! Retrouve les piles !", 210.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            blue.pause();
            blue.play();
            text.write("J'ai perdu ma pile ! Je pense qu'elle n'est pas loin !", 195.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            yellow.pause();
            yellow.play();
            text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("battery1")->second).canInteract(mCamera.getPosition()))
        {
            mBatteryCtr++;
            new_object.play();
            (cylinderModel.find("battery1")->second).disappear();
        }
        if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("battery2")->second).canInteract(mCamera.getPosition()))
        {
            mBatteryCtr++;
            new_object.play();
            (cylinderModel.find("battery2")->second).disappear();
        }

        text.write("Retrouve les piles :", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
        if (mBatteryCtr == 0){
            text.write("0/2", 230, 50.0, 0.5, glm::vec3(1., 1., 1.));
        }
        if (mBatteryCtr == 1){
            text.write("1/2", 230, 50.0, 0.5, glm::vec3(1., 1., 1.));
        }
        if (mBatteryCtr == 2){
            mPhase1 = false;
            mPhase2 = true;
            mNeedTalkToPink = true;
        }
    }


    //tree phase / phase 2
    if (mPhase2){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            rose.pause();
            rose.play();
            text.write("Merci beaucoup", 320.0, 120.0, 0.42, glm::vec3(1., 1., 1.));
            text.write("Ce radin de Bleu ne me donne jamais de bois, vole lui sa hache", 120.0, 90.0, 0.42, glm::vec3(1., 1., 1.));
            text.write("et coupe moi 3 arbres pour chauffer ma maison", 200.0, 60.0, 0.42, glm::vec3(1., 1., 1.));
            mNeedTalkToPink = false;
            mProgram = glimac::loadProgram(mApplicationPath.dirPath() + "assets/shaders/3D.vs.glsl", mApplicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl");
        }
        if(windowManager.isKeyPressed(SDLK_e) && !mAx && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            blue.pause();
            blue.play();
            text.write("Merci ! La lumiere est revenue !", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            yellow.pause();
            yellow.play();
            text.write("T'aimes l'espace toi ? Viens me voir quand tu auras du temps", 120.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if (!mNeedTalkToPink)
        {
            if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("ax")->second).canInteract(mCamera.getPosition()))
            {
                (cylinderModel.find("ax")->second).disappear();
                new_object.play();
                mAx = true;
            }
            if (!mAx)
                text.write("Vole la hache", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
            if (mAx){
                text.write("Coupe 3 arbres : ", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
                if (mTreeCtr == 0){
                    text.write("0/3", 230, 50.0, 0.5, glm::vec3(1., 1., 1.));
                }
                if (mTreeCtr == 1){
                    text.write("1/3", 230, 50.0, 0.5, glm::vec3(1., 1., 1.));
                }
                if (mTreeCtr == 2){
                    text.write("2/3", 230, 50.0, 0.5, glm::vec3(1., 1., 1.));
                }

                for (size_t i =0; i < trees.size(); i++){
                    if (windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find(trees[i])->second).canInteract(mCamera.getPosition()))
                    {
                        wood.play();
                        (cylinderModel.find(trees[i])->second).disappear();
                        mTreeCtr++;
                    }
                }
                if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
                {
                    blue.pause();
                    blue.play();
                    text.write("Hop hop hop rends moi ma hache voleur !", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
                }
                if (mTreeCtr >= 3){
                    mPhase2 = false;
                    mPhase3 = true;
                    mNeedTalkToPink = true;
                }
            }
        }
    }

    //football phase / phase 3
    if (mPhase3){
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            rose.pause();
            rose.play();
            text.write("Merci beaucoup de ton aide ! Va t'amuser !", 192.0, 90.0, 0.42, glm::vec3(1., 1., 1.));
            text.write("Tiens essaye de mettre un but pour voir !", 200.0, 60.0, 0.42, glm::vec3(1., 1., 1.));
            mNeedTalkToPink = false;
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            blue.pause();
            blue.play();
            text.write("Mais tu vas me rendre ma hache oui ?!", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            yellow.pause();
            yellow.play();
            text.write("T'aimes le foot ? Moi j'aime l'espace et j'ai une grosse fusee", 125.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        for (size_t i =0; i < trees.size(); i++){
            if (windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find(trees[i])->second).canInteract(mCamera.getPosition()))
            {
                text.write("Tu ne vas quand même pas raser les bois ?", 230.0, 100.0, 0.42, glm::vec3(0.5, 0.1, 0.1));
            }
        }
        if (!mNeedTalkToPink){
            text.write("Marque un but ! ", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
            if((sphericModel.find("ball")->second).isIn(rectangleModel.find("cage1")->second) || (sphericModel.find("ball")->second).isIn(rectangleModel.find("cage2")->second) ) {
                mPhase3 = false;
                mPhase4 = true;
                mNeedTalkToYellow = true;
            }
        }
    }

    //end phase / phase 4
    if (mPhase4){
        if (mNeedTalkToYellow)
        {   
            applause.pause();
            applause.play();
            text.write("Va voir Jaune ", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_pink")->second).canInteract(mCamera.getPosition()))
        {
            rose.pause();
            rose.play();
            text.write("Alors le foot ?", 350.0, 90.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_blue")->second).canInteract(mCamera.getPosition()))
        {
            blue.pause();
            blue.play();
            text.write("Mais tu vas me rendre ma hache oui ?!", 230.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (rectangleModel.find("monster_yellow")->second).canInteract(mCamera.getPosition()))
        {
            yellow.pause();
            yellow.play();
            text.write("Tiens prends les clefs de ma fusee et va faire un tour", 125.0, 100.0, 0.42, glm::vec3(1., 1., 1.));
            mNeedTalkToYellow = false;
            mRocketKeys = true;
        }
        if (mRocketKeys)
        {
            text.write("Emprunte la fusee ", 15, 50.0, 0.5, glm::vec3(1., 1., 1.));
        }
        if(windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find("rocket")->second).canInteract(mCamera.getPosition()) && mRocketKeys)
        {
            mEnd = true;
            mRocketKeys = false;
        }
        for (size_t i =0; i < trees.size(); i++){
            if (windowManager.isKeyPressed(SDLK_e) && (cylinderModel.find(trees[i])->second).canInteract(mCamera.getPosition()))
            {
                text.write("Tu ne vas quand même pas raser les bois ?", 230.0, 100.0, 0.42, glm::vec3(0.5, 0.1, 0.1));
            }
        }
    }

    
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
    if(mPhase1 || (mPhase2 && mNeedTalkToPink))
    {
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightPos_vs"),1,glm::value_ptr(mCamera.getPosition())); 
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightIntensity"),1,glm::value_ptr(glm::vec3(0.5 + (float)mBatteryCtr, 0.5 + (float)mBatteryCtr, 0.5 + (float)mBatteryCtr))); //Color of the light
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightDirection"),1,glm::value_ptr(mCamera.getFrontVector()));
	    glUniform1f(glGetUniformLocation(mProgram.getGLId(), "uInnerCutOff"),glm::cos(glm::radians(7.5f + 5.f*mBatteryCtr)));
	    glUniform1f(glGetUniformLocation(mProgram.getGLId(), "uOuterCutOff"),glm::cos(glm::radians(12.5f + 5.f*mBatteryCtr)));
	}
	else
	{
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uFirstLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(1.,-1.,1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uSecondLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(-1.,-1.,1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uThirdLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(0.,-1.,-1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uFourthLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(1.,1.,1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uFifthLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(-1.,1.,1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uSixthLightDirection"),1,glm::value_ptr(glm::vec3(mSkyTransform*glm::vec4(0.,1.,-1.,0.))));
	    glUniform3fv(glGetUniformLocation(mProgram.getGLId(), "uLightIntensity"),1,glm::value_ptr(glm::vec3(2.,2.,2.))); //Color of the light
	}

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

void Planet::drawSky(glm::mat4 &ProjMatrix)
{
    glEnable(GL_DEPTH_TEST);

    mSkyProgram.use();

    mSkyTransform = mTransformationsMatrix * mSkyTransform;

    glUniformMatrix4fv(glGetUniformLocation(mSkyProgram.getGLId(), "uMVPMatrix"),1,GL_FALSE,glm::value_ptr(ProjMatrix * mCamera.getViewMatrix() * mSkyTransform));

    //DRAW

    glBindVertexArray(mVaoSky);
    glBindTexture(GL_TEXTURE_2D, mTextureSkybox);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);

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

    glDeleteBuffers(1, &mVboSky);
    glDeleteVertexArrays(1,&mVaoSky);
}