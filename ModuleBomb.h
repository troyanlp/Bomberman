#ifndef __MODULEBOMB_H__
#define __MODULEBOMB_H__

#include "Module.h"
#include <list>
#include "Animation.h"

class Bomb;
class Explotion;
struct SDL_Texture;

class ModuleBomb :
	public Module
{
public:
	ModuleBomb(bool active = true);
	~ModuleBomb();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddBomb(int x, int y, int idPlayer);
	int GetNumBombsFromPlayer(int id);

	void AddExplotion(int x, int y);


public:
	std::list<Bomb*> bombs;
	std::list<Explotion*> explotions;
	//Graphics
	SDL_Texture* bombGraphics = nullptr;
	SDL_Texture* explotionGraphics = nullptr;
	//Bomb Animation
	Animation bomb;
	//Explotions Animation
	Animation fourWay;
	Animation twoWay;
	Animation ending;
};

#endif