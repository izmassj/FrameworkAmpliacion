#pragma once

#include <vector>

class Scene
{

	class Sprite* debugCircle256 = NULL;
	class Sprite* debugRectangle400 = NULL;

protected:
	std::vector<class Actor*> actors;
	std::vector<class Actor*> actors_to_destroy;

	struct {
		unsigned char r, g, b;
	} backgroundColor;

public:
	class GraphicsInterface* GI;
	class MyPhysics* mph;

public:
	Scene(class GraphicsInterface* GI, class MyPhysics* mph);
	~Scene();

	virtual void Update(float deltaTime);
	
	virtual void Render();
	void DestroyActor(class Actor*);
	void RemoveActors();
};

