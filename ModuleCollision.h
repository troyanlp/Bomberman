#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;

	bool collided = false;

	Collider(SDL_Rect rectangle) :
		rect(rectangle)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;

	~Collider() {

	}
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect);
	void RemoveCollider(Collider* c);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = true;
};

#endif // __ModuleCollision_H__