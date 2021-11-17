#include "SceneNode.h"
#include <iostream>
#include <algorithm>
#include <vector>

SceneNode::SceneNode(std::string name, Mesh* mesh, Vector4 colour) : name(name) {
	 this -> mesh = mesh;
	 this -> colour = colour;
	 parent = NULL;
	 this->shader = AssetLoaderSingleton::loader.getShader("SceneVertex.glsl", "SceneFragment.glsl");
	 modelScale = Vector3(1, 1, 1);
	
	 //tutorial 7
	 boundingRadius = 1.0f;
	 distanceFromCamera = 0.0f;
	 texture = 0;
}

SceneNode ::~SceneNode(void) {
	 for (unsigned int i = 0; i < children.size(); ++i) {
		 delete children[i];
		
	}
	
}

void SceneNode::SetShader(Shader* s)
{
	std::cout << s << "\n";
	shader = s;
	std::cout << s << "\n";
}

void SceneNode::AddChild(SceneNode * s) {
	 children.push_back(s);
	 s -> parent = this;
	//hunt through all kids check that isnt a child of itself
}

 void SceneNode::RemoveChild(SceneNode* s) {
	 //auto a = std::remove(children.begin(), children.end(), s);
	 std::cout << std::to_string(children.size()) << "\n";
	 children.erase(std::remove(children.begin(), children.end(), s), children.end());
	 std::cout << std::to_string(children.size()) << "\n";
	 s->RemoveChildren();
	 delete s;
 }

 void SceneNode::RemoveChildren() {
	 for each (auto child in children)
	 {
		 child->RemoveChildren();
		 delete child;
	 }
	 children.clear();
 }


void SceneNode::Draw(OGLRenderer& r) {

	if (mesh && shader) { 

		r.BindShader(GetShader());
		r.UpdateShaderMatrices();

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"diffuseTex"), 0);

		Matrix4 model = GetWorldTransform() *
			Matrix4::Scale(GetModelScale());
		glUniformMatrix4fv(
			glGetUniformLocation(shader->GetProgram(),
				"modelMatrix"), 1, false, model.values);

		glUniform4fv(glGetUniformLocation(shader->GetProgram(),
			"nodeColour"), 1, (float*)&GetColour());

		texture = GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform1i(glGetUniformLocation(shader->GetProgram(),
			"useTexture"), texture);

		mesh->Draw(); 
	};
	
}
void SceneNode::Update(float dt) {
	if (parent) { // This node has a parent ...
		worldTransform = parent->worldTransform * transform;

	}
	else { // Root node , world transform is local transform !
		worldTransform = transform;

	}
	for (vector < SceneNode* >::iterator i = children.begin();
		i != children.end(); ++i) {
		(*i)->Update(dt);

	}
}