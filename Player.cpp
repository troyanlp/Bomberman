#include "Player.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

Player::Player(int id, bool AI, SDL_Texture* gfx, iPoint spawnPosition) : id(id), AI(AI), graphics(gfx)
{
	LOG("constructor de player!");
	type = EntityType::PLAYER;
	position = spawnPosition;
	collider = App->collision->AddCollider({ position.x, position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT });
}


Player::~Player()
{
}

void Player::Draw()
{
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &(collider->rect));
	int a = 0;
}

bool Player::CleanUp()
{
	//if (current_animation != nullptr) RELEASE(current_animation);
	return true;
}
