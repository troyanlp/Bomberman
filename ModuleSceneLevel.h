#ifndef __MODULESCENELEVEL_H__
#define __MODULESCENELEVEL_H__

#include "Module.h"
#include "Entity.h"

struct SDL_Texture;

/*
n = null
e = external block
i = internal block
1 = player 1
2 = player 2
e = enemy
b = brick
t = item
x = explotion
0 = empty
*/
struct Square {
	char type = 'n';
	iPoint position;
	SDL_Rect squareRect;
};

class ModuleSceneLevel : public Module
{
public:
	ModuleSceneLevel(bool active = true);
	~ModuleSceneLevel();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	
	void PrintLevelMap();

	bool CleanUp();
private:
	void CreateExternalBlocks();
	void CreateBlocks();
	void CreateBricks();

	void InitializeSquareMatrix();


public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect externalBlock;
	SDL_Rect block;
	SDL_Rect brick;
	std::list<Entity*> Entities;
	//Matrix of 11 rows and 15 columns
	Square levelMap[11][15]; 

private:
	SDL_Rect background;
	SDL_Rect hud;
};

#endif // __MODULESCENELEVEL_H__