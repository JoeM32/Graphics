#include "Renderer.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Island.h"
#include "../nclgl/AssetLoader.h"
#include "../nclgl/AssetLoaderSingleton.h"
#include <algorithm> // For std :: sort ...

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	AssetLoaderSingleton::loader.Load();
	camera = new PlayerCamera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)));
	//camera = new CutsceneCamera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)), 5);
	//camera->AddShot({ 0.0f, 0.0f, (Vector3(0, 5, 250.0f)), 5 });
	//camera->Play();
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


	root = new SceneNode("root");

	SceneNode* robro = new CubeRobot(cube);


	root->AddChild(robro);
	SceneNode* island = new Island();
	root->AddChild(island);

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

	n->Draw(*this);
}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawNodes();

	ClearNodeLists();

}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}