#ifndef __MODULESCENELEVEL_H__
#define __MODULESCENELEVEL_H__

#include "Module.h"
#include "Entity.h"

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
private:
	void CreateExternalBlocks();
	void CreateBlocks();
	void DrawExternalBlocks();
	void DrawBlocks();
	void DrawBricks();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect externalBlock;
	SDL_Rect block;
	SDL_Rect brick;
	std::list<Entity*> Entitys;

private:
	SDL_Rect background;
	SDL_Rect hud;
};

#endif // __MODULESCENELEVEL_H__