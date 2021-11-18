#include "Island.h"
#include "OGLRenderer.cpp"

Island::Island() : SceneNode("Island")
{
	heightMap = new  HeightMap(TEXTUREDIR"noise.png");

	Vector3  dimensions = heightMap->GetHeightmapSize();

	shader = new  Shader("HeightVertex.glsl", "HeightFragment.glsl");

	terrainTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	OGLRenderer::SetTextureRepeating(terrainTex, true);

}

Island::~Island()
{
	delete heightMap;
}

void Island::Draw(OGLRenderer& r)
{
	r.BindShader(GetShader());
	r.UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTex);
	heightMap->Draw();
}

void Island::Update(float dt)
{
}
