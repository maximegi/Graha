#pragma once

#include <iostream>
#include <string>
#include <map>

#include <Program.hpp>
#include <FilePath.hpp>

//texte
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Character.hpp"

class Text
{
	public:
		Text(const glimac::FilePath& applicationPath) : mProgram(loadProgram(applicationPath.dirPath() + "assets/shaders/text.vs.glsl",
                              		  applicationPath.dirPath() + "assets/shaders/text.fs.glsl")) 
		{
    		mProj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	        mLocationProjMatrix = glGetUniformLocation(mProgram.getGLId(), "uProjection");
	        mLocationText = glGetUniformLocation(mProgram.getGLId(), "uText");
	        mLocationTextColor = glGetUniformLocation(mProgram.getGLId(), "uTextColor");

			initText();
		}
		void initialization();
		void write(std::string text, float x, float y, float scale, glm::vec3 color);
	private:
		std::map<char, Character> Characters;

		GLuint mVbo,mVao;
		glm::mat4 mProj;

		glimac::Program mProgram;
	    GLuint mLocationProjMatrix, mLocationText, mLocationTextColor;

		void initText();
};