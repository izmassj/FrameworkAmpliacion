#include "IntroScene.h"
#include "Sprite.h"
#include "Player.h"
#include "Parameters.h"
#include "Widget.h"
#include "GraphicsInterface.h"
#include "MyPhysics.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"

IntroScene::IntroScene(GraphicsInterface* GI, MyPhysics* mph) :Scene(GI, mph)
{
	GI->LoadImage("space.png");
	Sprite* spr = new Sprite(this, "space.png", 1000);
	spr->transform.position = Vector2(Parameters::width * 0.5f, Parameters::height * 0.5f);
	actors.push_back(spr);

	GI->LoadImage("spaceship2.png");
	Transform t;
	t.position = Vector2(Parameters::width * 0.5f, Parameters::height * 0.5f);
	Player* ply = new Player(this, "spaceship_sheet.png", t, 100, "spaceship_anim.json");
	actors.push_back(ply);

	CircleCollider* col = new CircleCollider();
	col->radius = 25;
	col->relative_position.x = 25;
	mph->AddCollider(col, ply);
	col = new CircleCollider();
	col->radius = 25;
	col->relative_position.x = -25;   
	mph->AddCollider(col, ply);
	/*
	RectangleCollider* col = new RectangleCollider();
	col->size = Vector2(25, 25);
	col->relative_position.x = 25;
	mph->AddCollider(col, ply);
	col = new RectangleCollider();
	col->size = Vector2(25, 25);
	col->relative_position.x = -25;
	mph->AddCollider(col, ply);
	*/
	GI->LoadImage("Asteroid.png");
	spr = new Sprite(this, "Asteroid.png", 150);
	spr->isStatic = true;
	//spr->mass = 10.f;
	spr->transform.position = ply->transform.position + Vector2(200, 200);
	actors.push_back(spr);

	col = new CircleCollider();
	col->radius = 75;
	mph->AddCollider(col, spr);

	spr = new Sprite(this, "Asteroid.png", 70);
	//spr->isStatic = true;
	spr->mass = 5.f;
	spr->transform.position = ply->transform.position + Vector2(-50, 200);
	actors.push_back(spr);

	RectangleCollider* rcol = new RectangleCollider();
	rcol->size = Vector2(70, 70);
	mph->AddCollider(rcol, spr);

	spr = new Sprite(this, "Asteroid.png", 250);
	spr->isStatic = true;
	spr->mass = 5.f;
	spr->transform.position = ply->transform.position + Vector2(-200, 200);
	actors.push_back(spr);

	col = new CircleCollider();
	col->radius = 100;
	mph->AddCollider(col, spr);

	Widget* wdt = new Widget(this);
	actors.push_back(wdt);
}

