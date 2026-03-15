#pragma once
#include "Sprite.h"
class Player : public Sprite
{
public:
	Player(Scene* myscene, std::string img_name, float side_size, std::string animation_json_path = "");
	Player(Scene* myscene, std::string img_name, Transform transform, float side_size, std::string animation_json_path = "");

	virtual void Update(float deltaTime);

private:

	void UpdateAnimation();
	void UpdateMovement(float deltaTime);

	float linear_speed = 300.f;
	float ang_speed = 100.f;

};
