#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "SDL/include/SDL_ttf.h"

struct SDL_Texture;

class ModuleGUI :
	public Module
{
public:
	ModuleGUI(bool active = true);
	~ModuleGUI();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	SDL_Rect background;
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textSurface;
	SDL_Texture* life;
	SDL_Rect lifeDest;
	SDL_Texture* points;
	SDL_Rect pointsDest;
};

#endif