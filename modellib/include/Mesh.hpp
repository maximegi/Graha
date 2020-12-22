#pragma once

#include <GL/glew.h>

#include <iostream>
#include <Program.hpp>
#include <glm.hpp>
#include <vector>

#include "Vertex.hpp"
#include "Texture.hpp"

class Mesh
{
	public:
		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mIndices;
		std::vector<Texture> mTextures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures,glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks) : 
			mVertices(vertices), mIndices(indices), mTextures(textures), mKa(Ka), mKd(Kd), mKs(Ks)
		{
			setupMesh();
		}

		void DrawTextures(glimac::Program &program);
		void DrawColors(glimac::Program &program);

		void deleteBuffers();

	private:
		GLuint vbo,ibo,vao;
		glm::vec3 mKa, mKd, mKs;

		void setupMesh();
};