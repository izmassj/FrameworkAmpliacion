#pragma once
#include <string>
#include <vector>
#include <map>
#include "Vector2.h"

struct AnimationData
{
	std::vector<int> frames;
	int frameRate = 0;
	Vector2 size;
	Vector2 startingPoint;
	Vector2 origin;
	bool loop = true;
};

class Animator
{
public:
	std::string spritesheetPath;
	std::map<std::string, AnimationData> animations;
	std::string currentAnimation;
	int currentFrame = 0;
	float timer = 0.f;
	bool loaded = false;

	Animator();

	bool Load(std::string path);
	void Update(float deltaTime);
	void Play(std::string animationName);
	bool HasAnimation(std::string animationName);
	bool HasAnimationLoaded();
	Vector2 GetCurrentSize();
	Vector2 GetCurrentOrigin();
	Vector2 GetCurrentFramePosition();
};
