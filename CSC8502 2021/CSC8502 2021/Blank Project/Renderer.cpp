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


	camera = new PlayerCamera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)));
	//camera = new CutsceneCamera(0.0f, 0.0f, (Vector3(0, 100, 750.0f)), 5);
	//camera->AddShot({ 0.0f, 0.0f, (Vector3(0, 5, 250.0f)), 5 });
	//camera->Play();
	quad = Mesh::GenerateQuad();
	cube = AssetLoaderSingleton::loader.getMesh("OffsetCubeY.msh");
	
	light = new Light(Vector3(500, 1000, 500),
		Vector4(1, 1, 1, 1), 6000);

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


	//shadows 2

	/*glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	root = new SceneNode("root");

	SceneNode* robro = new CubeRobot(cube);


	root->AddChild(robro);
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

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//deferred
	sphere = Mesh::LoadFromMeshFile("Sphere.msh");

	pointLights = new Light[LIGHT_NUM];

	for (int i = 0; i < LIGHT_NUM; ++i) {
		Light& l = pointLights[i];
		l.SetPosition(Vector3(rand() % (int)4000,
			rand() % (int)400,
			rand() % (int)4000));

		l.SetColour(Vector4(0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			0.5f + (float)(rand() / (float)RAND_MAX),
			1));
		l.SetRadius(250.0f + (rand() % 250));

	}

	sceneShader = new Shader("BumpVertex.glsl", // reused !
		"bufferFragment.glsl");
	pointlightShader = new Shader("pointlightvert.glsl",
		"pointlightfrag.glsl");
	combineShader = new Shader("combinevert.glsl",
		"combinefrag.glsl");

	if (!sceneShader->LoadSuccess() || !pointlightShader->LoadSuccess() ||
		!combineShader->LoadSuccess()) {
		return;

	}



	earthTex = SOIL_load_OGL_texture(
		TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	earthBump = SOIL_load_OGL_texture(
		TEXTUREDIR"Barren RedsDOT3.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);

	if (!pointlightShader->LoadSuccess() ||
		!combineShader->LoadSuccess()) {
		return;

	}
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
	skyboxShader = new Shader(
		"skyboxVertex.glsl", "skyboxFragment.glsl");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




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
		light->GetPosition(), Vector3(7000,-1000,5000));
	projMatrix = Matrix4::Perspective(1, 6000, 1, 45);

	/*viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 1500.0f,
		(float)width / (float)height, 45.0f);*/

	/*Matrix4 lightProjection = Matrix4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 1, 7.5);
	Matrix4 lightView = Matrix4::Rotation(30, Vector3(0, 1, 0));
	shadowMatrix = lightProjection * lightView;*/

	shadowMatrix = projMatrix * viewMatrix; // used later

	UpdateShaderMatrices();
	DrawNodesRaw(); 

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*void Renderer::DrawShadowScene() {
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	float near_plane = 1.0f, far_plane = 7.5f;
	Matrix4 lightProjection = Matrix4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	Matrix4 lightView = Matrix4::Rotation(30, Vector3(0, 1, 0));
	shadowMatrix = lightProjection * lightView;
	UpdateShaderMatrices();
	BindShader(shadowShader);
	//set shader
	//put in shadoMatrix
	DrawNodesRaw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// 2. then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	DrawNodes();
}*/

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	//light->SetPosition(camera->GetPosition());
	//Light& l = pointLights[0];
	//l.SetPosition(camera->GetPosition() - Vector3(0,250,0));
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

	BuildNodeLists(root);
	SortNodeLists();

	//shadows
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawShadowScene();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);


	FillBuffers();
	DrawPointLights();
	CombineBuffers();

	/*glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f); 
	DrawNodes();*/
	ClearNodeLists();

}

void Renderer::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	/*BindShader(sceneShader);
	glUniform1i(
		glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(
		glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);*/

	modelMatrix.ToIdentity();
	viewMatrix = camera->BuildViewMatrix();
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
		"cameraPos"), 1, (float*)&camera->GetPosition());

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
	return camera->GetPosition();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}