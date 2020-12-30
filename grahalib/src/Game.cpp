#include <iostream>
#include <GL/glew.h>

#include "Game.hpp"

void Game::InitWindow()
{
    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
}

void Game::initialization()
{
    modelsCreation();
    mMousePosition = mWindowManager.getMousePosition();
}

void Game::modelsCreation()
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1),glm::vec3(-2,-2,-5));
    std::string sphereFile = "assets/models/sphere/sphere.obj";
    Model<Spheric> sphere(sphereFile,modelMatrix);
    mModelsSph.push_back(sphere);

    modelMatrix = glm::translate(glm::mat4(1),glm::vec3(2,-2,-5));
    std::string cylinderFile = "assets/models/cylinder/cylinder.obj";
    Model<Cylinder> cylinder(cylinderFile,modelMatrix);
    mModelsCyl.push_back(cylinder);
}

void Game::RenderLoop(glimac::Program &program)
{
	bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(mWindowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        glClearColor(0.1,0.2,0.4,0.3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput();

        Draw(program);

        mMousePosition = mWindowManager.getMousePosition();

        // Update the display
        mWindowManager.swapBuffers();
    }
}

void Game::processInput()
{
    if(mWindowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
    {
        mCamera.rotateLeft((mMousePosition[0] - mWindowManager.getMousePosition()[0])/10.f);
        mCamera.rotateUp((mMousePosition[1] - mWindowManager.getMousePosition()[1])/10.f);
    }

    if(mWindowManager.isKeyPressed(SDLK_q))
    {
        mCamera.moveLeft(0.1f);
    }
    if(mWindowManager.isKeyPressed(SDLK_z))
    {
        mCamera.moveFront(0.1f);
    }
    if(mWindowManager.isKeyPressed(SDLK_d))
    {
        mCamera.moveLeft(-0.1f);
    }
    if(mWindowManager.isKeyPressed(SDLK_s))
    {
        mCamera.moveFront(-0.1f);
    }
    if(mWindowManager.isKeyPressed(SDLK_SPACE))
    {
        mCamera.moveUp(0.1f);
    }
    
    if(mWindowManager.isKeyPressed(SDLK_p))
    {
        for(size_t i; i < mModelsRect.size(); i++)
        {
            if(mModelsRect[i].collision(mCamera.getPosition()))
            {
                std::cout << "Dedans" << std::endl;
            }
            else
            {
                std::cout << "Dehors" << std::endl;
            }
        }
        for(size_t i; i < mModelsCyl.size(); i++)
        {
            if(mModelsCyl[i].collision(mCamera.getPosition()))
            {
                std::cout << "Dedans" << std::endl;
            }
            else
            {
                std::cout << "Dehors" << std::endl;
            }
        }
        for(size_t i; i < mModelsSph.size(); i++)
        {
            if(mModelsSph[i].collision(mCamera.getPosition()))
            {
                std::cout << "Dedans" << std::endl;
            }
            else
            {
                std::cout << "Dehors" << std::endl;
            }
        }
    }
}

void Game::Draw(glimac::Program &program)
{
    for(size_t i; i < mModelsRect.size(); i++)
    {
        mModelsRect[i].DrawColors(program,mCamera.getViewMatrix(),mProjMatrix);
    }
    for(size_t i; i < mModelsCyl.size(); i++)
    {
        mModelsCyl[i].DrawColors(program,mCamera.getViewMatrix(),mProjMatrix);
    }
    for(size_t i; i < mModelsSph.size(); i++)
    {
        mModelsSph[i].DrawColors(program,mCamera.getViewMatrix(),mProjMatrix);
    }
}

void Game::deleteBuffers()
{
    for(size_t i; i < mModelsRect.size(); i++)
    {
        mModelsRect[i].deleteBuffers();
    }
    for(size_t i; i < mModelsCyl.size(); i++)
    {
        mModelsCyl[i].deleteBuffers();
    }
    for(size_t i; i < mModelsSph.size(); i++)
    {
        mModelsSph[i].deleteBuffers();
    }
}