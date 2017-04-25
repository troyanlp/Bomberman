#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;

struct Particle
{
	bool to_delete = false;

	// TODO 1: Fill in the structure with all the properties you need for each particle
	iPoint position;
	int speed;
	int initial_frame = 0;
	int time_to_live;
	int damage;
	bool hasCollider;
	iPoint direction;
	Animation anim;
	char* audio;

	// TODO 11: Add an optional collider to each particle
	Collider* collider;


	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y); // feel free to expand this call

private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle laser;
	Particle explosion;
};

#endif // __MODULEPARTICLES_H__