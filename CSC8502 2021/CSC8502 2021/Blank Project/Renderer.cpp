#include "Renderer.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Camera.h"
#include "../nclgl/AssetLoader.h"
#include "../nclgl/AssetLoaderSingleton.h"
#include <algorithm> // For std :: sort ...

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	AssetLoaderSingleton::loader.Load();
	camera = new Camera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)));
	quad = Mesh::GenerateQuad();
	//cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	cube = AssetLoaderSingleton::loader.getMesh("OffsetCubeY.msh");
	//cube = AssetLoaderSingleton::loader.getMesh("OffsetCubeY.msh");

	//shader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
	/*std::cout << shader << "\n";
	shader = loader1.getShader("SceneVertex.glsl", "SceneFragment.glsl");

	if (shader == nullptr)
	{
		std::cout << "NULL1";
	}
	/*texture = SOIL_load_OGL_texture(TEXTUREDIR "stainedglass.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);*/

	texture = *AssetLoaderSingleton::loader.getTexture("stainedglass.tga");

	/*if (!shader->LoadSuccess() || !texture) {
		return;

	}*/

	root = new SceneNode("root");

	/*for (int i = 0; i < 5; ++i) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation(
			Vector3(0, 100.0f, -300.0f + 100.0f + 100 * i)));
		s->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
		s->SetBoundingRadius(100.0f);
		s->SetMesh(quad);
		s->SetTexture(texture);
		root->AddChild(s);

	}
	*/
	SceneNode* robro = new CubeRobot(cube);


	root->AddChild(robro);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init = true;

}

Renderer ::~Renderer(void) {
	delete root;
	delete quad;
	delete camera;
	AssetLoaderSingleton::loader.Unload();
	glDeleteTextures(1, &texture);

}
void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);

	root->Update(dt);

}

void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);

		}
		else {
			nodeList.push_back(from);

		}

	}

	for (vector < SceneNode* >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));

	}

}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), // note the r!
		transparentNodeList.rend(), // note the r!
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);

}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {
		DrawNode(i);

	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);

	}

}

void Renderer::DrawNode(SceneNode* n) {

	std::cout << n->GetName() << "\n";
	if (n->GetMesh()) {

		n->Draw(*this);

	}

}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/*BindShader(shader);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(shader->GetProgram(),
		"diffuseTex"), 0);*/
	DrawNodes();

	ClearNodeLists();

}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}