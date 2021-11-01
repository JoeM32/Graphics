#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	void UpdateTextureMatrix(float rotation, float scale);
	void ToggleRepeating();
	void ToggleFiltering();

	void SetBlending(float value);

	virtual void UpdateScene(float dt);
	void SwitchToPerspective();
	void SwitchToOrthographic();

protected:
	Shader* shader;
	Mesh* triangle;
	GLuint texture;
	GLuint texture2;
	bool filtering;
	bool repeating;
	float blending = 0;

	float fov = 90.0f;
	Camera* camera;
};
