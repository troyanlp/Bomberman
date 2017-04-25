#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMenu.h"

ModuleSceneMenu::ModuleSceneMenu(bool active) : Module(active)
{}

ModuleSceneMenu::~ModuleSceneMenu()
{}

// Load assets
bool ModuleSceneMenu::Start()
{
	LOG("Loading Menu");
	
	background = App->textures->Load("bomberman_logo.jpg");

	/*App->audio->PlayMusic("intro.ogg", 1.0f);
	if(fx == 0)
		fx = App->audio->LoadFx("starting.wav");
		*/
	App->renderer->camera.x = App->renderer->camera.y = 0;
	
	return true;
}

// UnLoad assets
bool ModuleSceneMenu::CleanUp()
{
	LOG("Unloading menu scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
update_status ModuleSceneMenu::Update()
{
	SDL_Rect texr; 
	texr.x = 0;
	texr.y = 0;
	texr.w = SCREEN_WIDTH; 
	texr.h = SCREEN_HEIGHT;
	App->renderer->Blit(background, 0, 0, NULL);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->scene_level, this);
		App->audio->PlayFx(fx);
	}

	return UPDATE_CONTINUE;
}