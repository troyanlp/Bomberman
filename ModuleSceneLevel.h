#ifndef __MODULESCENELEVEL_H__
#define __MODULESCENELEVEL_H__

#include "Module.h"
#include "Entity.h"
#include <list>

struct SDL_Texture;
struct ExplotionInstance;
class Enemy;

/*
n = null
e = external block
i = internal block
1 = player 1
2 = player 2
y = enemy
b = brick
t = item
o = bomb
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
	void AddBombToMapLevel(int x, int y);
	std::list<ExplotionInstance> AddExplotionToMapLevel(int x, int y, int flameLevel, int idPlayer);
	void EraseBrickFromMapLevel(int x, int y);

	void BackToMenu();

	int CheckEnemiesLeft();

	bool CleanUp();

private:
	void CreateExternalBlocks();
	void CreateBlocks();
	void CreateBricks();
	void CreateEnemies();

	void InitializeSquareMatrix();

	void BreakBrick(SDL_Rect position);
	
public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect externalBlock;
	SDL_Rect block;
	SDL_Rect brick;
	std::list<Entity*> Entities;
	std::list<Enemy*> Enemies;
	//Matrix of 11 rows and 15 columns
	Square levelMap[11][15]; 

private:
	SDL_Rect background;
};

#endif // __MODULESCENELEVEL_H__