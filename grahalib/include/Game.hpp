#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <string>

#include <SDLWindowManager.hpp>
#include <glm.hpp>

#include <Program.hpp>

//modellib
#include <Model.hpp>
#include <Rectangle.hpp>
#include <Cylinder.hpp>
#include <Spheric.hpp>

#include "FirstPersonCamera.hpp"

class Game
{
public:
	Game(unsigned int width, unsigned int height) : windowWidht(width), windowHeight(height),mWindowManager(width, height, "Graha")
	{
		InitWindow();
    	//ProjMatrix = glm::perspective(glm::radians(70.f),((float)width)/((float)height),0.1f,100.f);
	}
	void initialization();
	void modelsCreation();
	void RenderLoop(glimac::Program &program);
	void processInput();
	void Draw(glimac::Program &program);
	void deleteBuffers();

private:
	void InitWindow();
	unsigned int windowWidht;
	unsigned int windowHeight;
	glimac::SDLWindowManager mWindowManager;

    glm::mat4 mProjMatrix;
    glm::vec2 mMousePosition;

    FirstPersonCamera mCamera;

    std::vector<Model<Rectangle>> mModelsRect;
    std::vector<Model<Cylinder>> mModelsCyl;
    std::vector<Model<Spheric>> mModelsSph;
};