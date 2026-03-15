#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"

Player::Player(Scene* myscene, std::string img_name, float side_size, std::string animation_json_path):Sprite(myscene, img_name, side_size, animation_json_path)
{
	Game::camera.target = this;
}

Player::Player(Scene* myscene, std::string img_name, Transform transform, float side_size, std::string animation_json_path) :Sprite(myscene, img_name, transform, side_size, animation_json_path)
{
	Game::camera.target = this;
}

void Player::Update(float deltaTime)
{
	Sprite::Update(deltaTime);
	UpdateMovement(deltaTime);
	UpdateAnimation();
}

void Player::UpdateAnimation()
{
	if (Game::keyDown[SDLK_A] || Game::keyDown[SDLK_D] || Game::keyDown[SDLK_W])
	{
		animator.Play("move");
	}
	else 
	{
		animator.Play("idle");
	}
}

void Player::UpdateMovement(float deltaTime)
{
	Vector2 direction;

	if (Game::keyDown[SDLK_A])
		transform.rotation -= ang_speed * deltaTime;
	if (Game::keyDown[SDLK_D])
		transform.rotation += ang_speed * deltaTime;

	direction.y = -std::cos(transform.rotation *3.14159/180);
	direction.x = std::sin(transform.rotation *3.14159/180);
	if (Game::keyDown[SDLK_W])
		transform.position += direction * linear_speed * deltaTime;

}
