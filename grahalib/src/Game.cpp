#include <iostream>
#include <GL/glew.h>

#include "Game.hpp"

void Game::RenderLoop()
{
    musicAudio.play();

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
        firstPlanet.processInput(mWindowManager,deltaTime, mMousePosition, footAudio);

    //MODELS
        firstPlanet.drawModels(mProjMatrix);

    //TEXTS
        text.write("Objectives", 0.1, 150.0, 0.7, glm::vec3(1., 1., 1.));

        mMousePosition = mWindowManager.getMousePosition();

        // Update the display
        mWindowManager.swapBuffers();
    }
}

void Game::close()
{
    //MODELS
	firstPlanet.deleteBuffers();

    //AUDIO
    musicAudio.deleteBuffer();
    footAudio.deleteBuffer();
    new_objectAudio.deleteBuffer();
    woodAudio.deleteBuffer();
    shutdownOpenAL();
}