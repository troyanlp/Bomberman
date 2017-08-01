#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Point.h"
#include "Globals.h"

struct Collider;

class Object
{
	
public:
	enum objectType {
		BLOCK,
		BRICK,
		ITEM
	};

	Object();
	Object(objectType type, int x, int y);
	~Object();

public:
	objectType type;

	iPoint position;
	bool destroyed = false;
	Collider* collider = nullptr;

};

#endif

