#pragma once
#include "Actor.h"
class Widget : public Actor
{
public:
	Widget(class Scene* myScene);

	virtual void Update(float deltaTime);
	virtual void Render();
};

