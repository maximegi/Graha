#pragma once

#include <GL/glew.h>

#include <iostream>

#include <SDLWindowManager.hpp>
#include <glm.hpp>

class Game
{
public:
	Game(unsigned int width, unsigned int height) : windowWidht(width), windowHeight(height),mWindowManager(width, height, "Graha")
	{
		InitWindow();
    	//ProjMatrix = glm::perspective(glm::radians(70.f),((float)width)/((float)height),0.1f,100.f);
	}
	void RenderLoop();

	glimac::SDLWindowManager mWindowManager;
private:
	void InitWindow();
	unsigned int windowWidht;
	unsigned int windowHeight;

    glm::mat4 ProjMatrix;
};