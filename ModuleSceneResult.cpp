#include "ModuleSceneResult.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleGUI.h"
#include "ModuleSceneLevel.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

ModuleSceneResult::ModuleSceneResult(bool active) : Module(active)
{
}


ModuleSceneResult::~ModuleSceneResult()
{
}

bool ModuleSceneResult::Start()
{
	LOG("Loading Result");

	//Text
	color = { 255, 255, 255 };
	font48 = TTF_OpenFont("emulogic.ttf", 48);
	font16 = TTF_OpenFont("emulogic.ttf", 16);

	//Set up section rect
	section = { 0, 0, 800, 600 };

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 0;

	if (App->winFx == 0) App->winFx = App->audio->LoadFx("Audio/win.ogg");
	if (App->loseFx == 0) App->loseFx = App->audio->LoadFx("Audio/lose.ogg");

	//win = true;
	//numPoints = 1000;

	/*if (win) 
		textSurface = TTF_RenderText_Solid(font48, "You win!", color);
	else textSurface = TTF_RenderText_Solid(font48, "You lose!", color);
	result = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	resultDest = { 400-(textSurface->clip_rect.w/2), 200, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	if (win) {
		char* text = App->gui->GetCharPointer("", numPoints);
		textSurface = TTF_RenderText_Solid(font48, text, color);
		points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		pointsDest = { 400 - (textSurface->clip_rect.w / 2), 275, textSurface->clip_rect.w, textSurface->clip_rect.h };
		SDL_FreeSurface(textSurface);
		RELEASE_ARRAY(text);
	}*/

	textSurface = TTF_RenderText_Solid(font16, "Press space or enter to go back to menu", color);
	presskey = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	presskeyDest = { 400 - (textSurface->clip_rect.w / 2), 450, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	audioPlayed = false;

	return true;
}

update_status ModuleSceneResult::PreUpdate()
{
	if (App->fade->isFading() == false) {
		App->scene_level->CleanUp();
		//App->scene_level->Disable();
		App->collision->CleanUp();
		//App->gui->CleanUp();
		App->gui->Disable();
		App->player->Disable();
	}

	if (dataSet) {
		if (win)
			textSurface = TTF_RenderText_Solid(font48, "You win!", color);
		else
			textSurface = TTF_RenderText_Solid(font48, "You lose!", color);
		result = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		resultDest = { 400 - (textSurface->clip_rect.w / 2), 200, textSurface->clip_rect.w, textSurface->clip_rect.h };
		SDL_FreeSurface(textSurface);

		if (win) {
			char* text = App->gui->GetCharPointer("", numPoints);
			textSurface = TTF_RenderText_Solid(font48, text, color);
			points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			pointsDest = { 400 - (textSurface->clip_rect.w / 2), 275, textSurface->clip_rect.w, textSurface->clip_rect.h };
			SDL_FreeSurface(textSurface);
			RELEASE_ARRAY(text);
		}
	}

	//Draw background color
	App->renderer->DrawQuad(background, 0, 0, 0, 255, true);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneResult::Update()
{
	if (!audioPlayed && App->fade->isFading() == false && dataSet) {
		audioPlayed = true;
		if (win) App->audio->PlayFx(App->winFx);
		else App->audio->PlayFx(App->loseFx);
	}
	//Draw text
	if (dataSet) {
		App->renderer->Blit(result, NULL, NULL, &(section), &resultDest);
		if (win)
			App->renderer->Blit(points, NULL, NULL, &(section), &pointsDest);
		App->renderer->Blit(presskey, NULL, NULL, &(section), &presskeyDest);
	}

	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		&& App->fade->isFading() == false)
	{
		dataSet = false;
		App->fade->FadeToBlack((Module*)App->scene_menu, this);

	}

	return UPDATE_CONTINUE;
}

bool ModuleSceneResult::CleanUp()
{
	LOG("Unloading result scene");

	TTF_CloseFont(font48);
	TTF_CloseFont(font16);

	return true;
}

void ModuleSceneResult::SetData(bool result, int playerPoints)
{
	dataSet = true;
	
	win = result;
	numPoints = playerPoints;
}
