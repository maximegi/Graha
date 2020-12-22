#include <iostream>
#include <GL/glew.h>
#include <Mesh.hpp>

void Mesh::DrawTextures(glimac::Program &program)
{
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, mTextures[0].id);
	// glUniform1i(glGetUniformLocation(program.getGLId(),mTextures[0].uName.c_str()),0);

	// glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_2D, mTextures[1].id);
	// glUniform1i(glGetUniformLocation(program.getGLId(),mTextures[1].uName.c_str()),1);

	// glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES,this->mIndices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Mesh::DrawColors(glimac::Program &program)
{
	glUniform3fv(glGetUniformLocation(program.getGLId(),"uKa"),1,glm::value_ptr(mMaterial.Ka));
    glUniform3fv(glGetUniformLocation(program.getGLId(),"uKd"),1,glm::value_ptr(mMaterial.Kd));
    glUniform3fv(glGetUniformLocation(program.getGLId(),"uKs"),1,glm::value_ptr(mMaterial.Ks));
    glUniform1f(glGetUniformLocation(program.getGLId(),"uShininess"),mMaterial.shininess);

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES,this->mIndices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,mVertices.size()*sizeof(Vertex),mVertices.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,mIndices.size()*sizeof(unsigned int),mIndices.data(),GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//normal
	glEnableVertexAttribArray(2);	//texCoords
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)(offsetof(Vertex,position)));
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)(offsetof(Vertex,normal)));
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)(offsetof(Vertex,texCoords)));
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}


void Mesh::deleteBuffers()
{
    glDeleteBuffers(1,&vbo);
    glDeleteBuffers(1,&ibo);
    glDeleteVertexArrays(1,&vao);
}