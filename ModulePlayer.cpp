#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	// initial position of the player
	position.x = 100;
	position.y = 100;
	// idle animation
	idle.frames.push_back({ 66, 1, 50, 50 });

	// walk right animation


	// walk left animation


	// walk up animation


	// walk down animation

}

ModulePlayer::~ModulePlayer()
{
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Bomberman.png");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::Update()
{
	// Player movement
	int speed = 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;

		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;

		//collider->rect = { position.x, position.y, 30, 10 };
	}

	current_animation = &idle;
	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}