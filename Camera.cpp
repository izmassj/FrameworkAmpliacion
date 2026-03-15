#include "Camera.h"

void Camera::Update()
{
	if (target)
		position = position * 0.8f + target->transform.position * 0.2f;
}
