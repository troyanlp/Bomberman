#include "ModuleSceneResult.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleGUI.h"

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


	win = true;
	numPoints = 1000;

	if (win) textSurface = TTF_RenderText_Solid(font48, "You win!", color);
	else textSurface = TTF_RenderText_Solid(font48, "You lose!", color);
	result = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	resultDest = { 400-(textSurface->clip_rect.w/2), 200, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	if (win) {
		textSurface = TTF_RenderText_Solid(font48, App->gui->GetCharPointer("",numPoints), color);
		points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		pointsDest = { 400 - (textSurface->clip_rect.w / 2), 275, textSurface->clip_rect.w, textSurface->clip_rect.h };
		SDL_FreeSurface(textSurface);
	}

	textSurface = TTF_RenderText_Solid(font16, "Press space or enter to go back to menu", color);
	presskey = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	presskeyDest = { 400 - (textSurface->clip_rect.w / 2), 450, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	return true;
}

update_status ModuleSceneResult::PreUpdate()
{
	//Draw background color
	App->renderer->DrawQuad(background, 0, 0, 0, 255, true);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneResult::Update()
{

	//Draw text
	App->renderer->Blit(result, NULL, NULL, &(section), &resultDest);
	if (win) App->renderer->Blit(points, NULL, NULL, &(section), &pointsDest);
	App->renderer->Blit(presskey, NULL, NULL, &(section), &presskeyDest);

	if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		&& App->fade->isFading() == false)
	{
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
	win = result;
	numPoints = playerPoints;

	if(win) textSurface = TTF_RenderText_Solid(font48, "You win!", color);
	else textSurface = TTF_RenderText_Solid(font48, "You lose!", color);
	result = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	resultDest = { 600, 350, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	if (win) {
		textSurface = TTF_RenderText_Solid(font48, "Exit!", color);
		points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		pointsDest = { 600, 400, textSurface->clip_rect.w, textSurface->clip_rect.h };
		SDL_FreeSurface(textSurface);
	}

}
