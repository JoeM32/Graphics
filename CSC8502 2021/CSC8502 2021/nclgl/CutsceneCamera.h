#pragma once
#include "Camera.h"
#include <list>

struct Shot
{
	float yaw;
	float pitch;
	Vector3 position;
	float time;
};

class CutsceneCamera :
	public Camera
{
public:


	CutsceneCamera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		Shot shot = { 0,0,Vector3(0,0,0), 0 };
		shots.emplace_back(shot);
		currentShot = shots.begin();
		paused = true;
	};

	CutsceneCamera(float pitch, float yaw, Vector3 position, float time = 0) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
		Shot shot = {yaw,pitch,position, time};
		shots.emplace_back(shot);
		currentShot = shots.begin();
		paused = true;
	}
	void UpdateCamera(float dt = 1.0f);
	void AddShot(Shot shot);
	void Pause();
	void Play();
	bool isPaused();
protected:
	std::list<Shot> shots;
	std::list<Shot>::iterator currentShot;
	bool paused;
	float timer = 0;
	float travelTime = 5;
};


