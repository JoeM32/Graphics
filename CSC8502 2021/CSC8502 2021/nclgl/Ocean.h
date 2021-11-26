#pragma once
#include "..\nclgl\SceneNode.h"
#include "../nclgl/HeightMap.h"
class Ocean : public SceneNode {
public:
	Ocean();
	~Ocean();
	virtual void Draw(OGLRenderer& r) override;
	virtual void DrawRaw(OGLRenderer& r) override;
	HeightMap* getHeightMap() { return heightMap; };
	void Update(float dt) override;
	//float GetCameraDistance() const override { return 100000000000; };
protected:
	HeightMap* heightMap;
	Shader* shader;

	GLuint OceanTex;
	GLuint OceanBumpTex;

	float time;

	
};
