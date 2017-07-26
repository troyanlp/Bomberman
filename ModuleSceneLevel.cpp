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
	
	background = App->textures->Load("background.png");

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

 	App->textures->Unload(background);
	App->player->Disable();
	//App->collision->Disable();
	//App->particles->Disable();
	
	return true;
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
	App->renderer->Blit(background, 0, 0, NULL, NULL);
	
	return UPDATE_CONTINUE;
}