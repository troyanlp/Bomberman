#include "ModuleBomb.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Explotion.h"
#include "Bomb.h"
#include "ModuleSceneLevel.h"

ModuleBomb::ModuleBomb(bool start_enabled) : Module(start_enabled)
{
	//Explotion animations
	/*for (int i = 0; i < 7; i++) {
		fourWay.frames.push_back({ 0 + (48*i), 0, 48, 48});
	}
	fourWay.speed = 0.075f;
	fourWay.loop = true;
	for (int i = 0; i < 7; i++) {
		twoWay.frames.push_back({ 0 + (48 * i), 48, 48, 43 });
	}
	twoWay.speed = 0.075f;
	for (int i = 0; i < 7; i++) {
		ending.frames.push_back({ 0 + (48 * i), 91, 48, 43 });
	}
	ending.speed = 0.075f;
	*/
}


ModuleBomb::~ModuleBomb()
{
}

bool ModuleBomb::Start()
{
	//Loading graphics
	bombGraphics = App->textures->Load("Bomberman.png");
	explotionGraphics = App->textures->Load("Bomberman Explotions.png");

	return true;
}

update_status ModuleBomb::Update()
{	
	//Update Bombs
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
	//Update Explotions
	for (list<Explotion*>::iterator it = explotions.begin(); it != explotions.end(); ++it) {
		if ((*it)->destroyed) {
			(*it)->CleanUp();
			RELEASE(*it);
			it = explotions.erase(it);
			if (it == explotions.end()) break;
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

	App->textures->Unload(bombGraphics);

	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it)
		RELEASE(*it);

	bombs.clear();

	for (list<Explotion*>::iterator it = explotions.begin(); it != explotions.end(); ++it)
		RELEASE(*it);

	explotions.clear();

	return true;
}

void ModuleBomb::AddBomb(int x, int y, int idPlayer, int flamePower)
{
	LOG("Adding bomb!");
	Bomb* aux = new Bomb(x,y,idPlayer,flamePower,bombGraphics);
	bombs.push_back(aux);
	App->scene_level->AddBombToMapLevel(x,y);
}

int ModuleBomb::GetNumBombsFromPlayer(int id)
{
	int count = 0;
	for (list<Bomb*>::iterator it = bombs.begin(); it != bombs.end(); ++it) {
		if ((*it)->idPlayer == id) count++;
	}
	return count;
}

void ModuleBomb::AddExplotion(SDL_Rect position, ExplotionInstance instance)
{
	Explotion* aux = new Explotion(position, explotionGraphics, instance);
	explotions.push_back(aux);
}
