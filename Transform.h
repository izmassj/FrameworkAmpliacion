#pragma once

#include "Vector2.h"

class Transform
{
public:
	Transform() {};
	Vector2 position{};
	float rotation{};
	Vector2 scale = {1.f};
};

