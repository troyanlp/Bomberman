#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMenu.h"
#include "SDL\include\SDL.h"

ModuleSceneMenu::ModuleSceneMenu(bool active) : Module(active)
{}

ModuleSceneMenu::~ModuleSceneMenu()
{}

// Load assets
bool ModuleSceneMenu::Start()
{
	LOG("Loading Menu");
	
	graphics = App->textures->Load("bomberman_logo.jpg");

	/*App->audio->PlayMusic("intro.ogg", 1.0f);
	if(fx == 0)
		fx = App->audio->LoadFx("starting.wav");
		*/
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 0;
	
	return true;
}

// UnLoad assets
bool ModuleSceneMenu::CleanUp()
{
	LOG("Unloading menu scene");

	App->textures->Unload(graphics);
	
	return true;
}

// Update: draw background
update_status ModuleSceneMenu::PreUpdate()
{
	//Draw background color
	App->renderer->DrawQuad(background, 136, 136, 136, 255, true);
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneMenu::Update()
{
	

	//Drar Bomberman logo
	SDL_Rect texr; 
	texr.x = 0;
	texr.y = 0;
	texr.w = SCREEN_WIDTH; 
	texr.h = SCREEN_HEIGHT;
	SDL_Rect dest = { 0, 0, 800, 400 };
	App->renderer->Blit(graphics, 0, 0, &texr, &dest);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->scene_level, this);
		App->audio->PlayFx(fx);
	}

	return UPDATE_CONTINUE;
}