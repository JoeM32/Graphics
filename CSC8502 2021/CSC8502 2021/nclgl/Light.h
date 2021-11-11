#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	Light() {} // Default constructor , we ’ll be needing this later !
	Light(const Vector3& position, const Vector3& direction,  const Vector4& colour, const Vector4& colourSpecular, float radius, float angle) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;
		this->colourSpecular = colourSpecular;
		this->direction = direction;
		this->angle = angle;

	}

	Light(const Vector3& position, const Vector4& colour, float radius) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;

	}
	~Light(void) {};
	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& val) { position = val; }

	Vector3 GetDirection() const { return direction; }
	void SetDirection(const Vector3& val) { direction = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	float GetAngle() const { return angle; }
	void SetAngle(float val) { angle = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }

	Vector4 GetColourSpecular() const { return colourSpecular; }
	void SetColourSpecular(const Vector4& val) { colourSpecular = val; }

protected:
	Vector3 position;
	Vector3 direction;
	float radius;
	float angle;
	Vector4 colour;
	Vector4 colourSpecular;

};
