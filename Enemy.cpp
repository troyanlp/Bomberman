#include "Enemy.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "MemLeaks.h"
#include "ModuleInput.h"

Enemy::Enemy(iPoint pos, char direction) 
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

	switch (direction) {
	case 'u':
		current_animation = &up;
		goingDown = false;
		break;
	case 'd':
		current_animation = &down;
		goingDown = true;
		break;
	case 'r':
		current_animation = &side;
		goingRight = true;
		break;
	case 'l':
		current_animation = &side;
		goingRight = false;
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

}

void Enemy::Draw()
{
	//Logic

	//Draw
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

void Enemy::Die()
{
	collider->to_delete = true;
	App->textures->Unload(graphics);
	if (current_animation != nullptr) {
		//RELEASE(current_animation);
	}
}
