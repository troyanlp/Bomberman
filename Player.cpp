#include "Player.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

Player::Player(int id, bool AI, iPoint spawnPosition) : id(id), AI(AI)
{
	LOG("constructor de player!");
	type = EntityType::PLAYER;
	position = spawnPosition;
}


Player::~Player()
{
}

void Player::Draw()
{
	LOG("La posicion es: %d",collider->rect.x);
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &(collider->rect));
	int a = 0;
}

bool Player::CleanUp()
{
	if (current_animation != nullptr) RELEASE(current_animation);
	return true;
}
