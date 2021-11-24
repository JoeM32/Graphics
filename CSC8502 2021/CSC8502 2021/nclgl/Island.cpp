#include "Island.h"

Island::Island() : SceneNode("Island")
{
	heightMap = new  HeightMap(TEXTUREDIR"IslandHeightmap.png");
	Vector3  dimensions = heightMap->GetHeightmapSize();

	shader = AssetLoaderSingleton::loader.getShader("IslandVertex.glsl", "IslandFragment.glsl");


	rockTex = *AssetLoaderSingleton::loader.getTexture("rock.png");
	rockBumpTex = *AssetLoaderSingleton::loader.getTexture("rockBump.png");
	grassTex = *AssetLoaderSingleton::loader.getTexture("GRASS1.jfif");
	grassBumpTex = *AssetLoaderSingleton::loader.getTexture("grassBump.png");
	mudTex = *AssetLoaderSingleton::loader.getTexture("dirt.png");
	mudBumpTex = *AssetLoaderSingleton::loader.getTexture("dirtBump.png");
	sandTex = *AssetLoaderSingleton::loader.getTexture("sand.png");
	sandBumpTex = *AssetLoaderSingleton::loader.getTexture("sandBump.png");
	grass2Tex = *AssetLoaderSingleton::loader.getTexture("grass2.png");
	grass2BumpTex = *AssetLoaderSingleton::loader.getTexture("grass2Bump.png");
	noise = *AssetLoaderSingleton::loader.getTexture("IslandNoise.jpg");


	OGLRenderer::SetTextureRepeating(rockTex, true);
	OGLRenderer::SetTextureRepeating(rockBumpTex, true);
	OGLRenderer::SetTextureRepeating(grass2Tex, true);
	OGLRenderer::SetTextureRepeating(grass2BumpTex, true);
	OGLRenderer::SetTextureRepeating(sandTex, true);
	OGLRenderer::SetTextureRepeating(sandBumpTex, true);
	OGLRenderer::SetTextureRepeating(mudTex, true);
	OGLRenderer::SetTextureRepeating(mudBumpTex, true);
	OGLRenderer::SetTextureRepeating(grassTex, true);
	OGLRenderer::SetTextureRepeating(grassBumpTex, true);
	OGLRenderer::SetTextureRepeating(noise, true);

	SetBoundingRadius(dimensions.Length());
	//transform = Matrix4::Translation(dimensions/2);

}

Island::~Island()
{
	delete heightMap;
}

void Island::Draw(OGLRenderer& r)
{
	r.BindShader(shader);
	r.SetShaderLights();
	r.UpdateShaderMatrices();
	int i = 0;
	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"rockTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, rockTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"rockBumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, rockBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"sandTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, sandTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"sandBumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, sandBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"grassTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, grass2Tex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"grassBumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, grass2BumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"mudTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, mudTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"mudBumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, mudBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"noise"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, noise);
	i++;

	/*glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"shadowTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, r.GetShadowTex());
	i++;*/

	/*glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rockTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rockBumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"shadowTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, r.GetShadowTex());
	i++;*/

	glUniform3fv(glGetUniformLocation(shader->GetProgram(),
		"cameraPos"), 1, (float*)&r.GetCameraPos());






	/*glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTex);*/
	heightMap->Draw();
}

void Island::DrawRaw(OGLRenderer& r)
{
	heightMap->Draw();
}

void Island::Update(float dt)
{
}
