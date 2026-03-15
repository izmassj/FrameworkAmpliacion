#pragma once
#include "Transform.h"
#include "Actor.h"

class Camera
{
public:
	Camera(Vector2 initial_camera_position) :position(initial_camera_position) {};

	void Update();

	Vector2 position;
	Actor *target = NULL;
};

