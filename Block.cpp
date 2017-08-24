#include "Block.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"


Block::Block(iPoint spawnPosition, bool ext, SDL_Texture* gfx, SDL_Rect block) : external(ext), blockSprite(block), graphics(gfx)
{
	type = EntityType::BLOCK;
	position = spawnPosition;
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	if (ext) collider->type = CEXTERNALBLOCK;
	else collider->type = CINTERNALBLOCK;
	spriteDest = { position.x, position.y, 50, 50 };
}


Block::~Block()
{
	//App->textures->Unload(graphics);
}

void Block::Draw()
{
	App->renderer->Blit(graphics, NULL, NULL, &blockSprite, &spriteDest);
}

void Block::CleanUp()
{
}
