#pragma once
#include "Vector2.h"

class Collider
{
public:
	bool isTrigger{};
	Vector2 relative_position{};

	virtual ~Collider() {};
};

