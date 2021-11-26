# pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustrum.h"
#include "../nclgl/PlayerCamera.h"
#include "../nclgl/CutsceneCamera.h"

class Camera;
class SceneNode;
class Mesh;
class Shader;

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void UpdateScene(float msec) override;
	void RenderScene() override;
	GLuint GetShadowTex() override;
	GLuint GetSkybox() override;
	void SetShaderLights() override;
	virtual Vector3 GetCameraPos();
protected:

	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();

	void GenerateScreenTexture(GLuint& into, bool depth = false);

	GLuint earthTex;
	Shader* sceneShader;
	Shader* pointlightShader;
	Shader* combineShader;
	GLuint bufferFBO;
	GLuint bufferColourTex;
	GLuint bufferNormalTex;
	GLuint bufferDepthTex;
	GLuint pointLightFBO;
	GLuint lightDiffuseTex;
	GLuint lightSpecularTex;	GLuint bufferFBO2;	GLuint bufferColourTex2;	GLuint bufferDepthTex2;

	Light* pointLights; // Array of lighting data
	Mesh* sphere; // Light volume
	GLuint earthBump;




	void BuildNodeLists(SceneNode* from,int camera);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	void DrawNodesRaw();
	void DrawNodeRaw(SceneNode* n);

	void DrawShadowScene();
	GLuint shadowTex;
	GLuint shadowFBO;
	Shader* shadowShader;

	SceneNode* root;
	Camera* mainCamera[2];
	int currentCamera = 0;//not a fan of this solution, but the shaders are epxtcing to reach in and just grab the one camera.
	Mesh* quad;
	Mesh* cube;
	Shader* shader;
	GLuint texture;

	Light* light;

	Frustum frameFrustum[2];

	vector < SceneNode* > transparentNodeList;
	vector < SceneNode* > nodeList;
		GLuint skybox;	void DrawSkybox();	Shader* skyboxShader;};