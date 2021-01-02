#include <iostream>
#include <GL/glew.h>

#include "Model.hpp"

GLuint TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	glimac::FilePath filepath(filename);

	std::unique_ptr<glimac::Image> image = loadImage(filepath);
	if(image == NULL)
	{
		std::cerr << "error: image:" << filename << " could not be loaded" << std::endl;
        return EXIT_FAILURE;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_FLOAT, image->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);

    return tex;
}