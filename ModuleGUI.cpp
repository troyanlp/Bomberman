#include "ModuleGUI.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_ttf.h"


ModuleGUI::ModuleGUI(bool start_enabled) : Module(start_enabled)
{
}


ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Start()
{
	player1Life = 3;
	player1Points = 0;

	//Set up background rect
	background.h = 50;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 0;

	//Set up text
	color = { 255, 255, 255 };
	font = TTF_OpenFont("Font/emulogic.ttf", 12);

	char* text = GetCharPointer("Lives: ", player1Life);
	textSurface = TTF_RenderText_Solid(font, text, color);
	life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	lifeDest = { 5, 5, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	RELEASE_ARRAY(text);

	text = GetCharPointer("Points: ", player1Points);
	textSurface = TTF_RenderText_Solid(font, text, color);
	points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	pointsDest = { 5, 25, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	RELEASE_ARRAY(text);

	text = GetCharPointer("Meters walked: ", 0);
	textSurface = TTF_RenderText_Solid(font, text, color);
	steps = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	stepsDest = { 250, 5, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	RELEASE_ARRAY(text);

	text = GetCharPointer("Bombs used: ", 0);
	textSurface = TTF_RenderText_Solid(font, text, color);
	bombs = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	bombsDest = { 250, 25, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	RELEASE_ARRAY(text);

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	//Draw background color
	App->renderer->DrawQuad(background, 0, 0, 0, 255, true);
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	//Draw text options
	App->renderer->Blit(life, NULL, NULL, &(background), &lifeDest);
	App->renderer->Blit(points, NULL, NULL, &(background), &pointsDest);
	App->renderer->Blit(steps, NULL, NULL, &(background), &stepsDest);
	App->renderer->Blit(bombs, NULL, NULL, &(background), &bombsDest);

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	//TTF_CloseFont(font);

	return true;
}

void ModuleGUI::ChangePlayerLife(int increment, int player)
{
	if (player == 1) {
		player1Life += increment;
		char* text = GetCharPointer("Lives: ", player1Life);
		textSurface = TTF_RenderText_Solid(font, text, color);
		life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		SDL_FreeSurface(textSurface);
		RELEASE_ARRAY(text);
	}
	else {
		player2Life += increment;
		//char* text = GetCharPointer("Lives: ", player2Life);
		//textSurface = TTF_RenderText_Solid(font, text, color);
		//life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		//SDL_FreeSurface(textSurface);
		//RELEASE(text);
	}
}

void ModuleGUI::ChangePlayerPoints(int increment, int player)
{
	if (player == 1) {
		player1Points += increment;
		char* text = GetCharPointer("Points: ", player1Points);
		textSurface = TTF_RenderText_Solid(font, text, color);
		points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		SDL_FreeSurface(textSurface);
		RELEASE_ARRAY(text);
	}
	else {
		player2Life += increment;
		//char* text = GetCharPointer("Points: ", player2Points);
		//textSurface = TTF_RenderText_Solid(font, text, color);
		//points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		//SDL_FreeSurface(textSurface);
		//RELEASE_ARRAY(text);
	}
}

void ModuleGUI::ChangeMeters(int meters)
{
	char* text = GetCharPointer("Meters walked: ", meters);
	textSurface = TTF_RenderText_Solid(font, text, color);
	steps = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	SDL_FreeSurface(textSurface);
	RELEASE_ARRAY(text);
}

void ModuleGUI::ChangeBombs(int bombsUsed)
{
	char* text = GetCharPointer("Bombs used: ", bombsUsed);
	textSurface = TTF_RenderText_Solid(font, text, color);
	bombs = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	SDL_FreeSurface(textSurface);
	RELEASE_ARRAY(text);
}

char * ModuleGUI::GetCharPointer(char * text, int num)
{
	char* numtext = new char();
	itoa(num, numtext, 10);

	// calculate the required buffer size (also accounting for the null terminator):
	int bufferSize = strlen(text) + strlen(numtext) + 1;

	// allocate enough memory for the concatenated string:
	char* concatString = new char[bufferSize];

	// copy strings one and two over to the new buffer:
	strcpy(concatString, text);
	strcat(concatString, numtext);

	//RELEASE_ARRAY(numtext);
	
	return concatString;
}
