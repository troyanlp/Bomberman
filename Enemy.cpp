#include "Enemy.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "MemLeaks.h"
#include "ModuleInput.h"

Enemy::Enemy(iPoint pos, EnemyDirection dir) : direction(dir)
{
	LOG("Loading enemy");

	position = pos;

	type = ENEMY;

	graphics = App->textures->Load("Enemies and items.gif");

	// down animation
	down.frames.push_back({ 2, 2, 15, 23 });
	down.frames.push_back({ 20, 2, 15, 23 });
	down.frames.push_back({ 38, 2, 15, 23 });
	down.frames.push_back({ 56, 2, 15, 23 });
	down.speed = 0.075f;

	// up animation
	up.frames.push_back({ 74, 2, 15, 23 });
	up.frames.push_back({ 92, 2, 15, 23 });
	up.frames.push_back({ 110, 2, 15, 23 });
	up.frames.push_back({ 128, 2, 15, 23 });
	up.speed = 0.075f;

	// side animation
	side.frames.push_back({ 146, 2, 15, 23 });
	side.frames.push_back({ 164, 2, 15, 23 });
	side.frames.push_back({ 182, 2, 15, 23 });
	side.frames.push_back({ 200, 2, 15, 23 });
	side.speed = 0.075f;

	switch (dir) {
	case VERTICALUP:
		current_animation = &up;
		break;
	case VERTICALDOWN:
		current_animation = &down;
		break;
	case HORIZONTALRIGHT:
		current_animation = &side;
		flipType = SDL_FLIP_HORIZONTAL;
		break;
	case HORIZONTALLEFT:
		current_animation = &side;
		flipType = SDL_FLIP_NONE;
		break;
	}
	
	//collider
	collider = App->collision->AddCollider({ position.x, position.y, 30, 40 });
	collider->type = CENEMY;

	spriteDest = { position.x, position.y, 30, 40 };
}


Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (collider->collided) {
		Die();
	}

	switch (direction) {
	case VERTICALUP:
		if (CanMove(0, -enemySpeed)) {
			position.y -= enemySpeed;
		}
		else {
			ChangeDirection(VERTICALDOWN);
		}
		break;
	case VERTICALDOWN:
		if (CanMove(0, enemySpeed)) {
			position.y += enemySpeed;
		}
		else {
			ChangeDirection(VERTICALUP);
		}
		break;
	case HORIZONTALRIGHT:
		if (CanMove(enemySpeed, 0)) {
			position.x += enemySpeed;
		}
		else {
			ChangeDirection(HORIZONTALLEFT);
		}
		break;
	case HORIZONTALLEFT:
		if (CanMove(-enemySpeed, 0)) {
			position.x -= enemySpeed;
		}
		else {
			ChangeDirection(HORIZONTALRIGHT);
		}
		break;
	}
	collider->rect = { position.x,position.y,30,40 };
	spriteDest = { position.x, position.y, 30, 40 };
}

void Enemy::Draw()
{
	if (!destroyed)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &spriteDest,0,flipType);
}

void Enemy::CleanUp()
{
	collider->to_delete = true;
	App->textures->Unload(graphics);
	if (current_animation != nullptr) {
		//RELEASE(current_animation);
	}
}

void Enemy::ChangeDirection(EnemyDirection dir)
{
	direction = dir;
	switch (direction) {
	case VERTICALUP:
		current_animation = &up;
		break;
	case VERTICALDOWN:
		current_animation = &down;
		break;
	case HORIZONTALRIGHT:
		current_animation = &side;
		flipType = SDL_FLIP_HORIZONTAL;
		break;
	case HORIZONTALLEFT:
		current_animation = &side;
		flipType = SDL_FLIP_NONE;
		break;
	}
}

bool Enemy::CanMove(int x, int y)
{
	SDL_Rect newPos = { position.x + x, position.y + y, 30, 40 };
	//LOG("El new pos es: %d, %d, %d, %d", newPos.x, newPos.y, newPos.w, newPos.h);
	if (App->collision->FindCollision(newPos, ColliderType::CBOMB)) {
		return false;
	}
	else {
		return true;
	}
}

void Enemy::Die()
{
	collider->to_delete = true;
	App->textures->Unload(graphics);
	if (current_animation != nullptr) {
		//RELEASE(current_animation);
	}
}
