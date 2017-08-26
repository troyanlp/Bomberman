#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMenu.h"
#include "ModuleWindow.h"
#include "SDL\include\SDL.h"

ModuleSceneMenu::ModuleSceneMenu(bool active) : Module(active)
{}

ModuleSceneMenu::~ModuleSceneMenu()
{}

// Load assets
bool ModuleSceneMenu::Start()
{
	LOG("Loading Menu");
	
	graphics = App->textures->Load("bomberman_logo.png");

	//Logo
	logo = { 0, 0, 720, 400 };
	position = iPoint(25, 25);
	logoDest = { position.x, position.y, 675, 325 };

	//Text
	color = { 255, 255, 255 };
	font = TTF_OpenFont("emulogic.ttf", 12);
	
	textSurface = TTF_RenderText_Solid(font, "Play", color);
	play = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	playDest = { 600, 350, textSurface->clip_rect.w, textSurface->clip_rect.h };

	SDL_FreeSurface(textSurface);
	textSurface = TTF_RenderText_Solid(font, "Exit!", color);
	exit = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	exitDest = { 600, 400, textSurface->clip_rect.w, textSurface->clip_rect.h };

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

	TTF_CloseFont(font);
	
	return true;
}

// Update: draw background
update_status ModuleSceneMenu::PreUpdate()
{
	//Draw background color
	App->renderer->DrawQuad(background, 0, 0, 0, 255, true);
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneMenu::Update()
{
	//Draw Bomberman logo
	/*SDL_Rect texr; 
	texr.x = 0;
	texr.y = 0;
	texr.w = SCREEN_WIDTH; 
	texr.h = SCREEN_HEIGHT;
	SDL_Rect dest = { 0, 0, 800, 400 };
	App->renderer->Blit(graphics, 0, 0, &texr, &dest);*/
	App->renderer->Blit(graphics, position.x, position.y, &(logo), &logoDest);

	App->renderer->Blit(play, position.x, position.y, &(logo), &playDest);
	App->renderer->Blit(exit, position.x, position.y, &(logo), &exitDest);


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fade->isFading() == false)
	{
		App->fade->FadeToBlack((Module*)App->scene_level, this);
		App->audio->PlayFx(fx);
	}

	return UPDATE_CONTINUE;
}