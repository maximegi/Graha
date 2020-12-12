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

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : 
			mVertices(vertices), mIndices(indices),mTextures(textures)
		{
			setupMesh();
		}
		~Mesh();

		void Draw(glimac::Program &program);

	private:
		GLuint vao,vbo,ibo;

		void setupMesh();
};