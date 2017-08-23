#include "Enemy.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"


Enemy::Enemy(int x, int y)
{
	LOG("Loading player");

	graphics = App->textures->Load("Enemies and items.gif");

	//collider
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CBOMB;

	spriteDest = { position.x, position.y, 50, 50 };
}


Enemy::~Enemy()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

bool Enemy::CleanUp()
{
	return false;
}
