#include "SceneNode.h"
#include <iostream>
#include <algorithm>
#include <vector>

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) {
	 this -> mesh = mesh;
	 this -> colour = colour;
	 parent = NULL;
	 this->shader = NULL;
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


void SceneNode::Draw(const OGLRenderer& r) {

	if (mesh) { mesh->Draw(); };
	
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