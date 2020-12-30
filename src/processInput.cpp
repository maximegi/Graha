#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

//glm
#include <glm.hpp>

#include "FirstPersonCamera.hpp"

void processInput(glimac::SDLWindowManager windowManager, FirstPersonCamera camera, glm::vec2 mousePosition)
{
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
    if(windowManager.isKeyPressed(SDLK_SPACE))
    {
        camera.moveUp(0.1f);
    }

    mousePosition = windowManager.getMousePosition();
}