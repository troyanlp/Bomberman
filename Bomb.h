#ifndef __BOMB_H__
#define __BOMB_H__

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
	int flameLevel;
private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation bomb;
	SDL_Rect spriteDest;
	int timeToLive = 20;
	int timer = 0;
	
};

#endif