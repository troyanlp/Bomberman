#pragma once
#include "Entity.h"
#include "Animation.h"


struct SDL_Texture;

class Bomb :
	public Entity
{
public:
	Bomb(int x, int y);
	~Bomb();

	void Draw();
	void Explode();
	bool CleanUp();
	
public:
	bool exploded = false;
private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation bomb;
	int timeToLive = 20;
	int timer = 0;
};

