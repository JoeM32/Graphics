#include "Renderer.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Island.h"
#include "../nclgl/AssetLoader.h"
#include "../nclgl/AssetLoaderSingleton.h"
#include <algorithm> // For std :: sort ...

constexpr auto SHADOWSIZE = 2048;

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

	//light = new Light(Vector3(12.0f, 6.0f, 22.0f) * 100,
	//	Vector4(1, 1, 1, 1), 250.0f);

	light = new Light(Vector3(2000, 800, 2000),
		Vector4(1, 1, 1, 1), 2500.0f);

	shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);

}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadowShader);
	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	projMatrix = Matrix4::Perspective(1, 100, 1, 45);
	shadowMatrix = projMatrix * viewMatrix; // used later

	DrawNodesRaw(); 

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	//light->SetPosition(camera->GetPosition());
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

void Renderer::DrawNodesRaw() {
	for (const auto& i : nodeList) {
		DrawNodeRaw(i);

	}
}

void Renderer::DrawNode(SceneNode* n) {
	modelMatrix = n->GetTransform();
	UpdateShaderMatrices();
	n->Draw(*this);
}

void Renderer::DrawNodeRaw(SceneNode* n) {
	modelMatrix = n->GetTransform();
	UpdateShaderMatrices();
	n->DrawRaw(*this);
}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawShadowScene();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawNodes();

	ClearNodeLists();

}

GLuint Renderer::GetShadowTex()
{
	return shadowTex;
}

void Renderer::SetShaderLights() 
{
	SetShaderLight(*light);

}

Vector3 Renderer::GetCameraPos()
{
	return camera->GetPosition();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}