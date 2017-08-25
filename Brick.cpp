#include "Brick.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"


Brick::Brick(iPoint spawnPosition, SDL_Texture* gfx, SDL_Rect brick) : brickSprite(brick), graphics(gfx)
{
	LOG("Nuevo ladrillo!")
	type = EntityType::BRICK;
	position = spawnPosition;
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CBRICK;
	spriteDest = { position.x, position.y, 50, 50 };
}


Brick::~Brick()
{
}

void Brick::Draw()
{
	App->renderer->Blit(graphics, NULL, NULL, &brickSprite, &spriteDest);
}

Item* Brick::Loot()
{
	if (hasLoot) {
		looted = true;
		return loot;
	}
	else {
		return nullptr;
	}
}

void Brick::AddLoot(ItemType itemType)
{
	hasLoot = true;
	loot = new Item(position, itemType);
}

void Brick::CleanUp()
{
	collider->to_delete = true;
	if (hasLoot && !looted) RELEASE(loot);
}
