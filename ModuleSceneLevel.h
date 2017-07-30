#ifndef __MODULESCENELEVEL_H__
#define __MODULESCENELEVEL_H__

#include "Module.h"

struct SDL_Texture;

class ModuleSceneLevel : public Module
{
public:
	ModuleSceneLevel(bool active = true);
	~ModuleSceneLevel();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
private:
	SDL_Rect background;
	SDL_Rect hud;
};

#endif // __MODULESCENELEVEL_H__