#pragma once
#include "..\nclgl\SceneNode.h"
#include "../nclgl/HeightMap.h"
class Island : public SceneNode {
public:
	Island();
	~Island();
	virtual void Draw(OGLRenderer& r) override;
	void Update(float dt) override;
protected:
	HeightMap* heightMap;
	Shader* shader;
	GLuint terrainTex;

};

