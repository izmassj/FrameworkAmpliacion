#pragma once
#include "Actor.h"
#include "Vector2.h"
#include "Animator.h"
#include <iostream>

class Sprite :public Actor
{
protected:
	std::string image_name;
	Vector2 size;

public:
	Animator animator;

	Sprite(Scene *myscene, std::string img_name, Vector2 size, std::string animation_json_path = "");
	Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size, std::string animation_json_path = "");

	virtual void Update(float deltaTime);
	virtual void Render();
};
