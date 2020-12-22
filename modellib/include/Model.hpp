#pragma once

#include <GL/glew.h>

#include <iostream>
#include <Program.hpp>
#include <glm.hpp>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

class Model
{
	public:
		Model(std::string &path)
		{
			loadModel(path);
		}

		void DrawTextures(glimac::Program &program);
		void DrawColors(glimac::Program &program);

		void deleteBuffers();
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(const std::string &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

GLuint TextureFromFile(const char *path, const std::string &directory);