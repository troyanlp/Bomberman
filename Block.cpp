#include "Block.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"


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
}

void Block::Draw()
{
	App->renderer->Blit(graphics, NULL, NULL, &blockSprite, &spriteDest);
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

bool Block::CleanUp()
{
	return true;
}
