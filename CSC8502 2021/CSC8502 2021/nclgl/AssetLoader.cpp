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
	return shaders[{vertex, fragment}];
}

Mesh* AssetLoader::getMesh(std::string file) 
{
	return meshes[file];
}

GLuint* AssetLoader::getTexture(std::string file) 
{
	return &textures[file];
}

void AssetLoader::Load()
{
	for each (auto file in meshFiles)
	{
		Mesh* mesh = Mesh::LoadFromMeshFile(file);
		meshes.insert({ file, mesh });
	}
	for each (auto pair in shaderFiles)
	{
		Shader* shader = new Shader(pair.first, pair.second);
		if (!shader->LoadSuccess())
		{
			throw invalid_argument("Incorrect shader");
		}
		shaders.insert({ pair, shader });
	}
	for each (auto file in textureFiles)
	{
		GLuint texture = SOIL_load_OGL_texture(("../Textures/" + file).c_str(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

		if (!texture)
		{
			throw invalid_argument("Incorrect texture");
		}
		textures.insert({ file, texture });

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



