#pragma once
#include "..\nclgl\SceneNode.h"
class Rock : public SceneNode {
public:
	Rock();
	~Rock();
	virtual void Draw(OGLRenderer& r) override;
	virtual void DrawRaw(OGLRenderer& r) override;
	void Update(float dt) override;
protected:
	Shader* shader;

	GLuint diffuseTex;
	GLuint bumpTex;

};