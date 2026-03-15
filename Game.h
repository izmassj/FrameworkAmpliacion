#pragma once

#include <map>
#include "Scene.h"
#include "Camera.h"

class Game
{
public:
	static void Create();
	static void Destroy();

	static void Play();

	static std::map<int, bool> keyDown;

	static Camera camera;
private:
	static Game* instance;
	
	static float DeltaTime;

	class GraphicsInterface* GI;
	class MyPhysics* mph;

	bool game_end = false;

	Scene* currentScene;

	Game();
	~Game();

	void Loop();
	void Update(float deltaTime);
	void Render();
	void UpdateInputs();
};

