#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "SDL/include/SDL.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{

	// idle animation
	idleDown.frames.push_back({ 113, 2, 17, 30 });
	idleDown.frames.push_back({ 133, 2, 17, 30 });
	idleDown.frames.push_back({ 153, 2, 17, 30 });
	idleDown.speed = 0.075f;
	
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

	// celebrate animation
	celebrate.frames.push_back({ 177, 98, 15, 29 });
	celebrate.frames.push_back({ 195, 98, 21, 29 });
	celebrate.frames.push_back({ 219, 98, 19, 29 });
	celebrate.speed = 0.1f;

	// die animation
	die.frames.push_back({ 1, 135, 17, 30 });
	die.frames.push_back({ 22, 135, 17, 30 });
	die.frames.push_back({ 44, 135, 21, 30 });
	die.frames.push_back({ 69, 135, 19, 30 });
	die.frames.push_back({ 93, 135, 21, 30 });
	die.frames.push_back({ 118, 135, 17, 30 });
	die.frames.push_back({ 139, 135, 26, 30 });
	die.frames.push_back({ 167, 135, 20, 30 });
	die.frames.push_back({ 191, 135, 17, 30 });
	die.frames.push_back({ 211, 135, 19, 30 });
	die.speed = 0.1f;
	die.loop = false;

}

ModulePlayer::~ModulePlayer()
{
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Bomberman.png");

	destroyed = false;

	// initial position of the player
	position.x = 100;
	position.y = 100;

	//Set colliider
	collider = App->collision->AddCollider({ position.x, position.y, 17, 30 });

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
		collider->rect = { position.x, position.y, 15, 28 };
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		current_animation = &walkRight;
		collider->rect = { position.x, position.y, 17, 29 };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
		current_animation = &walkDown;
		collider->rect = { position.x, position.y, 15, 28 };
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		current_animation = &walkUp;
		collider->rect = { position.x, position.y, 17, 26 };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			current_animation = &idleDown;

	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),&(collider->rect));


	return UPDATE_CONTINUE;
}