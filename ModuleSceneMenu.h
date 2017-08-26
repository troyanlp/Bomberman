#ifndef __MODULESCENEMENU_H__
#define __MODULESCENEMENU_H__

#include "Module.h"

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
	SDL_Rect background;
};

#endif // __MODULESCENEMENU_H__