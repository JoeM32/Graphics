#include "AssetLoader.h"

using namespace std;

AssetLoader::AssetLoader()
{
	
}

AssetLoader::~AssetLoader(void)
{
	Unload();
}

Shader* AssetLoader::getShader(string vertex, string fragment)
{
	if (!shaders.count({ vertex, fragment }))
	{
		loadShader(vertex, fragment);
	}
	return shaders[{vertex, fragment}];
}


//TODO there is room for loadMesh etc functions to prevent code copying
Mesh* AssetLoader::getMesh(std::string file) 
{
	
	if (!meshes.count(file))
	{
		loadMesh(file);
	}
	return meshes[file];
	
}

GLuint* AssetLoader::getTexture(std::string file) 
{
	if (!textures.count(file))
	{
		loadTexture(file);
	}
	return &textures[file];
}

void AssetLoader::Load()
{
	for each (auto file in meshFiles)
	{
		loadMesh(file);
	}
	for each (auto pair in shaderFiles)
	{
		loadShader(pair.first,pair.second);
	}
	for each (auto file in textureFiles)
	{
		loadTexture(file);
	}
}

void AssetLoader::Unload()
{
	for each (auto pair in meshes)
	{
		delete pair.second;
	}
	for each (auto pair in shaders)
	{
		delete pair.second;
	}
	meshes = {};
	shaders = {};
	textures = {};
}

Shader* AssetLoader::loadShader(std::string vertex, std::string fragment)
{
	pair<string, string> pair = { vertex,fragment };
	Shader* shader = new Shader(pair.first, pair.second);
	if (!shader->LoadSuccess())
	{
		throw invalid_argument("Incorrect shader");
	}
	shaders.insert({ pair, shader });
	return shader;
}

Mesh* AssetLoader::loadMesh(std::string file)
{
	Mesh* mesh = Mesh::LoadFromMeshFile(file);
	if (!mesh)
	{
		throw invalid_argument("Incorrect mesh");
	}
	meshes.insert({ file, mesh });
	return mesh;
}

GLuint* AssetLoader::loadTexture(std::string file)
{
	GLuint texture = SOIL_load_OGL_texture(("../Textures/" + file).c_str(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!texture)
	{
		throw invalid_argument("Incorrect texture");
	}
	textures.insert({ file, texture });
	return &texture;
}



