#include "Renderer.h"
#include "../nclgl/Light.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	heightMap = new HeightMap(TEXTUREDIR "noise.png");
	texture = SOIL_load_OGL_texture(
		TEXTUREDIR "Barren Reds.jpg ", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	shader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");

	if (!shader->LoadSuccess() || !texture) {
		return;

	}
	SetTextureRepeating(texture, true);	Vector3 heightmapSize = heightMap->GetHeightmapSize();
	camera = new Camera(-45.0f, 0.0f,
		heightmapSize * Vector3(0.5f, 5.0f, 0.5f));
	for (int i = 0; i < 4; i++)
	{
		lights[i] = new Light(heightmapSize * Vector3((i+1) * 0.5f, 1.5f, (3- i) * 0.5f),
			Vector4(1, 1, 1, 1), Vector4(0, 1, 0, 1), heightmapSize.x * 0.2f);
	}

	std::cout << "rough values11111111" << "\n";	for (int i = 0; i < 4; i++)
	{

		std::cout << std::to_string(lights[i]->GetColourSpecular().x) << "\n";

		std::cout << std::to_string(lights[i]->GetColourSpecular().y) << "\n";

		std::cout << std::to_string(lights[i]->GetColourSpecular().z) << "\n";

		std::cout << std::to_string(lights[i]->GetColourSpecular().w) << "\n";
	}

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer ::~Renderer(void) {
	 delete camera;
	 delete heightMap;
	 delete shader;
	 for (int i = 0; i < 4; i++)
	 {
		 delete lights[i]; // This bit is new ...
	 }
	
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(shader);
	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform3fv(glGetUniformLocation(shader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	UpdateShaderMatrices();
	SetShaderLights(*lights);

	heightMap->Draw();

}