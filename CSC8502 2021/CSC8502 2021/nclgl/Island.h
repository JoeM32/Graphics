#pragma once
#include "..\nclgl\SceneNode.h"
#include "../nclgl/HeightMap.h"
class Island : public SceneNode {
public:
	Island();
	~Island();
	virtual void Draw(OGLRenderer& r) override;
	virtual void DrawRaw(OGLRenderer& r) override;
	HeightMap* getHeightMap() { return heightMap;  };
	void Update(float dt) override;

	

protected:
	HeightMap* heightMap;
	Shader* shader;

	GLuint grassTex;
	GLuint grassBumpTex;
	GLuint grass2Tex;
	GLuint grass2BumpTex;
	GLuint mudTex;
	GLuint mudBumpTex;
	GLuint rockTex;
	GLuint rockBumpTex;
	GLuint sandTex;
	GLuint sandBumpTex;
	GLuint noise;
};

