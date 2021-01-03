#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <string>

#include <SDLWindowManager.hpp>
#include <glm.hpp>

#include <Program.hpp>

#include "Text.hpp"
#include "Planet.hpp"

#include "FirstPersonCamera.hpp"

class Game
{
public:
	Game(unsigned int width, unsigned int height, glimac::SDLWindowManager &window, glimac::FilePath &applicationPath) :  
			mWindowWidth(width),
			mWindowHeight(height),
			mWindowManager(window),
			text(applicationPath),
			mProjMatrix(glm::perspective(glm::radians(70.f),((float)width)/((float)height),0.1f,100.f)),
			mMousePosition(mWindowManager.getMousePosition()), deltaTime(0.f), lastFrame(0.f),
			firstPlanet(applicationPath, "assets/meshes.txt", glm::vec3(0.f,7.85f,0.f)) {}
			
	void RenderLoop();
	void close();
	void quest();

private:
	unsigned int mWindowWidth, mWindowHeight;
	glimac::SDLWindowManager mWindowManager;
	Text text;

    glm::mat4 mProjMatrix;
    glm::vec2 mMousePosition;
    float deltaTime, lastFrame;

    Planet firstPlanet;
};