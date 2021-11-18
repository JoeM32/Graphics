#include "PlayerCamera.h"
#include "Window.h"
#include <algorithm>

void PlayerCamera::UpdateCamera(float dt) {
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;

	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;


	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	float relativeSpeed = speed * dt;
		position += forward * relativeSpeed;

	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= forward * relativeSpeed;

	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position -= right * relativeSpeed;

	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position += right * relativeSpeed;

		position.y += relativeSpeed;

	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= relativeSpeed;

	}
	/*if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
		roll -= 30.0f * dt;

	}
		roll += 30.0f * dt;
	}*/
