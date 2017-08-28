#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"
#include "Player.h"
#include "ModuleGUI.h"

using namespace std;

ModuleCollision::ModuleCollision()
{
	//Fill collision matrix
	collisionMatrix[make_pair(CPLAYER, CEXTERNALBLOCK)] = true;
	collisionMatrix[make_pair(CPLAYER, CINTERNALBLOCK)] = true;
	collisionMatrix[make_pair(CPLAYER, CBRICK)] = true;
	collisionMatrix[make_pair(CPLAYER, CITEM)] = true;

	collisionMatrix[make_pair(CBOMB, CEXTERNALBLOCK)] = true;
	collisionMatrix[make_pair(CBOMB, CINTERNALBLOCK)] = true;
	collisionMatrix[make_pair(CBOMB, CBRICK)] = true;

	//collisionMatrix[make_pair(CEXPLOTION, CPLAYER)] = true;
	//collisionMatrix[make_pair(CEXPLOTION, CBRICK)] = true;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		//Check if player or enemy collide with and explotion
		if ((*it)->type == CPLAYER || (*it)->type == CENEMY) {
			for (std::list<Collider*>::iterator it2 = std::next(it, 1); it2 != colliders.end(); ++it2) {
				if ((*it2)->type == CEXPLOTION1 || (*it2)->type == CEXPLOTION2) {
					if ((*it)->CheckCollision((*it2)->rect)) {
						//LOG("COLISION DE PLAYER Y %d", (*it2)->type);
						//App->player->player1->Hurt();
						if ((*it2)->type == CEXPLOTION1 && (*it)->type == CENEMY && !(*it)->collided) App->gui->ChangePlayerPoints(1000, 1);
						else if ((*it2)->type == CEXPLOTION2 && (*it)->type == CENEMY && !(*it)->collided) App->gui->ChangePlayerPoints(1000, 2);
						(*it)->collided = true;
						(*it)->message = HURT;
						
						
					}
				}
			}
		}

		//Check if player is hurt colliding with an enemy
		if ((*it)->type == CPLAYER) {
			for (std::list<Collider*>::iterator it2 = std::next(it, 1); it2 != colliders.end(); ++it2) {
				if ((*it2)->type == CENEMY) {
					if ((*it)->CheckCollision((*it2)->rect)) {
						(*it)->collided = true;
						(*it)->message = HURT;
					}
				}
			}
		}

		//Check if player is hurt colliding with an item
		if ((*it)->type == CPLAYER) {
			for (std::list<Collider*>::iterator it2 = std::next(it, 1); it2 != colliders.end(); ++it2) {
				if ((*it2)->type == CITEM) {
					if ((*it)->CheckCollision((*it2)->rect)) {
						(*it)->collided = true;
						(*it2)->collided = true;
						(*it)->message = (*it2)->message;
						(*it2)->message = NOTHING;
					}
				}
			}
		}
		
	}


	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

bool ModuleCollision::FindCollision(const SDL_Rect rect, ColliderType extra1, ColliderType extra2)
{
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		if ((*it)->type == CEXTERNALBLOCK || (*it)->type == CINTERNALBLOCK || (*it)->type == CBRICK
			|| (*it)->type == extra1 || (*it)->type == extra2) {
			double d = sqrt(pow((*it)->rect.x - rect.x, 2) + pow((*it)->rect.y - rect.y, 2));
			if (d < 100) {
				if ((*it)->CheckCollision(rect)) {
					LOG("El tipo de la colision es: %d", (*it)->type);
					return true;
				}
			}
		}
	}
	return false;
}

bool ModuleCollision::CheckColliderTypes(ColliderType type, ColliderType otherType)
{
	return (collisionMatrix[make_pair(type, otherType)] || collisionMatrix[make_pair(otherType, type)]);
}


// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect)
{
	Collider* ret = new Collider(rect);

	colliders.push_back(ret);

	return ret;
}

void ModuleCollision::RemoveCollider(Collider* c)
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		if (*it == c) {
			RELEASE(*it);
		}
	}
}

void ModuleCollision::RemoveAllColliders()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	//Calculate when is NOT colliding
	if (rect.x < r.x && (rect.x + rect.w) < r.x) return false;
	if (rect.x > (r.x + r.w) && (rect.x + rect.w) > (r.x + r.w)) return false;
	if (rect.y < r.y && (rect.y + rect.h) < r.y) return false;
	if (rect.y > (r.y + r.h) && (rect.y + rect.h) > (r.y + r.h)) return false;

	return true;
}
