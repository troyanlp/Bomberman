#include "Object.h"
#include "Application.h"
#include "ModuleCollision.h"

Object::Object()
{
}

Object::Object(objectType type, int x, int y) : type(type)
{
	position.x = x;
	position.y = y;
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
}



Object::~Object()
{	
}
