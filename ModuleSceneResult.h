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

private:
	bool win = true;
	int numPoints = 100;
	TTF_Font* font48;
	TTF_Font* font16;
	SDL_Color color;
	SDL_Rect section;
	SDL_Surface* textSurface;
	SDL_Texture* result;
	SDL_Rect resultDest;
	SDL_Texture* points;
	SDL_Rect pointsDest;
	SDL_Texture* presskey;
	SDL_Rect presskeyDest;
	SDL_Rect background;
};

#endif