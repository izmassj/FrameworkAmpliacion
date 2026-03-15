#include "Scene.h"
#include "Actor.h"

#include "GraphicsInterface.h"
#include "MyPhysics.h"

#ifdef DEBUG_COLLIDERS
#include "Sprite.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#endif

Scene::Scene(GraphicsInterface* GI, MyPhysics * mph)
{
	backgroundColor.b = backgroundColor.g = backgroundColor.r = 0;
	this->GI = GI;
	this->mph = mph;

#ifdef DEBUG_COLLIDERS
	GI->LoadImage("circle_PNG256_256.png");
	debugCircle256 = new Sprite(this, "circle_PNG256_256.png", 1);
	GI->LoadImage("square_PNG400_400.png");
	debugRectangle400 = new Sprite(this, "square_PNG400_400.png", 1);
#endif // DEBUG_COLLIDERS

}

Scene::~Scene()
{
	for (Actor* actor : actors)
	{
		mph->RemoveActor(actor);

		delete actor;
	}
}

void Scene::Update(float deltaTime)
{
	for (Actor* actor : actors)
	{
		actor->Update(deltaTime);
	}

}

void Scene::Render()
{
	GI->ClearScreen(backgroundColor.r, backgroundColor.g, backgroundColor.b);

	for (Actor* actor : actors)
	{
		actor->Render();
	}


#ifdef DEBUG_COLLIDERS

	for (auto pair : mph->GetCollidersByActor())
	{
		Actor* act = pair.first;
		for (Collider* col : *pair.second)
		{
			Transform t;
			if (dynamic_cast<CircleCollider*>(col))
			{
				t.position = act->transform.position + col->relative_position.rotate(act->transform.rotation);
				t.scale = ((CircleCollider*)col)->radius * 2;
				debugCircle256->transform = t;
				debugCircle256->Render();
			}
			else
			{
				t.position = act->transform.position + col->relative_position;
				t.scale = ((RectangleCollider*)col)->size;
				debugRectangle400->transform = t;
				debugRectangle400->Render();
			}
		}
	}

#endif
	GI->DrawFrame();
}

void Scene::DestroyActor(Actor* act)
{
	actors_to_destroy.push_back(act);
}

void Scene::RemoveActors()
{
	for (Actor* act : actors_to_destroy)
	{
		auto it = std::find(actors.begin(), actors.end(), act);
		if (it != actors.end())
			actors.erase(it);

		mph->RemoveActor(act);

		delete act;
	}

	actors_to_destroy.clear();
}
