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
	//Set up background rect
	background.h = 50;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 0;

	//Set up text
	color = { 255, 255, 255 };
	font = TTF_OpenFont("emulogic.ttf", 12);

	textSurface = TTF_RenderText_Solid(font, GetCharPointer("Lives: ",player1Life), color);
	life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	lifeDest = { 5, 5, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(font, GetCharPointer("Points: ", player1Points), color);
	points = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	pointsDest = { 5, 25, textSurface->clip_rect.w, textSurface->clip_rect.h };
	SDL_FreeSurface(textSurface);

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

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	TTF_CloseFont(font);

	return true;
}

void ModuleGUI::ChangePlayerLife(int increment, int player)
{
	if (player == 1) {
		player1Life += increment;
		textSurface = TTF_RenderText_Solid(font, GetCharPointer("Lives: ", player1Life), color);
		life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	}
	else {
		player2Life += increment;
		//textSurface = TTF_RenderText_Solid(font, GetCharPointer("Lives: ", player2Life), color);
		//life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	}
}

void ModuleGUI::ChangePlayerPoints(int increment, int player)
{
	if (player == 1) {
		player1Points += increment;
		textSurface = TTF_RenderText_Solid(font, GetCharPointer("Lives: ", player1Life), color);
		life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	}
	else {
		player2Life += increment;
		//textSurface = TTF_RenderText_Solid(font, GetCharPointer("Lives: ", player2Life), color);
		//life = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
	}
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
	
	return concatString;
}
