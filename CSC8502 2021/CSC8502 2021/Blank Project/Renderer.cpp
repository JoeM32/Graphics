#include "Renderer.h"
#include "../nclgl/CubeRobot.h"
#include "../nclgl/Island.h"
#include "../nclgl/Ocean.h"
#include "../nclgl/Rock.h"
#include "../nclgl/AssetLoader.h"
#include "../nclgl/AssetLoaderSingleton.h"
#include <algorithm> // For std :: sort ...

constexpr auto SHADOWSIZE = 2048;
constexpr auto LIGHT_NUM = 30;

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	AssetLoaderSingleton::loader.Load();
	AssetLoaderSingleton::loader.SetFiltering(true);


	
	auto cutscene = new CutsceneCamera(5.7f, 241, (Vector3(-375, 100, 437.5)), 10);
	cutscene->AddShot({ 210, -10, Vector3(964, 767, -46), 10 });
	cutscene->AddShot({ 184, 4.96, Vector3(1966, 383, 441), 10 });
	cutscene->AddShot({ 129, -35.1, Vector3(3548, 1734, 994), 10 });
	cutscene->AddShot({ 90.9, -64.6, Vector3(3397, 2718, 2202), 10 });
	cutscene->AddShot({ 5.3, 10.3, Vector3(1982, 150, 4255), 10 });
	cutscene->AddShot({ 261, -21, Vector3(-190, 951, 1492), 10 });
	cutscene->AddShot({ 181, -28, Vector3(2002, 1350, -271), 10 });
	cutscene->Play();
	mainCamera[0] = cutscene;
	//mainCamera[0] = new PlayerCamera(0.0f, 0.0f, (Vector3(0, 100, 750.0f))); //change to controllable camera
	mainCamera[1] = new Camera(-90, 0.0f, (Vector3(0, 100, 750.0f)));
	quad = Mesh::GenerateQuad();
	cube = AssetLoaderSingleton::loader.getMesh("OffsetCubeY.msh");
	
	light = new Light(Vector3(500, 1000, 500),
		Vector4(1, 1, 1, 1), 2700);

	shadowShader = AssetLoaderSingleton::loader.getShader("shadowVert.glsl", "shadowFrag.glsl");

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

	//SceneNode* robro = new CubeRobot(cube);


	//root->AddChild(robro);
	Island* island = new Island();
	//island->getHeightMap()->rockSpots;
	for each (Vector3 var in island->getHeightMap()->rockSpots)
	{
		SceneNode* rock = new Rock();
		rock->SetTransform(Matrix4::Translation(var) * rock->GetTransform());
		island->AddChild(rock);

	}
	root->AddChild(island);
	
	SceneNode* ocean = new Ocean();
	root->AddChild(ocean);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);


	//deferred
	sphere = AssetLoaderSingleton::loader.getMesh("Sphere.msh");

	pointLights = new Light[LIGHT_NUM];

	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		/*l.SetPosition(Vector3(rand() % (int)4000,
			rand() % (int)400,
			rand() % (int)4000));*/

		l.SetPosition(island->getHeightMap()->rockSpots.at(rand() % (island->getHeightMap()->rockSpots.size() - 1)) + Vector3(0,20,0));
		l.SetColour(Vector4(0.05,1,0.1,
			1));
		l.SetRadius(250.0f + (rand() % 250));

	}

	sceneShader = AssetLoaderSingleton::loader.getShader("BumpVertex.glsl", // reused !
		"bufferFragment.glsl");
	pointlightShader = AssetLoaderSingleton::loader.getShader("pointlightvert.glsl",
		"pointlightfrag.glsl");
	combineShader = AssetLoaderSingleton::loader.getShader("combinevert.glsl",
		"combinefrag.glsl");


	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &pointLightFBO);

	GLenum buffers[2] = {
	GL_COLOR_ATTACHMENT0 ,
	GL_COLOR_ATTACHMENT1
	};
	// Generate our scene depth texture ...
	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(lightDiffuseTex);
	GenerateScreenTexture(lightSpecularTex);
	// And now attach them to our FBOs
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightDiffuseTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	//skybox
	skybox = SOIL_load_OGL_cubemap(
		TEXTUREDIR "west.png", TEXTUREDIR "east.png",
		TEXTUREDIR "up.png", TEXTUREDIR "down.png",
		TEXTUREDIR "south.png", TEXTUREDIR "north.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!skybox) {
		std::cout << "Skybox FAILED";
	}
	skyboxShader = AssetLoaderSingleton::loader.getShader(
		"skyboxVertex.glsl", "skyboxFragment.glsl");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//second camera

	glGenFramebuffers(1, &bufferFBO2);

	glGenTextures(1, &bufferColourTex2);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 300, 300, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &bufferDepthTex2);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 300, 300, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex2, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex2, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	init = true;

}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	//modelMatrix.ToIdentity(); 
	//textureMatrix.ToIdentity(); 
	UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);

}

void Renderer::GenerateScreenTexture(GLuint& into, bool depth) {
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
	GLuint type = depth ? GL_DEPTH_COMPONENT : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0,
		format, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

}

Renderer ::~Renderer(void) {
	delete root;
	delete quad;
	delete mainCamera[0];
	delete mainCamera[1];
	AssetLoaderSingleton::loader.Unload();
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &bufferFBO2);
	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferColourTex2);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &pointLightFBO);
	glDeleteTextures(1, &lightDiffuseTex);
	glDeleteTextures(1, &lightSpecularTex);
	delete[] pointLights;
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
		light->GetPosition(), Vector3(7000,-1000,5000));
	projMatrix = Matrix4::Perspective(1, 6000, 1, 45);


	shadowMatrix = projMatrix * viewMatrix; // used later

	UpdateShaderMatrices();
	DrawNodesRaw(); 

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::UpdateScene(float dt) {
	mainCamera[0]->UpdateCamera(dt);
	viewMatrix = mainCamera[0]->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	frameFrustum[0].FromMatrix(projMatrix * viewMatrix);

	mainCamera[1]->UpdateCamera(dt);
	mainCamera[1]->SetPosition(mainCamera[0]->GetPosition() + Vector3(0, 5000, 0));
	mainCamera[1]->SetYaw(mainCamera[0]->GetYaw());
	viewMatrix = mainCamera[1]->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		1, 20.0);
	frameFrustum[1].FromMatrix(projMatrix * viewMatrix);

	//light->SetPosition(camera->GetPosition());
	//Light& l = pointLights[0];
	//l.SetPosition(camera->GetPosition() - Vector3(0,250,0));
	//light->SetRadius(camera->GetPosition().y);

	root->Update(dt);
}

void Renderer::BuildNodeLists(SceneNode* from, int camera) {
	if (frameFrustum[camera].InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			mainCamera[camera]->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
			//std::cout << from->GetName();
		}
		else {
			nodeList.push_back(from);

		}

	}

	for (vector < SceneNode* >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists(*i, camera);

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
	//std::cout << "Starting for camera " << std::to_string(currentCamera) << "\n";
	for (const auto& i : nodeList) {
		DrawNode(i);
		//std::cout << i->GetName() << "\n";
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);

	}
	//std::cout << "Nodes drawn" << "\n";
}

void Renderer::DrawNodesRaw() {
	for (const auto& i : nodeList) {
		DrawNodeRaw(i);

	}
}

void Renderer::DrawNode(SceneNode* n) {
	modelMatrix = n->GetTransform();
	//modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	n->Draw(*this);
}

void Renderer::DrawNodeRaw(SceneNode* n) {
	modelMatrix = n->GetTransform();
	//modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	n->DrawRaw(*this);
}

void Renderer::RenderScene() {

	currentCamera = 0;
	BuildNodeLists(root,currentCamera);
	SortNodeLists();

	//shadows
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawShadowScene();
	viewMatrix = mainCamera[currentCamera]->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	FillBuffers();
	DrawPointLights();

	ClearNodeLists();
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO2);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 300, 300);
	currentCamera = 1;
	viewMatrix = mainCamera[currentCamera]->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		1, 20);
	UpdateShaderMatrices();
	BuildNodeLists(root, currentCamera);
	SortNodeLists();
	DrawNodes();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	CombineBuffers();

	ClearNodeLists();

}

void Renderer::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();

	modelMatrix.ToIdentity();
	viewMatrix = mainCamera[currentCamera]->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	DrawNodes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawPointLights() {
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	BindShader(pointlightShader);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	glUniform1i(glGetUniformLocation(
		pointlightShader->GetProgram(), "depthTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glUniform1i(glGetUniformLocation(
		pointlightShader->GetProgram(), "normTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(pointlightShader->GetProgram(),
		"cameraPos"), 1, (float*)&mainCamera[currentCamera]->GetPosition());

	glUniform2f(glGetUniformLocation(pointlightShader->GetProgram(),
		"pixelSize"), 1.0f / width, 1.0f / height);

	Matrix4 invViewProj = (projMatrix * viewMatrix).Inverse();
	glUniformMatrix4fv(glGetUniformLocation(
		pointlightShader->GetProgram(), "inverseProjView"),
		1, false, invViewProj.values);
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		SetShaderLight(l);
		sphere->Draw();

	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::CombineBuffers() {
	BindShader(combineShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "diffuseLight"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightDiffuseTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "specularLight"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	glUniform1i(glGetUniformLocation(
		combineShader->GetProgram(), "miniMap"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex2);

	glUniform1f(glGetUniformLocation(
		combineShader->GetProgram(), "height"), (float)height);

	glUniform1f(glGetUniformLocation(
		combineShader->GetProgram(), "width"), (float)width);


	quad->Draw();

}




GLuint Renderer::GetShadowTex()
{
	return shadowTex;
}

GLuint Renderer::GetSkybox()
{
	return skybox;
}

void Renderer::SetShaderLights() 
{
	SetShaderLight(*light);

}

Vector3 Renderer::GetCameraPos()
{
	return mainCamera[currentCamera]->GetPosition();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}