#include <iostream>
#include <GL/glew.h>
#include <FilePath.hpp>
#include <Image.hpp>
#include "Model.hpp"

void Model::Draw(glimac::Program &program)
{
	for(size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(program);
	}
}

void Model::loadModel(const std::string &path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene-> mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0,path.find_last_of('/'));

	processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh,scene));
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for(size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.position = pos;

		if(mesh->HasNormals())
		{
			glm::vec3 norm;
			norm.x = mesh->mNormals[i].x;
			norm.y = mesh->mNormals[i].y;
			norm.z = mesh->mNormals[i].z;
			vertex.normal = norm;
		}

		if(mesh->mTextureCoords[0])
		{
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = tex;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f,0.0f);
		}

		vertices.push_back(vertex);
	}

	for(size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	std::cout << mesh->mMaterialIndex << std::endl;
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "uTextureDiffuse");
		textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "uTextureSpecular");
		textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for(size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.uName = typeName;
		textures.push_back(texture);
	}

	return textures;
}

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