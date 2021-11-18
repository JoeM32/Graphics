#pragma once
#include "Camera.h"
class PlayerCamera :
	public Camera
{
public:
	PlayerCamera(void) {
		yaw = 0.0f;
		pitch = 0.0f;

	};

	PlayerCamera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;

	}
	void UpdateCamera(float dt = 1.0f);
protected:
};

