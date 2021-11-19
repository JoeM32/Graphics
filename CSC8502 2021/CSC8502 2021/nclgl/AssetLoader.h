#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>
#include <string>
#include <map>
#include <utility>

//TODO
//ideally we change all these to shared pointers later as currently meshes and shaders can be deleted externally :/
//make class a singleton
class AssetLoader
{
public:
	AssetLoader() ;
	~AssetLoader(void);
	Shader* getShader(std::string vertex, std::string fragment);
	Mesh* getMesh(std::string file);
	GLuint* getTexture(std::string file);
	void Load();
	void Unload();
private:
	const std::vector<std::string> meshFiles = { "OffsetCubeY.msh" };
	const std::vector<std::pair<std::string, std::string>> shaderFiles = { {"SceneVertex.glsl", "SceneFragment.glsl"}};
	const std::vector<std::string> textureFiles = { "stainedglass.tga" };
	std::map<std::pair<std::string, std::string>, Shader*> shaders;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, GLuint> textures;
	Shader* loadShader(std::string vertex, std::string fragment);
	Mesh* loadMesh(std::string file);
	GLuint* loadTexture(std::string file);
};

