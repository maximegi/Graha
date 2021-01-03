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

//modellib
#include <Model.hpp>
#include <Rectangle.hpp>
#include <Cylinder.hpp>
#include <Spheric.hpp>
#include "Text.hpp"

#include "FirstPersonCamera.hpp"

class Planet
{
public:
	Planet(glimac::FilePath applicationPath, std::string meshesFile, glm::vec3 beginPosition) : mCamera(beginPosition),
																								mProgram(glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
							                              							 										 applicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl")) 
	{
		parse(meshesFile);
	}

	void processInput(const glimac::SDLWindowManager &windowManager, float deltaTime, glm::vec2 mousePosition);
	void drawModels(glm::mat4 &ProjMatrix);
	void deleteBuffers();
	void quest(Text &text, const glimac::SDLWindowManager &windowManager);


private:
    std::map<std::string, Model<Rectangle>> rectangleModel;
    std::map<std::string, Model<Cylinder>> cylinderModel;
    std::map<std::string, Model<Spheric>> sphericModel;
    std::vector<std::string> trees;

    glm::mat4 mTransformationsMatrix;

	FirstPersonCamera mCamera;

    glimac::Program mProgram;
    bool phase0 = true;
    bool phase1 = false;
    bool phase2 = false;
    bool phase3 = false;
    bool phase4 = false;
    bool end = false;
    bool ax = false;
    bool needTalkToPink = false;
    bool needTalkToYellow = false;
    bool rocketKeys = false;
	int batteryCtr = 0;
	int treeCtr = 0;

    void parse(std::string &meshesFile);
    bool collision(glm::mat4 newTransformationsMatrix, glm::vec3 position);

};

std::vector<std::string> split (const std::string &s, char delim);
glm::mat4 vectorstr2mat4(std::vector<std::string> vec);