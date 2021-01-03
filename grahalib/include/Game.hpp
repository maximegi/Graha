#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <string>

#include <SDLWindowManager.hpp>
#include <glm.hpp>

#include <Program.hpp>

#include "Audio.hpp"
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
			musicAudio("assets/audio/music.ogg", "LOOPING"),
			footAudio("assets/audio/foot.ogg", "NOT LOOPING"),
			new_objectAudio("assets/audio/new_object.ogg", "NOT LOOPING"),
			woodAudio("assets/audio/wood.ogg", "NOT LOOPING"),
			mProjMatrix(glm::perspective(glm::radians(70.f),((float)width)/((float)height),0.1f,100.f)),
			mMousePosition(mWindowManager.getMousePosition()), deltaTime(0.f), lastFrame(0.f),
			firstPlanet(applicationPath, "assets/meshes.txt", glm::vec3(0.f,7.85f,0.f)) {}
			
	void RenderLoop();
	void close();

private:
	unsigned int mWindowWidth, mWindowHeight;
	glimac::SDLWindowManager mWindowManager;
	Text text;

    Audio musicAudio;
    Audio footAudio;
    Audio new_objectAudio;
    Audio woodAudio;

    glm::mat4 mProjMatrix;
    glm::vec2 mMousePosition;
    float deltaTime, lastFrame;

    Planet firstPlanet;
};