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

#include <Image.hpp>
#include <FilePath.hpp>

#include "Mesh.hpp"

template<typename T>
class Model
{
	public:
		Model(std::string &path, glm::mat4 ModelMatrix) : mCollision(ModelMatrix), mModelMatrix(ModelMatrix), mActionZone(glm::scale(ModelMatrix,glm::vec3(4.5f,4.5f,4.5f)))
		{
			loadModel(path);
		}

		void DrawTextures(glimac::Program &program, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 transformationsMatrix = glm::mat4(1));
		void DrawColors(glimac::Program &program, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 transformationsMatrix = glm::mat4(1));

		void move(glm::mat4 transform);

		bool collide(glm::mat4 transformationsMatrix, glm::vec3 position);
		bool canInteract(glm::vec3 position) { return mActionZone.contains(position); }
		template<typename U> bool isIn(Model<U> container);

		void disappear();
		void deleteBuffers();
		T mCollision;


	private:
		std::vector<Mesh> meshes;
		std::string directory;

	    glm::mat4 mModelMatrix;
		
		T mActionZone;

		void loadModel(const std::string &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

GLuint TextureFromFile(const char *path, const std::string &directory);





template<typename T>
void Model<T>::DrawTextures(glimac::Program &program, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 transformationsMatrix)
{
	if(transformationsMatrix != glm::mat4(1))
	{
		move(transformationsMatrix);
	}
	glm::mat4 MVMatrix = viewMatrix*mModelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uModelMatrix"),1,GL_FALSE,glm::value_ptr(mModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uMVMatrix"),1,GL_FALSE,glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uMVPMatrix"),1,GL_FALSE,glm::value_ptr(projMatrix*MVMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uNormalMatrix"),1,GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(mModelMatrix))));

	for(size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].DrawTextures(program);
	}
}

template<typename T>
void Model<T>::DrawColors(glimac::Program &program, glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::mat4 transformationsMatrix)
{
	if(transformationsMatrix != glm::mat4(1))
	{
		move(transformationsMatrix);
	}
	glm::mat4 MVMatrix = viewMatrix*mModelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uModelMatrix"),1,GL_FALSE,glm::value_ptr(mModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uMVMatrix"),1,GL_FALSE,glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uMVPMatrix"),1,GL_FALSE,glm::value_ptr(projMatrix*MVMatrix));
    glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uNormalMatrix"),1,GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(mModelMatrix))));

	for(size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].DrawColors(program);
	}
}

template<typename T>
void Model<T>::move(glm::mat4 transform)
{
	mModelMatrix = transform * mModelMatrix; //Update the object's position if player interacts with it
	T col(mModelMatrix);
	mCollision = col; //Update collision zone
	T act(glm::scale(mModelMatrix,glm::vec3(4.5f,4.5f,4.5f)));
	mActionZone = act; //Update action zone
}

template<typename T>
bool Model<T>::collide(glm::mat4 transformationsMatrix, glm::vec3 position)
{
	glm::mat4 newModelMatrix = transformationsMatrix * mModelMatrix;
	T newCol(newModelMatrix);
	return newCol.contains(position);
}

template<typename T>
template<typename U> bool Model<T>::isIn(Model<U> container)
{
    return container.mCollision.contains(this->mCollision.getPosition());
}


template<typename T>
void Model<T>::loadModel(const std::string &path)
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

template<typename T>
void Model<T>::processNode(aiNode *node, const aiScene *scene)
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

template<typename T>
Mesh Model<T>::processMesh(aiMesh *mesh, const aiScene *scene)
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

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	Material mat;
	aiColor3D color;

	material->Get(AI_MATKEY_COLOR_AMBIENT,color);
	mat.Ka = glm::vec3(color.r,color.g,color.b);
	material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
	mat.Kd = glm::vec3(color.r,color.g,color.b);
	material->Get(AI_MATKEY_COLOR_SPECULAR,color);
	mat.Ks = glm::vec3(color.r,color.g,color.b);
	material->Get(AI_MATKEY_SHININESS,mat.shininess);

	if(mesh->mMaterialIndex >= 0)
	{
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "uTextureDiffuse");
		textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "uTextureSpecular");
		textures.insert(textures.end(),specularMaps.begin(),specularMaps.end());
	}

	return Mesh(vertices, indices, textures, mat);
}

template<typename T>
std::vector<Texture> Model<T>::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
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

template<typename T>
void Model<T>::disappear()
{
	move(glm::mat4(0));
	deleteBuffers();
}

template<typename T>
void Model<T>::deleteBuffers()
{
	for(size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].deleteBuffers();
	}
}