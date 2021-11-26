#include "Rock.h"

Rock::Rock() : SceneNode("Rock")
{
	shader = AssetLoaderSingleton::loader.getShader("defaultVert.glsl", "defaultFragment.glsl");
	diffuseTex = *AssetLoaderSingleton::loader.getTexture("rock.png");
	bumpTex = *AssetLoaderSingleton::loader.getTexture("rockBump.png");
	int type = (int)rand() % 3;
	if (type == 0)
	{
		mesh = AssetLoaderSingleton::loader.getMesh("Rock_03_LOD0.msh");
		//diffuseTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_03_D.TGA");
		//bumpTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_03_N.TGA");
		this->SetTransform(Matrix4::Rotation(rand() % 90, Vector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50)) * Matrix4::Scale(Vector3(20, 20, 20)) * Matrix4::Translation(Vector3(-5, -2, -10)));
	}
	else if(type == 1)
	{
		mesh = AssetLoaderSingleton::loader.getMesh("Rock_04_LOD1.msh");
		//diffuseTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_04_D.TGA");
		//bumpTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_04_N.TGA");
		this->SetTransform(Matrix4::Rotation(rand() % 90, Vector3(rand() % 100 - 50, rand () % 100 - 50, rand () % 100 - 50)) * Matrix4::Scale(Vector3(20, 20, 20)) * Matrix4::Translation(Vector3(-16, -6, -12)));
	}
	else
	{
		mesh = AssetLoaderSingleton::loader.getMesh("Rock_05_LOD1.msh");
		//diffuseTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_05_D.TGA");
		//bumpTex = *AssetLoaderSingleton::loader.getTexture("T_Rock_05_N.TGA");
		this->SetTransform(Matrix4::Rotation(rand() % 90, Vector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50)) * Matrix4::Scale(Vector3(20, 20, 20)) * Matrix4::Translation(Vector3(-12, -8, -18)));
	}
	boundingRadius = 5000;
}

Rock::~Rock()
{
}

void Rock::Draw(OGLRenderer& r)
{
	r.BindShader(shader);
	r.SetShaderLights();
	r.UpdateShaderMatrices();
	int i = 0;
	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"DiffuseTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, diffuseTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"BumpTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, bumpTex);
	i++;

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"shadowTex"), i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, r.GetShadowTex());
	i++;

	glUniform1f(glGetUniformLocation(
		shader->GetProgram(), "size"), (float)0.005);

	mesh->Draw();
}

void Rock::DrawRaw(OGLRenderer& r)
{
	mesh->Draw();
}

void Rock::Update(float dt)
{

}
