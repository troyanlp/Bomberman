#ifndef __MODULESCENERESULT_H__
#define __MODULESCENERESULT_H__

#include "Module.h"
#include "SDL/include/SDL_ttf.h"

struct SDL_Surface;

class ModuleSceneResult :
	public Module
{
public:
	ModuleSceneResult(bool active = true);
	~ModuleSceneResult();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void SetData(bool result, int playerPoints);
	void SetExtraData(int meters, int bombsUsed);

	char* GetMetersText();
	char* GetBombsText();

private:
	bool dataSet = false;
	bool win = true;
	int numPoints = 100;
	int numFinalPoints = 0;
	int metersWalked = 0;
	int bombsUsed = 0;
	bool audioPlayed = false;
	bool addedWalkedPoints = false;
	bool addedBombPoints = false;
	TTF_Font* font48;
	TTF_Font* font16;
	SDL_Color color;
	SDL_Rect section;
	SDL_Surface* textSurface;
	SDL_Texture* result;
	SDL_Rect resultDest;
	SDL_Texture* tempPoints;
	SDL_Rect tempPointsDest;
	SDL_Texture* finalPoints;
	SDL_Rect finalPointsDest;
	SDL_Texture* metersPoints;
	SDL_Rect metersPointsDest;
	SDL_Texture* bombsPoints;
	SDL_Rect bombsPointsDest;
	SDL_Texture* presskey;
	SDL_Rect presskeyDest;
	SDL_Rect background;
};

#endif