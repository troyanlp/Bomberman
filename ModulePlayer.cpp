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
	idle.frames.push_back({ 113, 2, 17, 30 });
	idle.frames.push_back({ 133, 2, 17, 30 });
	idle.frames.push_back({ 153, 2, 17, 30 });
	idle.speed = 0.075f;
	
	// walk right animation
	walkRight.frames.push_back({ 5, 36, 17, 29 });
	walkRight.frames.push_back({ 27, 36, 17, 29 });
	walkRight.frames.push_back({ 47, 36, 17, 29 });
	walkRight.frames.push_back({ 67, 36, 17, 29 });
	walkRight.frames.push_back({ 87, 36, 17, 29 });
	walkRight.frames.push_back({ 47, 36, 17, 29 });
	walkRight.speed = 0.1f;
	
	// walk left animation
	walkLeft.frames.push_back({ 5, 99, 15, 28 });
	walkLeft.frames.push_back({ 25, 99, 15, 28 });
	walkLeft.frames.push_back({ 45, 99, 15, 28 });
	walkLeft.frames.push_back({ 65, 99, 15, 28 });
	walkLeft.frames.push_back({ 87, 99, 15, 28 });
	walkLeft.frames.push_back({ 45, 99, 15, 28 });
	walkLeft.speed = 0.1f;

	// walk up animation
	walkUp.frames.push_back({ 4, 69, 17, 26 });
	walkUp.frames.push_back({ 25, 69, 17, 26 });
	walkUp.frames.push_back({ 45, 69, 17, 26 });
	walkUp.frames.push_back({ 66, 69, 17, 26 });
	walkUp.frames.push_back({ 88, 69, 17, 26 });
	walkUp.frames.push_back({ 45, 69, 17, 26 });
	walkUp.speed = 0.1f;

	// walk down animation
	walkDown.frames.push_back({ 3, 3, 17, 29 });
	walkDown.frames.push_back({ 24, 3, 17, 29 });
	walkDown.frames.push_back({ 45, 3, 17, 29 });
	walkDown.frames.push_back({ 66, 3, 17, 29 });
	walkDown.frames.push_back({ 87, 3, 17, 29 });
	walkDown.frames.push_back({ 45, 3, 17, 29 });
	walkDown.speed = 0.1f;
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
		current_animation = &walkLeft;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		current_animation = &walkRight;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
		current_animation = &walkDown;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		current_animation = &walkUp;
		//collider->rect = { position.x, position.y, 30, 10 };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			current_animation = &idle;

	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}