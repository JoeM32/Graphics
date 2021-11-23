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
	GLuint lightSpecularTex;
	Light* pointLights; // Array of lighting data
	Mesh* sphere; // Light volume
	GLuint earthBump;




	void BuildNodeLists(SceneNode* from);
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
	PlayerCamera* camera;
	Mesh* quad;
	Mesh* cube;
	Shader* shader;
	GLuint texture;

	Light* light;

	Frustum frameFrustum;

	vector < SceneNode* > transparentNodeList;
	vector < SceneNode* > nodeList;
		GLuint depthMapFBO;
	GLuint depthMap;	GLuint skybox;	void DrawSkybox();	Shader* skyboxShader;};