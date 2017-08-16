#include "ModuleBomb.h"


ModuleBomb::ModuleBomb(bool start_enabled) : Module(start_enabled)
{

}


ModuleBomb::~ModuleBomb()
{
}

bool ModuleBomb::Start()
{
	return true;
}

update_status ModuleBomb::Update()
{
	//LOG("Active Bombs: %d", bombs.size());
	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it) {
		if ((*it)->exploded) {
			RELEASE(*it);
			it = bombs.erase(it);
			if (it == bombs.end()) break;
		}
		else {
			(*it)->Draw();
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleBomb::CleanUp()
{
	LOG("Freeing all bombs");

	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it)
		RELEASE(*it);

	bombs.clear();

	return true;
}

void ModuleBomb::AddBomb(int x, int y, int idPlayer)
{
	LOG("Adding bomb!");
	Bomb* aux = new Bomb(x,y,idPlayer);
	bombs.push_back(aux);
}

int ModuleBomb::GetNumBombsFromPlayer(int id)
{
	int count = 0;
	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it) {
		if ((*it)->idPlayer == id) count++;
	}
	return count;
}
