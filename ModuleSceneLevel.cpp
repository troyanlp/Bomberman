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
	
	graphics = App->textures->Load("background.png");

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 100;
	//Set up hud rect
	hud.h = 100;
	hud.w = SCREEN_WIDTH;
	hud.x = 0;
	hud.y = 0;

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

// Update: draw background
update_status ModuleSceneLevel::Update()
{
	// Move camera forward -----------------------------
	//int scroll_speed = 1;

	//App->player->position.x += 1;
	//App->player->collider->SetPos(App->player->position.x, App->player->position.y);
	//App->renderer->camera.x -= 3;
	
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, 0, 0, NULL, NULL);
	
	return UPDATE_CONTINUE;
}