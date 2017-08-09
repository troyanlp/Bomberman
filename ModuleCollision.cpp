#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision()
{
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
	//LOG("El size es: %d", colliders.size());
	/*for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		Collider* c1 = *it;
		for (list<Collider*>::iterator jt = colliders.begin(); jt != colliders.end();)
		{
			Collider* c2 = *jt;
			if (c1 != c2) {
				if (c1->CheckCollision(c2->rect)) {
					LOG("BOOM!!!");
					//c1->to_delete = true;
				}
			}
		}
	}*/


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

bool ModuleCollision::FindCollision(const SDL_Rect rect)
{
	return false;
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

void ModuleCollision::RemoveCollider(Collider * c)
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();
}

void ModuleCollision::RemoveAllColliders()
{
}

/*void ModuleCollision::RemoveCollider(Collider* c)
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		if (*it == c) {
		RELEASE(*it);
		}
	}
}*/

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (rect.x >= r.x && rect.x <= (rect.x + rect.w)) {
		if (rect.y >= r.y && rect.y <= (rect.y + rect.h)) {
			return true;
		}
	}
	return false;
}
