#include <iostream>
#include <glm.hpp>
#include <vector>

#include "Vertex.hpp"
#include "Texture.hpp"

class Mesh
{
	public:
		vector<Vertex> mVertices;
		vector<unsigned int> mIndices;
		vector<Texture> mTextures;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) : 
			mVertices(vertices), mIndices(indices),mTextures(textures)
		{
			setupMesh();
		}
		~Mesh();

		void Draw();

	private:
		GLuint vao,vbo,ibo;

		void setupMesh();
}