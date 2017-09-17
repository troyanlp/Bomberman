#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneMenu.h"
#include "ModuleWindow.h"
#include "ModuleSceneLevel.h"
#include "ModuleCollision.h"
#include "ModuleGUI.h"
#include "ModulePlayer.h"
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
	
	textSurface = TTF_RenderText_Solid(font, "Exit", color);
	exit = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	exitDest = { 600, 400, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(font, "Level 1", color);
	level1 = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	level1Dest = { 600, 350, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(font, "Level 2", color);
	level2 = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	level2Dest = { 600, 400, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(font, "Back", color);
	back = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	backDest = { 600, 450, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	//Bomb select
	bombGraphics = App->textures->Load("Bomberman.png");

	bomb.frames.push_back({ 177, 11, 18, 18 });
	bomb.frames.push_back({ 195, 11, 18, 18 });
	bomb.frames.push_back({ 211, 11, 18, 18 });
	bomb.speed = 0.075f;

	bomb_animation = &bomb;
	bombDest = { 560, 345, 30, 30 };

	focus = 1;
	focusLevel = 1;
	levelSelection = false;

	if(App->bombermanFx == 0) App->bombermanFx = App->audio->LoadFx("Audio/bomberman.ogg");
	if (App->selectFx == 0) App->selectFx = App->audio->LoadFx("Audio/select.ogg");
	if (App->confirmFx == 0) App->confirmFx = App->audio->LoadFx("Audio/confirm.ogg");

	App->audio->PlayFx(App->bombermanFx);
		
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
	App->textures->Unload(bombGraphics);

	TTF_CloseFont(font);
	
	return true;
}

void ModuleSceneMenu::BackToMenu()
{
	backToMenu = true;
}

// Update: draw background
update_status ModuleSceneMenu::PreUpdate()
{
	//Draw background color
	//App->renderer->DrawQuad(background, 0, 0, 0, 255, true);
	if (backToMenu) {
		if (App->fade->isFading() == false) {
			backToMenu = false;
			App->scene_level->CleanUp();
			//App->scene_level->Disable();
			App->collision->CleanUp();
			//App->gui->CleanUp();
			App->gui->Disable();
			App->player->Disable();
		}
	}

	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleSceneMenu::Update()
{
	//Draw Bomberman logo
	App->renderer->Blit(graphics, position.x, position.y, &(logo), &logoDest);
	//Draw text options
	if (levelSelection) {
		App->renderer->Blit(level1, position.x, position.y, &(logo), &level1Dest);
		App->renderer->Blit(level2, position.x, position.y, &(logo), &level2Dest);
		App->renderer->Blit(back, position.x, position.y, &(logo), &backDest);
	}
	else {
		App->renderer->Blit(play, position.x, position.y, &(logo), &playDest);
		App->renderer->Blit(exit, position.x, position.y, &(logo), &exitDest);
	}
	//Logic of focus
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (levelSelection) {
			if (focusLevel < 3) {
				App->audio->PlayFx(App->selectFx);
				focusLevel++;
				bombDest.y += 50;
			}
		}
		else {
			if (focus < 2) {
				App->audio->PlayFx(App->selectFx);
				focus++;
				bombDest.y += 50;
			}
		}
		
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (levelSelection) {
			if (focusLevel > 1) {
				App->audio->PlayFx(App->selectFx);
				focusLevel--;
				bombDest.y -= 50;
			}
		}
		else {
			if (focus > 1) {
				App->audio->PlayFx(App->selectFx);
				focus--;
				bombDest.y -= 50;
			}
		}
		
	}
	
	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) 
		&& App->fade->isFading() == false)
	{
		if (levelSelection) {
			switch (focusLevel)
			{
			case 1:
				App->audio->PlayFx(App->confirmFx);
				App->currentLevel = 1;
				App->fade->FadeToBlack((Module*)App->scene_level, this);
				//App->audio->PlayFx(fx);
				break;
			case 2:
				App->audio->PlayFx(App->confirmFx);
				App->currentLevel = 2;
				App->fade->FadeToBlack((Module*)App->scene_level, this);
				//App->audio->PlayFx(fx);
				break;
			case 3:
				App->audio->PlayFx(App->confirmFx);
				levelSelection = false;
				focusLevel = 1;
				focus = 1;
				bombDest.y = 345;
				break;
			}
		}
		else {
			if (focus == 1) {
				App->audio->PlayFx(App->confirmFx);
				levelSelection = true;
				focusLevel = 1;
				focus = 1;
				bombDest.y = 345;
			}
			else {
				App->audio->PlayFx(App->confirmFx);
				return UPDATE_STOP;
			}
		}
		
		
	}
	
	//Draw bomb to indicate focus
	App->renderer->Blit(bombGraphics, position.x, position.y, &(bomb_animation->GetCurrentFrame()), &bombDest);

	return UPDATE_CONTINUE;
}