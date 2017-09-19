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
	font48 = TTF_OpenFont("Font/emulogic.ttf", 48);
	font16 = TTF_OpenFont("Font/emulogic.ttf", 16);

	//Set up section rect
	section = { 0, 0, 800, 600 };

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 0;

	if (App->winFx == 0) App->winFx = App->audio->LoadFx("Audio/win.ogg");
	if (App->loseFx == 0) App->loseFx = App->audio->LoadFx("Audio/lose.ogg");

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
		if (win) {
			textSurface = TTF_RenderText_Solid(font48, "You win!", color);
		}
		else {
			textSurface = TTF_RenderText_Solid(font48, "You lose!", color);
		}
		result = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
		resultDest = { 400 - (textSurface->clip_rect.w / 2), 200, textSurface->clip_rect.w, textSurface->clip_rect.h };
		SDL_FreeSurface(textSurface);

		if (win) {
			char* text = App->gui->GetCharPointer("Points earned: ", numPoints);
			textSurface = TTF_RenderText_Solid(font16, text, color);
			tempPoints = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			tempPointsDest = { 400 - (textSurface->clip_rect.w / 2), 275, textSurface->clip_rect.w, textSurface->clip_rect.h };
			SDL_FreeSurface(textSurface);
			RELEASE_ARRAY(text);
			
			text = App->gui->GetCharPointer("", numFinalPoints);
			textSurface = TTF_RenderText_Solid(font48, text, color);
			finalPoints = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			finalPointsDest = { 400 - (textSurface->clip_rect.w / 2), 350, textSurface->clip_rect.w, textSurface->clip_rect.h };
			SDL_FreeSurface(textSurface);
			RELEASE_ARRAY(text);

			text = GetMetersText(); 
			textSurface = TTF_RenderText_Solid(font16, text, color);
			metersPoints = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			metersPointsDest = { 400 - (textSurface->clip_rect.w / 2), 300, textSurface->clip_rect.w, textSurface->clip_rect.h };
			SDL_FreeSurface(textSurface);
			RELEASE_ARRAY(text);

			text = GetBombsText();
			textSurface = TTF_RenderText_Solid(font16, text, color);
			bombsPoints = SDL_CreateTextureFromSurface(App->renderer->renderer, textSurface);
			bombsPointsDest = { 400 - (textSurface->clip_rect.w / 2), 325, textSurface->clip_rect.w, textSurface->clip_rect.h };
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
		if (win) {
			App->renderer->Blit(tempPoints, NULL, NULL, &(section), &tempPointsDest);
			App->renderer->Blit(metersPoints, NULL, NULL, &(section), &metersPointsDest);
			App->renderer->Blit(bombsPoints, NULL, NULL, &(section), &bombsPointsDest);
			App->renderer->Blit(finalPoints, NULL, NULL, &(section), &finalPointsDest);
		}
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

	addedWalkedPoints = false;
	addedBombPoints = false;

	return true;
}

void ModuleSceneResult::SetData(bool result, int playerPoints)
{
	dataSet = true;
	
	win = result;
	numPoints = playerPoints;
}

void ModuleSceneResult::SetExtraData(int meters, int bombs)
{
	metersWalked = meters;
	bombsUsed = bombs;
}

char * ModuleSceneResult::GetMetersText()
{
	char* points = App->gui->GetCharPointer("Number of meters walked: ", metersWalked);
	char* rating1;
	char* rating2;
	char* end = ")";
	if (App->currentLevel == 1) {
		/*
		A -> 0-120
		B -> 71-180
		C -> 181-250
		D -> >250
		*/
		if (metersWalked <= 120) {
			rating1 = App->gui->GetCharPointer(" -> A+ (", 0);
			rating2 = App->gui->GetCharPointer("-", 120);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 5000;
			}
		}
		else if (metersWalked > 120 && metersWalked <= 180) {
			rating1 = App->gui->GetCharPointer(" -> B+ (", 120);
			rating2 = App->gui->GetCharPointer("-", 180);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 2500;
			}
		}
		else if (metersWalked > 180 && metersWalked <= 250) {
			rating1 = App->gui->GetCharPointer(" -> C+ (", 180);
			rating2 = App->gui->GetCharPointer("-", 250);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 1000;
			}
		}
		else if (metersWalked > 250) {
			rating1 = App->gui->GetCharPointer(" -> D- (>", 250);
			rating2 = "-";
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints;
			}
		}

	}
	else {
		/*
		A -> 0-120
		B -> 71-180
		C -> 181-250
		D -> >250
		*/
		if (metersWalked <= 120) {
			rating1 = App->gui->GetCharPointer(" -> A+ (", 0);
			rating2 = App->gui->GetCharPointer("-", 120);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 5000;
			}
		}
		else if (metersWalked > 120 && metersWalked <= 180) {
			rating1 = App->gui->GetCharPointer(" -> B+ (", 120);
			rating2 = App->gui->GetCharPointer("-", 180);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 2500;
			}
		}
		else if (metersWalked > 180 && metersWalked <= 250) {
			rating1 = App->gui->GetCharPointer(" -> C+ (", 180);
			rating2 = App->gui->GetCharPointer("-", 250);
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints + 1000;
			}
		}
		else if (metersWalked > 250) {
			rating1 = App->gui->GetCharPointer(" -> D- (>", 250);
			rating2 = "-";
			if (!addedWalkedPoints) {
				addedWalkedPoints = true;
				numFinalPoints = numPoints;
			}
		}
	}

	// calculate the required buffer size (also accounting for the null terminator):
	int bufferSize = strlen(points) + strlen(rating1) + strlen(rating2) + strlen(end) + 1;

	// allocate enough memory for the concatenated string:
	char* concatString = new char[bufferSize];

	// copy strings one and two over to the new buffer:
	strcpy(concatString, points);
	strcat(concatString, rating1);
	strcat(concatString, rating2);
	strcat(concatString, end);

	return concatString;
}

char * ModuleSceneResult::GetBombsText()
{
	char* points = App->gui->GetCharPointer("Number of bombs used: ", bombsUsed);
	char* rating1;
	char* rating2;
	char* end = ")";
	if (App->currentLevel == 1) {
		/*
		A -> 0-15
		B -> 16-20
		C -> 21-30
		D -> >30
		*/
		if (bombsUsed <= 15) {
			rating1 = App->gui->GetCharPointer(" -> A+ (", 0);
			rating2 = App->gui->GetCharPointer("-", 15);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 5000;
			}
		}
		else if (bombsUsed > 15 && bombsUsed <= 20) {
			rating1 = App->gui->GetCharPointer(" -> B+ (", 15);
			rating2 = App->gui->GetCharPointer("-", 20);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 2500;
			}
		}
		else if (bombsUsed > 20 && bombsUsed <= 30) {
			rating1 = App->gui->GetCharPointer(" -> C+ (", 20);
			rating2 = App->gui->GetCharPointer("-", 30);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 1000;
			}
		}
		else if (bombsUsed > 30) {
			rating1 = App->gui->GetCharPointer(" -> D- (>", 30);
			rating2 = "-";
			if (!addedBombPoints) {
				addedBombPoints = true;
			}
		}

	}
	else {
		/*
		A -> 0-10
		B -> 11-15
		C -> 16-25
		D -> >26
		*/
		if (bombsUsed <= 10) {
			rating1 = App->gui->GetCharPointer(" -> A+ (", 0);
			rating2 = App->gui->GetCharPointer("-", 10);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 5000;
			}
		}
		else if (bombsUsed > 10 && bombsUsed <= 15) {
			rating1 = App->gui->GetCharPointer(" -> B+ (", 10);
			rating2 = App->gui->GetCharPointer("-", 15);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 2500;
			}
		}
		else if (bombsUsed > 15 && bombsUsed <= 25) {
			rating1 = App->gui->GetCharPointer(" -> C+ (", 15);
			rating2 = App->gui->GetCharPointer("-", 25);
			if (!addedBombPoints) {
				addedBombPoints = true;
				numFinalPoints += 1000;
			}
		}
		else if (bombsUsed > 25) {
			rating1 = App->gui->GetCharPointer(" -> D- (>", 25);
			rating2 = "-";
			if (!addedBombPoints) {
				addedBombPoints = true;
			}
		}
	}

	// calculate the required buffer size (also accounting for the null terminator):
	int bufferSize = strlen(points) + strlen(rating1) + strlen(rating2) + strlen(end) + 1;

	// allocate enough memory for the concatenated string:
	char* concatString = new char[bufferSize];

	// copy strings one and two over to the new buffer:
	strcpy(concatString, points);
	strcat(concatString, rating1);
	strcat(concatString, rating2);
	strcat(concatString, end);

	return concatString;
}
