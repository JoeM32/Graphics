#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	meshes[0] = Mesh::GenerateTriangle();
	meshes[1] = Mesh::GenerateQuad();

	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR "brick.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR "star.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (!textures[0] || !textures[1]) {
		return;

	}

	SetTextureRepeating(textures[0], true);
	SetTextureRepeating(textures[1], true);

	shader[0] = new Shader("TexturedVertex.glsl", "StencilFragment.glsl");
	if (!shader[0]->LoadSuccess()) {
		return;

	}
	shader[1] = new Shader("TexturedVertex.glsl", "StencilFragment2.glsl");
	if (!shader[1]->LoadSuccess()) {
		return;

	}
	usingScissor = false;
	usingStencil = false;
	init = true;

}

Renderer ::~Renderer(void) {
	delete meshes[0];
	delete meshes[1];
	delete shader[0];
	delete shader[1];
}

void Renderer::ToggleScissor() {
	 usingScissor = !usingScissor;
	
}

 void Renderer::ToggleStencil() {
	 usingStencil = !usingStencil;
	
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
		 | GL_STENCIL_BUFFER_BIT);

	 if (usingScissor) {
		 glEnable(GL_SCISSOR_TEST);
		 glScissor((float)width / 2.5f, (float)height / 2.5f,
			 (float)width / 5.0f, (float)height / 5.0f);

	 }

	 BindShader(shader[0]);

	 textureMatrix = Matrix4::Scale(Vector3(6, 6, 6));
	 UpdateShaderMatrices();

	 glUniform1i(glGetUniformLocation(shader[0]->GetProgram(),
		 "diffuseTex"), 0);
		 glEnable(GL_STENCIL_TEST);

		 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		 glStencilFunc(GL_ALWAYS, 2, ~0);
		 glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		 glBindTexture(GL_TEXTURE_2D, textures[1]);
		 meshes[1]->Draw();

		 glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		 glStencilFunc(GL_EQUAL, 2, ~0);
		 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	 UpdateShaderMatrices();
		 "diffuseTex"), 0);

	 meshes[0]->Draw();
	 glDisable(GL_SCISSOR_TEST);
	 glDisable(GL_STENCIL_TEST);

 }