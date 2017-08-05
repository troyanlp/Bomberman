#include "Entity.h"
#include "Application.h"
#include "ModuleCollision.h"

Entity::Entity()
{
}

Entity::Entity(EntityType type, int x, int y) : type(type)
{
	position.x = x;
	position.y = y;
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	//LOG("constructor de entity!");
}



Entity::~Entity()
{	
}

void Entity::Draw()
{
}
