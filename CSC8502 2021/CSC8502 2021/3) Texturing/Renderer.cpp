#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();

	texture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	texture2 = SOIL_load_OGL_texture(TEXTUREDIR"water.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (!texture || !texture2) {
		return;

	}
	shader = new Shader("complexVertex.glsl",  "complexFragment.glsl");

	if (!shader->LoadSuccess()) {
		return;

	}
	filtering = true;
	repeating = false;
	init = true;

	SwitchToPerspective();

	camera = new Camera();
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(1 / (tan(fov / 2.0f))) * ((float)height / (float)width), 45.0f);

}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f,
		width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);

}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();

}

Renderer ::~Renderer(void) {
	delete camera;
	 delete triangle;
	 delete shader;
	 glDeleteTextures(1, &texture);
	 glDeleteTextures(1, &texture2);
	
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BindShader(shader);
	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram()
		, "projMatrix"), 1, false, projMatrix.values);

	float scale = 100.0f;
	float rotation = 0.0f;
	Vector3 position(0, 0, -1500.0f);

	for (int i = 0; i < 3; ++i) {
		Vector3 tempPos = position;
		tempPos.z += (i * 500.0f);
		tempPos.x -= (i * 100.0f);
		tempPos.y -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(tempPos) *
			Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
			Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(
			shader->GetProgram(), "modelMatrix"),
			1, false, modelMatrix.values);

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"diffuseTex"), 0); // this last parameter
		glActiveTexture(GL_TEXTURE0); // should match this number !
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"diffuseTex2"), 1); // this last parameter
		glActiveTexture(GL_TEXTURE0 + 1); // should match this number !
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUniform1f(glGetUniformLocation(shader->GetProgram()
			, "blending"), blending);


		triangle->Draw();

	}
}


void Renderer::UpdateTextureMatrix(float rotation, float scale) {
	 Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	 Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	 Matrix4 rotationMatrix = Matrix4::Rotation(rotation, Vector3(0, 0, 1));
	 textureMatrix = pop * rotationMatrix * push * Matrix4::Scale(Vector3(scale,scale,scale));
	
}

void Renderer::ToggleRepeating() {
	 repeating = !repeating;
	 SetTextureRepeating(texture, repeating);
	 SetTextureRepeating(texture2, repeating);
	
}
void Renderer::ToggleFiltering() {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		filtering ? GL_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		filtering ? GL_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		filtering ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::SetBlending(float value)
{
	blending = value;
}
