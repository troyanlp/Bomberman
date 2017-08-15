#ifndef __MODULEBOMB_H__
#define __MODULEBOMB_H__

#include "Module.h"
#include <list>
#include "Bomb.h"

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


public:
	std::list<Bomb*> bombs;
};

#endif