#include "Ocean.h"

/*Ocean::Ocean() : SceneNode("Ocean")
{
	heightMap = new  HeightMap(500,500);

	Vector3  dimensions = heightMap->GetHeightmapSize();

	//shader = AssetLoaderSingleton::loader.getShader("OceanVertex.glsl", "OceanFragment.glsl");
	shader = AssetLoaderSingleton::loader.getShader("TexturedVertex.glsl", "texturedFragment.glsl");

	OceanTex = *AssetLoaderSingleton::loader.getTexture("water.TGA");
	OceanBumpTex = *AssetLoaderSingleton::loader.getTexture("waterbump.png");


	OGLRenderer::SetTextureRepeating(OceanTex, true);
	OGLRenderer::SetTextureRepeating(OceanBumpTex, true);

	SetBoundingRadius(dimensions.Length());
}*/

Ocean::Ocean() : SceneNode("Ocean")
{
	heightMap = new  HeightMap(1000, 1000);
	Vector3  dimensions = heightMap->GetHeightmapSize();

	shader = AssetLoaderSingleton::loader.getShader("OceanVert.glsl", "OceanFrag.glsl");


	OceanTex = *AssetLoaderSingleton::loader.getTexture("water.TGA");
	OceanBumpTex = *AssetLoaderSingleton::loader.getTexture("waterbump.png");


	OGLRenderer::SetTextureRepeating(OceanTex, true);
	OGLRenderer::SetTextureRepeating(OceanBumpTex, true);

	SetBoundingRadius(dimensions.Length());


}

void Ocean::Draw(OGLRenderer& r)
{
	r.BindShader(shader);
	r.SetShaderLights();
	r.UpdateShaderMatrices();
	int i = 0;
	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"oceanTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, OceanTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"oceanBumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, OceanBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "cubeTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, r.GetSkybox());



	glUniform3fv(glGetUniformLocation(shader->GetProgram(),
		"cameraPos"), 1, (float*)&r.GetCameraPos());

	glUniform1f(glGetUniformLocation(shader->GetProgram(),
		"time"), time);

	heightMap->Draw();
}

Ocean::~Ocean()
{
	delete heightMap;
}


/*void Ocean::Draw(OGLRenderer& r)
{
	r.BindShader(shader);
	r.UpdateShaderMatrices();
	int i = 0;
	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, OceanTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"bumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, OceanBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "cubeTex"), i); 
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, r.GetSkybox());

	glUniform3fv(glGetUniformLocation(shader->GetProgram(),
		"cameraPos"), 1, (float*)&r.GetCameraPos());

	glUniform1f(glGetUniformLocation(shader->GetProgram(),
		"time"), time);

	heightMap->Draw();
}*/



void Ocean::DrawRaw(OGLRenderer& r)
{
	heightMap->Draw();
}

void Ocean::Update(float dt)
{
	time += dt;
}
