#include "Bomb.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"


Bomb::Bomb(int x, int y)
{
	LOG("Loading Bomb");

	graphics = App->textures->Load("Bomberman.png");

	destroyed = false;

	// initial position of the bomb
	position.x = x;
	position.y = y;

	// bomb animation
	bomb.frames.push_back({ 177, 11, 18, 18 });
	bomb.frames.push_back({ 195, 11, 18, 18 });
	bomb.frames.push_back({ 211, 11, 18, 18 });
	bomb.speed = 0.075f;

	current_animation = &bomb;
}


Bomb::~Bomb()
{
}

void Bomb::Draw()
{
	//LOG("Pintando!!");
	SDL_Rect dest;
	dest = { position.x, position.y, 50, 50 };
	if (current_animation->GetCurrentFrame().x == bomb.frames.front().x) {
		timer++;
		if (timer == 25) current_animation->speed = 0.125;
		else if (timer == 50) current_animation->speed = 0.2;
		else if (timer >= 60) Explode();
	}
	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &dest);
}

void Bomb::Explode()
{
	LOG("BOOM");
	exploded = true;
}

bool Bomb::CleanUp()
{
	LOG("Unloading Bomb");

	App->textures->Unload(graphics);

	return true;
}
