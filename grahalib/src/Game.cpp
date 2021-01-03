#include <iostream>
#include <GL/glew.h>

#include "Game.hpp"


void Game::RenderLoop()
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

        float currentFrame = mWindowManager.getTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
    //EVENTS
        firstPlanet.processInput(mWindowManager,deltaTime, mMousePosition);

    //MODELS
        firstPlanet.drawModels(mProjMatrix);

    //TEXTS
        firstPlanet.quest(text, mWindowManager);

        mMousePosition = mWindowManager.getMousePosition();

        // Update the display
        mWindowManager.swapBuffers();
    }
}

void Game::close()
{
	firstPlanet.deleteBuffers();
}