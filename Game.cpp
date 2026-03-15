#include "Game.h"
#include "GraphicsInterface.h"
#include "MyPhysics.h"
#include "SDL3/SDL.h"
#include "Parameters.h"
#include <Windows.h>
#include "IntroScene.h"

Game* Game::instance = 0;
std::map<int, bool> Game::keyDown;
float Game::DeltaTime = 0.0f;
Camera Game::camera(Vector2(0.f));

void Game::Create()
{
	if (!instance)
		instance = new Game();

	Game::camera.position = Parameters::initial_camera_position;
}

void Game::Destroy()
{
	if (instance)
		delete instance;
}

void Game::Play()
{
	if (instance)
		instance->Loop();
}

Game::Game()
{
	GI = new GraphicsInterface();
	mph = new MyPhysics();

	currentScene = new IntroScene(GI, mph);

}

Game::~Game()
{
	delete GI;
	delete mph;
}

void Game::UpdateInputs()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			game_end = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			keyDown[event.key.key] = true;
			if (event.key.key == SDLK_SPACE && event.key.repeat == 0)
			{
				keyDown[SDLK_SPACE] = true;
			}
			else
				keyDown[SDLK_SPACE] = false;
			break;
		case SDL_EVENT_KEY_UP:
			keyDown[event.key.key] = false;
			break;
		}

		const	bool* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
		{
			game_end = true;
		}
	}
}

void Game::Update(float deltaTime)
{
	UpdateInputs();
	currentScene->Update(deltaTime);
	Game::camera.Update();
}

void Game::Render()
{
	currentScene->Render();
}

void Game::Loop()
{
	unsigned int millis = SDL_GetTicks();
	float desired_deltatime = 1.0f / Parameters::desired_FPS;
	while (!game_end)
	{
		Update(DeltaTime);

		mph->Update();

		currentScene->RemoveActors();

		Render();

		DeltaTime = (SDL_GetTicks() - millis)/1000.f;
		millis = SDL_GetTicks();

		Sleep(int(abs(desired_deltatime - DeltaTime)/1000));
	}
}
