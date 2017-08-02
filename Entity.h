#ifndef __Entity_H__
#define __Entity_H__

#include "Point.h"
#include "Globals.h"

struct Collider;

class Entity
{
	
public:
	enum EntityType {
		PLAYER,
		BOMB,
		IA,
		BLOCK,
		BRICK,
		ITEM
	};

	Entity();
	Entity(EntityType type, int x, int y);
	~Entity();

	virtual void Draw();

public:
	EntityType type;
	iPoint position;
	bool destroyed = false;
	Collider* collider = nullptr;

};

#endif

