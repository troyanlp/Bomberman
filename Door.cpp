#include "Door.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleSceneLevel.h"
#include "ModuleGUI.h"

Door::Door(iPoint pos)
{
	type = EntityType::DOOR;
	position = pos;

	graphics = App->textures->Load("door.png");
	doorSprite = { 0, 0, 150, 150 };
	spriteDest = { position.x - 10, position.y - 10, 50, 50 };

	collider = App->collision->AddCollider({ position.x, position.y, 30, 30 });
	collider->type = CDOOR;
}


Door::~Door()
{
}

void Door::Draw()
{
	if (!destroyed && collider->collided && collider->message == ColliderMessage::HURT) {
		if (CheckFinishedLevel()) {
			CleanUp();
			App->scene_level->GoToResult(true, App->gui->player1Points);
		}
		collider->collided = false;
		collider->message = NOTHING;
	}

	if (!destroyed)
		App->renderer->Blit(graphics, position.x, position.y, &doorSprite, &spriteDest);
}

void Door::CleanUp()
{
	LOG("JORNADA DE PUERTAS ABIERTAS!");
	destroyed = true;
	collider->to_delete = true;
}

bool Door::CheckFinishedLevel()
{
	int numEnemies = App->scene_level->CheckEnemiesLeft();
	if (numEnemies == 0) return true;
	else return false;
}
