#include "Animator.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

Animator::Animator()
{
}

bool Animator::Load(std::string path)
{
	std::ifstream file(path);
	if (!file.is_open())
		return false;

	json data;
	file >> data;

	spritesheetPath = data["spritesheetPath"];
	animations.clear();
	currentAnimation = "";
	currentFrame = 0;
	timer = 0.f;

	for (auto& item : data["animations"].items())
	{
		AnimationData animation;

		for (auto& frame : item.value()["frames"])
		{
			animation.frames.push_back(frame);
		}

		animation.frameRate = item.value()["frameRate"];
		animation.size = Vector2(item.value()["size"][0], item.value()["size"][1]);
		animation.startingPoint = Vector2(item.value()["startingPoint"][0], item.value()["startingPoint"][1]);
		animation.origin = Vector2(item.value()["origin"][0], item.value()["origin"][1]);

		if (item.value().contains("loop"))
		{
			animation.loop = item.value()["loop"];
		}

		animations[item.key()] = animation;

		if (currentAnimation == "")
		{
			currentAnimation = item.key();
		}
	}

	loaded = animations.size() > 0;
	return loaded;
}

void Animator::Update(float deltaTime)
{
	if (!loaded)
		return;

	if (currentAnimation == "")
		return;

	AnimationData& animation = animations[currentAnimation];
	if (animation.frames.size() == 0)
		return;

	if (animation.frameRate <= 0)
		return;

	timer += deltaTime;
	float timePerFrame = 1.f / animation.frameRate;

	if (timer >= timePerFrame)
	{
		timer = 0.f;
		currentFrame++;

		if (currentFrame >= animation.frames.size())
		{
			if (animation.loop)
				currentFrame = 0;
			else
				currentFrame = animation.frames.size() - 1;
		}
	}
}

void Animator::Play(std::string animationName)
{
	if (!HasAnimation(animationName))
		return;

	if (currentAnimation == animationName)
		return;

	currentAnimation = animationName;
	currentFrame = 0;
	timer = 0.f;
}

bool Animator::HasAnimation(std::string animationName)
{
	return animations.find(animationName) != animations.end();
}

bool Animator::HasAnimationLoaded()
{
	return loaded && currentAnimation != "";
}

Vector2 Animator::GetCurrentSize()
{
	if (!HasAnimationLoaded())
		return Vector2(0.f);

	return animations[currentAnimation].size;
}

Vector2 Animator::GetCurrentOrigin()
{
	if (!HasAnimationLoaded())
		return Vector2(0.f);

	return animations[currentAnimation].origin;
}

Vector2 Animator::GetCurrentFramePosition()
{
	if (!HasAnimationLoaded())
		return Vector2(0.f);

	AnimationData& animation = animations[currentAnimation];
	if (animation.frames.size() == 0)
		return animation.startingPoint;

	float x = animation.startingPoint.x + animation.frames[currentFrame] * animation.size.x;
	float y = animation.startingPoint.y;
	return Vector2(x, y);
}
