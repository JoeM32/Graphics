#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();

	matrixShader = new Shader("MatrixVertex.glsl",
		"colourFragment.glsl");

	if (!matrixShader->LoadSuccess()) {
		return;

	}

	init = true;

	SwitchToOrthographic();

	camera = new Camera();
}

Renderer ::~Renderer(void) {
	delete triangle;
	delete matrixShader;
	delete camera;
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(1/(tan(fov/2.0f))) * ((float)height / (float)width), 45.0f);

}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f,
		width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);

}

void Renderer::UpdateScene(float dt) {
	camera -> UpdateCamera(dt);
	viewMatrix = camera -> BuildViewMatrix();
	//viewMatrix = viewMatrix * Matrix4::Scale(Vector3(2, 2, 2));

}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(matrixShader);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader -> GetProgram()
		, "projMatrix"), 1, false, projMatrix.values);


	for (int i = 0; i < 3; ++i) {
		Vector3 tempPos = position;
		tempPos.z += (i * 500.0f);
		tempPos.x -= (i * 100.0f);
		tempPos.y -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(tempPos) *
			Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
			Matrix4::Scale(Vector3(scale, scale, scale));

		//modelMatrix = Matrix4::Scale(Vector3(scale, scale, scale)) * Matrix4::Translation(tempPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0));

		glUniformMatrix4fv(glGetUniformLocation(
			matrixShader->GetProgram(), "modelMatrix"),
			1, false, modelMatrix.values);
		triangle->Draw();

	}

}
