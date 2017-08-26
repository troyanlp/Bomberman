#ifndef __MODULESCENEMENU_H__
#define __MODULESCENEMENU_H__

#include "Module.h"
#include "SDL/include/SDL_ttf.h"

struct SDL_Texture;

class ModuleSceneMenu : public Module
{
public:
	ModuleSceneMenu(bool active = true);
	~ModuleSceneMenu();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect logo;
	SDL_Rect logoDest;
	iPoint position;
	uint fx = 0;
private:
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textSurface;
	SDL_Texture* play;
	SDL_Rect playDest;
	SDL_Texture* exit;
	SDL_Rect exitDest;
	SDL_Rect background;
};

#endif // __MODULESCENEMENU_H__