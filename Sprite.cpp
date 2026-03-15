#include "Sprite.h"
#include "Scene.h"
#include "GraphicsInterface.h"

Sprite::Sprite(Scene* myscene, std::string img_name, Vector2 size, std::string animation_json_path):Actor(myscene)
{
	image_name = img_name;
	this->size = size;

	if (animation_json_path != "")
	{
		if (animator.Load(animation_json_path))
		{
			image_name = animator.spritesheetPath;
			myScene->GI->LoadImage(image_name);
			this->size = animator.GetCurrentSize();
		}
	}
}

Sprite::Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size, std::string animation_json_path):Actor(myscene)
{
	image_name = img_name;
	this->transform = transform;
	this->size = size;

	if (animation_json_path != "")
	{
		if (animator.Load(animation_json_path))
		{
			image_name = animator.spritesheetPath;
			myScene->GI->LoadImage(image_name);
			this->size = animator.GetCurrentSize();
		}
	}
}

void Sprite::Update(float deltaTime)
{
	animator.Update(deltaTime);
	if (animator.HasAnimationLoaded())
	{
		size = animator.GetCurrentSize();
	}
}

void Sprite::Render()
{
	if (animator.HasAnimationLoaded())
	{
		myScene->GI->DrawSpritePart(image_name, transform, animator.GetCurrentFramePosition(), animator.GetCurrentSize(), animator.GetCurrentOrigin());
	}
	else
	{
		myScene->GI->DrawSprite(image_name, transform, size);
	}
}
