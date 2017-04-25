#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	laser.speed = 5;
	laser.initial_frame = 0;
	laser.time_to_live = 1200;
	laser.damage = 10;
	laser.hasCollider = true;
	laser.collider = new Collider({ 232, 103, 16, 12 });
	
	laser.direction = iPoint(1,0);
	laser.anim.frames.push_back({ 232, 103, 16, 12 });
	laser.anim.frames.push_back({ 249, 103, 16, 12 });
	laser.audio = "rtype/laser.wav";

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	explosion.speed = 5;
	explosion.initial_frame = 0;
	explosion.time_to_live = 12;
	explosion.damage = 10;
	explosion.hasCollider = true;
	explosion.direction = iPoint(0, 0);
	explosion.anim.frames.push_back({ 274, 296, 33, 30 });
	explosion.anim.frames.push_back({ 313, 296, 33, 30 });
	explosion.anim.frames.push_back({ 346, 296, 33, 30 });
	explosion.anim.frames.push_back({ 382, 296, 33, 30 });
	explosion.anim.frames.push_back({ 419, 296, 33, 30 });
	explosion.anim.frames.push_back({ 457, 296, 33, 30 });
	explosion.audio = "rtype/explosion.wav";

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;

		if(p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else {
			App->renderer->Blit(graphics, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()), 1.0f);
			++it;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	
	Particle *p = new Particle(particle);
	p->position.x = x;
	p->position.y = y;
	
	active.push_back(p);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	SDL_Rect texture;
	texture.w = 30;
	texture.h = 10;
	//collider = App->collision->AddCollider(texture);
}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) 
{
	this->speed = p.speed;
	this->initial_frame = SDL_GetTicks();
	this->time_to_live = p.time_to_live;
	this->damage = p.damage;
	this->hasCollider = p.hasCollider;
	//this->collider = App->collision->AddCollider(p.collider->rect);
	this->direction = p.direction;
	this->anim = p.anim;
	this->audio = p.audio;
}

Particle::~Particle()
{
	collider->to_delete = true;
	to_delete = true;
}

bool Particle::Update()
{
	bool ret = true;

	collider->rect.x = position.x + collider->rect.w/2;
	collider->rect.y = position.y;

	// TODO 5: This is the core of the particle functionality.
	// Return false if the particle must be destroyed
	
	if (SDL_GetTicks() >= (initial_frame + time_to_live)) {
		to_delete = true;
		ret = false;
	}
	else {
		position.x += speed;
	}

	return ret;
}

