#pragma once

#include <vector>
#include <map>
#include "Vector2.h"

#define DEBUG_COLLIDERS

typedef std::map<class Actor*, std::vector<class Collider*>*> ColliderMap;

class MyPhysics
{
	struct HIT_INFO
	{
		Vector2 col_p1, col_p2;
		float minimum_allowed_distance=0.f;
		float current_distance;
	};

	ColliderMap colliders_by_actor;

	MyPhysics();
	void Update();
	void MoveActors(class Actor * a1, class Actor* a2, Vector2 current_a1_to_a2, float minimum_distance, float current_distance);
	HIT_INFO ManageCollision(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);
	HIT_INFO IsCollidingCircleRectangle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);
	HIT_INFO IsCollidingCircleCircle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);
	HIT_INFO IsCollidingRectangleRectangle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);



public:
	void RemoveActor(class Actor*);
	void AddCollider(class Collider* col, class Actor* act);

	__forceinline ColliderMap GetCollidersByActor() const { return colliders_by_actor; }

	friend class Game;

};

