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
#include "Text.hpp"

#include "Audio.hpp"
#include "FirstPersonCamera.hpp"

class Planet
{
public:
	Planet(glimac::FilePath applicationPath, std::string meshesFile, glm::vec3 beginPosition) : mCamera(beginPosition),
																								mApplicationPath(applicationPath),
																								mProgram(glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
							                              							 										 applicationPath.dirPath() + "assets/shaders/directionallightcolors.fs.glsl")),
																								mSkyProgram(glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/skybox.vs.glsl",
							                              							 											applicationPath.dirPath() + "assets/shaders/skybox.fs.glsl")),
																								mSkyTransform(glm::mat4(1))
	{
		parse(meshesFile);
		initSkybox();
	}

	void processInput(const glimac::SDLWindowManager &windowManager, float deltaTime, glm::vec2 mousePosition, Audio footAudio);
	void drawModels(glm::mat4 &ProjMatrix);
	void drawSky(glm::mat4 &ProjMatrix);
	void deleteBuffers();
	void quest(Text &text, const glimac::SDLWindowManager &windowManager, Audio &wood, Audio &new_object, Audio &rose, Audio &blue, Audio &yellow, Audio &applause);
    bool getmEnd() { return mEnd; };


private:
    std::map<std::string, Model<Rectangle>> rectangleModel;
    std::map<std::string, Model<Cylinder>> cylinderModel;
    std::map<std::string, Model<Spheric>> sphericModel;
    std::vector<std::string> trees;

    glm::mat4 mTransformationsMatrix;

	FirstPersonCamera mCamera;

	glimac::FilePath mApplicationPath;
    glimac::Program mProgram;
    bool mPhase0 = true;
    bool mPhase1 = false;
    bool mPhase2 = false;
    bool mPhase3 = false;
    bool mPhase4 = false;
    bool mEnd = false;
    bool mAx = false;
    bool mNeedTalkToPink = false;
    bool mNeedTalkToYellow = false;
    bool mRocketKeys = false;
	int mBatteryCtr = 0;
	int mTreeCtr = 0;

    glimac::Program mSkyProgram;
    GLuint mVboSky, mVaoSky, mTextureSkybox;
    glm::mat4 mSkyTransform;

    void parse(std::string &meshesFile);
    void initSkybox();
    bool collision(glm::mat4 newTransformationsMatrix, glm::vec3 position);

};

std::vector<std::string> split (const std::string &s, char delim);
glm::mat4 vectorstr2mat4(std::vector<std::string> vec);