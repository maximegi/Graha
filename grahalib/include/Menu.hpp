#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>

#include <glm.hpp>
#include <Program.hpp>
#include <FilePath.hpp>


class Menu
{
public:
	Menu(const std::string imagePath, const glimac::FilePath &applicationPath) : mProgram(glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/text2D.vs.glsl", applicationPath.dirPath() + "assets/shaders/text2D.fs.glsl"))
	{
		initialization(imagePath);
	}

	void draw();
	void deleteBuffers();
private:
	GLuint mVbo, mVao, mTexture;

	glimac::Program mProgram;

	void initialization(const std::string imagePath);
};