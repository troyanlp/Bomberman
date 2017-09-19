#include "TrapSquare.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"


TrapSquare::TrapSquare(iPoint spawnPosition)
{
	type = EntityType::TRAP;

	position = spawnPosition;

	graphics = App->textures->Load("Sprites/tiles.png");

	squareSprite = { 52, 631, 15, 15 };

	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CTRAP;

	spriteDest = { position.x, position.y, 50, 50 };
}


TrapSquare::~TrapSquare()
{
}

void TrapSquare::Draw()
{
	App->renderer->Blit(graphics, NULL, NULL, &squareSprite, &spriteDest);
}

void TrapSquare::CleanUp()
{
}
