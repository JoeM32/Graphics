# pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	void PresentScene();
	void DrawPostProcess();
	void DrawPostProcess2();
	void DrawScene();

	Shader* sceneShader;
	Shader* processShader;

	Camera* camera;

	Mesh* quad;
	HeightMap* heightMap;
	GLuint heightTexture;
	GLuint bufferFBO;
	GLuint bufferFBO2;
	GLuint processFBO;
	GLuint bufferColourTex[3];
	GLuint bufferDepthTex;

};