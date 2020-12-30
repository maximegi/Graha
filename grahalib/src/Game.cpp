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
}

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

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // Update the display
        mWindowManager.swapBuffers();
    }

    return;
}