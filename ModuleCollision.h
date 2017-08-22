#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include <list>
#include <map>
#include "Module.h"

enum ColliderType {
	CPLAYER,
	CBOMB,
	CEXPLOTION,
	CIA,
	CEXTERNALBLOCK,
	CINTERNALBLOCK,
	CBRICK,
	CITEM,
	CNULL
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;

	bool collided = false;

	ColliderType type = CNULL;

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
	void RemoveAllColliders();
	void DebugDraw();

	bool FindCollision(const SDL_Rect rect);
	bool CheckColliderTypes(ColliderType type, ColliderType otherType);


private:

	std::map<std::pair<ColliderType, ColliderType>, bool> collisionMatrix;
	std::list<Collider*> colliders;
	bool debug = true;
};

#endif // __ModuleCollision_H__