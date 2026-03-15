#include "MyPhysics.h"
#include "Collider.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "Actor.h"

MyPhysics::MyPhysics()
{

}

void MyPhysics::Update()
{
	for (auto it1 = colliders_by_actor.begin(); it1 != colliders_by_actor.end(); ++it1)
	{
		//¿Es el actor dinámico? (los estáticos no generan eventos)
		Actor* actor1 = it1->first;
		if (!actor1->isStatic)
			//Por cada collider que tiene el actor
			for (Collider* collider1 : *it1->second)
			{
				//Inspeccionar el resto de actores de la escena que tengan colliders
				for (auto it2 = std::next(it1); it2 != colliders_by_actor.end(); ++it2)
				{
					Actor* actor2 = it2->first;
					//Para cada uno de estos actores, inspeccionar todos sus colliders
					//Este segundo actor da igual si es estático o no, hay que inspeccionarlo,
					//ya que puede estar colisionando con el dinámico que estamos analizando
					for (Collider* collider2 : *it2->second)
					{
						HIT_INFO hit = ManageCollision(collider1, actor1, collider2, actor2);

						if (hit.minimum_allowed_distance>0.f)
						{
							if (collider1->isTrigger || collider2->isTrigger)
							{
								actor1->OnTrigger(actor2);
								actor2->OnTrigger(actor1);
							}
							else
							{
								MoveActors(actor1, actor2, hit.col_p2-hit.col_p1, hit.minimum_allowed_distance, hit.current_distance);
								actor1->OnHit(actor2);
								actor2->OnHit(actor1);
							}
						}
					}
				}
			}
	}
}

void MyPhysics::MoveActors(Actor* actor1, Actor* actor2, Vector2 current_a1_to_a2, float minimum_distance, float current_distance)
{
	float total_mass = actor1->mass + actor2->mass;
	float factor1; 
	float factor2; 
	if (actor2->isStatic)
	{
		factor1 = 1.f;
		factor2 = 0.f;
	}
	else
	{
		factor1 = actor2->mass / total_mass;
		factor2 = actor1->mass / total_mass;
	}
	float overlapping = minimum_distance - current_distance;

	actor1->transform.position -= (current_a1_to_a2 / current_distance) * overlapping * factor1;
	actor2->transform.position += (current_a1_to_a2 / current_distance) * overlapping * factor2;
}

MyPhysics::HIT_INFO MyPhysics::ManageCollision(Collider* col1, Actor* a1, Collider* col2, Actor* a2)
{
	if (dynamic_cast<CircleCollider*>(col1))
	{
		if (dynamic_cast<CircleCollider*>(col2))
			//Colision circle-circle
			return IsCollidingCircleCircle(col1, a1, col2, a2);
		else
			//Colision circle-rectangle
			return IsCollidingCircleRectangle(col1, a1, col2, a2);
	}
	else
	{
		if (dynamic_cast<CircleCollider*>(col2))
		{
			//Colision rectangle-circle
			HIT_INFO hit = IsCollidingCircleRectangle(col2, a2, col1, a1);
			if (hit.minimum_allowed_distance == 0.f)
				return hit;
			Vector2 swap = hit.col_p1;
			hit.col_p1 = hit.col_p2;
			hit.col_p2 = swap;
			return hit;
		}
		else
			//Colision rectangle-rectangle
			return IsCollidingRectangleRectangle(col1, a1, col2, a2);
	}
}

MyPhysics::HIT_INFO MyPhysics::IsCollidingCircleRectangle(Collider* col1, Actor* a1, Collider* col2, Actor* a2)
{
	return HIT_INFO();
}

MyPhysics::HIT_INFO MyPhysics::IsCollidingCircleCircle(Collider* col1, Actor* a1, Collider* col2, Actor* a2)
{
	HIT_INFO hit;
	hit.col_p1 = col1->relative_position.rotate(a1->transform.rotation) + a1->transform.position;
	hit.col_p2 = col2->relative_position.rotate(a2->transform.rotation) + a2->transform.position;
	Vector2 col1_to_col2 = hit.col_p2 - hit.col_p1;
	hit.current_distance = col1_to_col2.Module();
	hit.minimum_allowed_distance = ((CircleCollider*)col1)->radius + ((CircleCollider*)col2)->radius;

	//Minimum allowed = 0 expresa NO colision
	if (hit.minimum_allowed_distance < hit.current_distance)
		hit.minimum_allowed_distance = 0.0f;

	return hit;
}

MyPhysics::HIT_INFO MyPhysics::IsCollidingRectangleRectangle(Collider* col1, Actor* a1, Collider* col2, Actor* a2)
{
	RectangleCollider *rc1 = (RectangleCollider*)col1;
	RectangleCollider* rc2 = (RectangleCollider*)col2;

	Vector2 pmin1 = a1->transform.position + rc1->relative_position - rc1->size * 0.5f;
	Vector2 pmax1 = a1->transform.position + rc1->relative_position + rc1->size * 0.5f;
	Vector2 pmin2 = a2->transform.position + rc2->relative_position - rc2->size * 0.5f;
	Vector2 pmax2 = a2->transform.position + rc2->relative_position + rc2->size * 0.5f;

	float distX = std::max(pmax1.x, pmax2.x) - std::min(pmin1.x, pmin2.x);
	float distY = std::max(pmax1.y, pmax2.y) - std::min(pmin1.y, pmin2.y);
	bool overlapX = distX < rc1->size.x + rc2->size.x;
	bool overlapY = distY < rc1->size.y + rc2->size.y;

	HIT_INFO hit;
	if (overlapX && overlapY)
	{
		hit.col_p1 = a1->transform.position + rc1->relative_position;
		hit.col_p2 = a2->transform.position + rc2->relative_position;
		hit.current_distance = (hit.col_p1 - hit.col_p2).Module();
		Vector2 colDir = (hit.col_p1 - hit.col_p2).Normalized();
		
		float minimumDistanceX, minimumDistanceY;
		if (std::abs(colDir.x) > std::abs(colDir.y))
		{
			minimumDistanceX = (rc1->size.x + rc2->size.x) * 0.5f;
			minimumDistanceY = minimumDistanceX * colDir.y / colDir.x;
		}
		else
		{
			minimumDistanceY = (rc1->size.y + rc2->size.y) * 0.5f;
			minimumDistanceX = minimumDistanceY * colDir.x / colDir.y;
		}

		hit.minimum_allowed_distance = std::sqrt(minimumDistanceX * minimumDistanceX + minimumDistanceY * minimumDistanceY);
	}
	return hit;
}

void MyPhysics::RemoveActor(Actor* act)
{
	auto it = colliders_by_actor.find(act);
	if (it != colliders_by_actor.end())
	{
		for (Collider* collider : *it->second)
			delete collider;
		delete it->second;

		colliders_by_actor.erase(it);
	}
}


void MyPhysics::AddCollider(Collider* col, Actor* act)
{
	auto it = colliders_by_actor.find(act);
	if (it == colliders_by_actor.end())
	{
		auto pair = colliders_by_actor.insert(std::make_pair(act, new std::vector<Collider*>()));
		pair.first->second->push_back(col);
	}
	else
	{
		it->second->push_back(col);
	}

}
