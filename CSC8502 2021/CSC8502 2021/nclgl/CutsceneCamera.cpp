#include "CutsceneCamera.h"
#include "Window.h"
#include <string>


float Lerp(float a, float b, float t)
{
	t = t > 1 ? 1 : t;
	return a + (t * (b - a));
}

Vector3 Lerp(Vector3 a, Vector3 b, float t)
{
	a.x = Lerp(a.x, b.x, t);
	a.y = Lerp(a.y, b.y, t);
	a.z = Lerp(a.z, b.z, t);
	return a;
}


void CutsceneCamera::UpdateCamera(float dt)
{
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		paused = !paused;

	}
	if (!paused)
	{
		timer += dt;
		if (position == currentShot->position)
		{
			if (timer > (*currentShot).time)
			{
				currentShot++;
				if (currentShot == shots.end())
				{
					currentShot = shots.begin();
				}
				timer = 0;
			}
		}
		else
		{
			float t = timer / travelTime;
			pitch = Lerp(pitch, currentShot->pitch, t);
			yaw = Lerp(yaw, currentShot->yaw, t);
			position = Lerp(position, currentShot->position, t);
			if (position == currentShot->position)
			{
				timer = 0;
			}
		}
	}
}

void CutsceneCamera::AddShot(Shot shot)
{
	shots.emplace_back(shot);
	std::cout << std::to_string(shots.size());
}

void CutsceneCamera::Pause()
{
	paused = true;
}

void CutsceneCamera::Play()
{
	paused = false;
}

bool CutsceneCamera::isPaused()
{
	return paused;
}
