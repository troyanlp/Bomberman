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
	//LOG("Update ModuleBomb");

	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it) {
		if ((*it)->exploded) {
			//it = bombs.erase(it);
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

void ModuleBomb::AddBomb(int x, int y)
{
	LOG("Adding bomb!");
	Bomb* aux = new Bomb(x,y);
	bombs.push_back(aux);
}
