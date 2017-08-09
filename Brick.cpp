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
	/*	if (!destroyed) {
	if (external) {
	SDL_Rect dest;
	dest = { 0, 50 + (50 * i), 50, 50 };
	App->renderer->Blit(graphics, NULL, NULL, &externalBlock, &dest);
	}
	else {

	}
	}*/
}

bool Brick::CleanUp()
{
	return true;
}
