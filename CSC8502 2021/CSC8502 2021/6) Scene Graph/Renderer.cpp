#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	camera = new Camera();

	defaultShader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");

	if (!defaultShader->LoadSuccess()) {
		return;

	}

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30, 175));

	root = new SceneNode();
	for (int i = 0; i < 10; i++)
	{
		auto robot = new CubeRobot(cube);
		root->AddChild(robot);
		auto transform = robot->GetTransform();
		transform = transform * Matrix4::Translation(Vector3(i * 60,0,0));
		if (i == 9)
		{
			transform = transform * Matrix4::Translation(Vector3(200, 0, 0));
			transform = transform * Matrix4::Scale(Vector3(10, 10, 10));
		}
		robot->SetTransform(transform);
	}

	glEnable(GL_DEPTH_TEST);
	init = true;

}

 Renderer ::~Renderer(void) {
	 delete root;
	 delete defaultShader;
	 delete camera;
	 delete cube;
	
}

void Renderer::UpdateScene(float dt) {
	 camera -> UpdateCamera(dt);
	 viewMatrix = camera -> BuildViewMatrix();
	 root -> Update(dt);
	
}

void Renderer::RenderScene() {

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	auto shader = defaultShader;
	DrawNode(root);
}void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {

		auto shader = defaultShader;
		if (n->GetShader())
		{
			shader = n->GetShader();
		}

		BindShader(shader);
		UpdateShaderMatrices();

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"diffuseTex"), 1);



		Matrix4 model = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(
			glGetUniformLocation(shader->GetProgram(),
				"modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(shader->GetProgram(),
			"nodeColour"), 1, (float*)& n->GetColour());

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"useTexture"), 0); // Next tutorial ;)
		n->Draw(*this);

	}

	for (vector < SceneNode* >::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);

	}
}