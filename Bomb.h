#pragma once
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

class Bomb :
	public Entity
{
public:
	Bomb(int x, int y, int id, SDL_Texture* gfx);
	~Bomb();

	void Draw();
	void Explode();
	bool CleanUp();
	
public:
	bool exploded = false;
	int idPlayer;
private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation bomb;
	SDL_Rect spriteDest;
	int timeToLive = 20;
	int timer = 0;
	
};

