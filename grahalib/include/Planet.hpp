#pragma once

#include <GL/glew.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <SDLWindowManager.hpp>
#include <glm.hpp>

#include <Program.hpp>
#include <FilePath.hpp>

//modellib
#include <Model.hpp>
#include <Rectangle.hpp>
#include <Cylinder.hpp>
#include <Spheric.hpp>

#include "FirstPersonCamera.hpp"

class Planet
{
public:
	Planet(glimac::FilePath applicationPath, std::string meshesFile, glm::vec3 beginPosition) : mCamera(beginPosition),
																								mApplicationPath(applicationPath),
																								mProgram(glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
							                              							 										 applicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl")),
																								mSun(true), mUsingLamp(false)
	{
		parse(meshesFile);
		initSkybox();
	}

	void processInput(const glimac::SDLWindowManager &windowManager, float deltaTime, glm::vec2 mousePosition);
	void drawModels(glm::mat4 &ProjMatrix);
	void deleteBuffers();

private:
    std::map<std::string, Model<Rectangle>> rectangleModel;
    std::map<std::string, Model<Cylinder>> cylinderModel;
    std::map<std::string, Model<Spheric>> sphericModel;

    glm::mat4 mTransformationsMatrix;

	FirstPersonCamera mCamera;

	glimac::FilePath mApplicationPath;
    glimac::Program mProgram;

    std::vector<glimac::FilePath> mSkyboxFaces;
    GLuint mVboSky, mVaoSky, mTextureSkybox;
    glimac::Program mSkyProgram;

    bool mSun;
    bool mUsingLamp;

    void parse(std::string &meshesFile);
    void initSkybox();
    bool collision(glm::mat4 newTransformationsMatrix, glm::vec3 position);
};

std::vector<std::string> split (const std::string &s, char delim);
glm::mat4 vectorstr2mat4(std::vector<std::string> vec);