#pragma once

#include "Transform.h"


class Actor
{
public:
	class Scene* myScene;
	bool isStatic{};
	float mass = 1.f;

	Actor(class Scene* myScene);
	Transform transform;
	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void OnTrigger(Actor* otherActor) {};
	virtual void OnHit(Actor* otherActor) {};
};

