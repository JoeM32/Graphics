#include "CubeRobot.h"

CubeRobot::CubeRobot(Mesh* cube) {
	// SetMesh ( cube ); // Uncomment if you want a local origin marker !

	SceneNode* body = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);

	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);



	hips = new SceneNode(cube, Vector4(0, 1, 1, 1)); // Blue !
	hips->SetModelScale(Vector3(12, 5, 6));
	hips->SetTransform(Matrix4::Translation(Vector3(0, -10, 0)));
	body->AddChild(hips);



	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	hips->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	hips->AddChild(rightLeg);

	body->RemoveChild(hips);

}void CubeRobot::Update(float dt) {
	transform = transform *
		Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));

	head->SetTransform(head -> GetTransform() *
		Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));	leftArm->SetTransform(leftArm -> GetTransform() *
		Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));

	rightArm->SetTransform(rightArm->GetTransform() *
		Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));

	SceneNode::Update(dt);

}

