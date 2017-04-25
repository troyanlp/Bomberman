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
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* background = nullptr;
	uint fx = 0;
};

#endif // __MODULESCENEMENU_H__