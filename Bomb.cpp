#include "Bomb.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleBomb.h"
#include "ModuleSceneLevel.h"
#include "Explotion.h"


Bomb::Bomb(int x, int y, int id, int flame, SDL_Texture* gfx) : graphics(gfx), idPlayer(id)
{
	LOG("Loading Bomb");

	flameLevel = flame;

	destroyed = false;

	//Bomb animation
	bomb.frames.push_back({ 177, 11, 18, 18 });
	bomb.frames.push_back({ 195, 11, 18, 18 });
	bomb.frames.push_back({ 211, 11, 18, 18 });
	bomb.speed = 0.075f;

	current_animation = &bomb;

	// initial position of the bomb
	position.x = x;
	position.y = y;

	//collider
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CBOMB;

	spriteDest = { position.x, position.y, 50, 50 };
}


Bomb::~Bomb()
{
}

void Bomb::Draw()
{
	if (current_animation->GetCurrentFrame().x == bomb.frames.front().x) {
		timer++;
		if (timer == 25) current_animation->speed = 0.125f;
		else if (timer == 50) current_animation->speed = 0.2f;
		else if (timer >= 60) Explode();
	}
	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &spriteDest);
}

void Bomb::Explode()
{
	LOG("BOOM");
	list<ExplotionInstance> ex = App->scene_level->AddExplotionToMapLevel(position.x,position.y,flameLevel, idPlayer);
	exploded = true;
	collider->to_delete = true;
}

void Bomb::CleanUp()
{
	LOG("Unloading Bomb");

	App->textures->Unload(graphics);
	collider->to_delete = true;
}
