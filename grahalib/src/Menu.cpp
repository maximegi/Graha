#include <iostream>
#include <GL/glew.h>
#include <Image.hpp>
#include "Vertex2DUV.hpp"

#include "Menu.hpp"

void Menu::initialization(const std::string imagePath)
{

    std::unique_ptr<glimac::Image> img = glimac::loadImage(imagePath);
    if(img == NULL)
    {
        std::cout << "Could not find texture" << std::endl;
    }

	const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_TEXTURE = 1;

    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getWidth(), img->getHeight(), 0, GL_RGBA, GL_FLOAT, img->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);

    Vertex2DUV vertices[] = {
    Vertex2DUV(glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0)),
    Vertex2DUV(glm::vec2(1.0, -1.0), glm::vec2(1.0, 1.0)),
    Vertex2DUV(glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0)), 
    Vertex2DUV(glm::vec2(-1.0, 1.0), glm::vec2(0.0, 0.0))
    };
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glGenVertexArrays(1,&mVao);
    glBindVertexArray(mVao);
    
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    
    glVertexAttribPointer(VERTEX_ATTR_POSITION,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, position)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, texture)));
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Menu::draw()
{
    mProgram.use();
    glBindVertexArray(mVao);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glUniform1i(glGetUniformLocation(mProgram.getGLId(), "uTexture"), 0);
    glDrawArrays(GL_QUADS,0,4);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Menu::deleteBuffers()
{
    glDeleteBuffers(1,&mVbo);
    glDeleteVertexArrays(1,&mVao);
    glDeleteTextures(1, &mTexture);
}