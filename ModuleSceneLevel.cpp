#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleSceneLevel.h"

ModuleSceneLevel::ModuleSceneLevel(bool active) : Module(active)
{}

ModuleSceneLevel::~ModuleSceneLevel()
{}

// Load assets
bool ModuleSceneLevel::Start()
{
	LOG("Loading space scene");
	
	graphics = App->textures->Load("Super Bomberman 5 JPN - Stage 01.png");

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 50;
	//Set up hud rect
	hud.h = 50;
	hud.w = SCREEN_WIDTH;
	hud.x = 0;
	hud.y = 0;

	//Set up external block
	externalBlock.x = 9;
	externalBlock.y = 20;
	externalBlock.w = 15;
	externalBlock.h = 13;

	//Set up block
	block.x = 146;
	block.y = 240;
	block.w = 14;
	block.h = 15;

	//Set up brick
	brick.x = 78;
	brick.y = 240;
	brick.w = 15;
	brick.h = 15;

	App->player->Enable();
	//App->particles->Enable();
	//App->collision->Enable();

	//App->audio->PlayMusic("stage1.ogg", 1.0f);
	
	//create some colliders for the walls

	

	return true;
}

// UnLoad assets
bool ModuleSceneLevel::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(graphics);
	App->player->Disable();
	//App->collision->Disable();
	//App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneLevel::PreUpdate()
{
	//Draw hud background color
	App->renderer->DrawQuad(hud, 136, 136, 136, 255, true);
	//Draw background floor color
	App->renderer->DrawQuad(background, 64, 120, 16, 255, true);
	return UPDATE_CONTINUE;
}

update_status ModuleSceneLevel::Update()
{
	// Move camera forward -----------------------------
	//int scroll_speed = 1;

	//App->player->position.x += 1;
	//App->player->collider->SetPos(App->player->position.x, App->player->position.y);
	//App->renderer->camera.x -= 3;
	
	// Draw everything --------------------------------------
	//App->renderer->Blit(graphics, 0, 0, NULL, NULL);
	DrawExternalBlocks();
	DrawBlocks();
	DrawBricks();
	
	return UPDATE_CONTINUE;
}

void ModuleSceneLevel::DrawExternalBlocks()
{
	SDL_Rect dest;
	//Left
	for (int i = 0; i < 11; i++) {
		dest = { 0, 50+(50*i), 50, 50 };
		App->renderer->Blit(graphics, NULL, NULL, &externalBlock, &dest);
	}
	//Up
	for (int i = 1; i <= 14; i++) {
		dest = { 0+(50*i), 50, 50, 50 };
		App->renderer->Blit(graphics, NULL, NULL, &externalBlock, &dest);
	}
	//Right
	for (int i = 1; i <= 10; i++) {
		dest = { 700, 50 + (50 * i), 50, 50 };
		App->renderer->Blit(graphics, NULL, NULL, &externalBlock, &dest);
	}
	//Down
	for (int i = 1; i <= 13; i++) {
		dest = { 0 + (50 * i), 550, 50, 50 };
		App->renderer->Blit(graphics, NULL, NULL, &externalBlock, &dest);
	}
}

void ModuleSceneLevel::DrawBlocks()
{
	SDL_Rect dest;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			dest = { 100 + (100*j), 150 + (100 * i), 50, 50 };
			App->renderer->Blit(graphics, NULL, NULL, &block, &dest);
		}
	}
}

void ModuleSceneLevel::DrawBricks()
{
	SDL_Rect dest;
	dest = { 150, 100, 50, 50 };
	App->renderer->Blit(graphics, NULL, NULL, &brick, &dest);
	dest = { 200, 100, 50, 50 };
	App->renderer->Blit(graphics, NULL, NULL, &brick, &dest);
}
