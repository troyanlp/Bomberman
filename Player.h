#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

class Player :
	public Entity
{
public:
	Player(int id, bool AI, iPoint spawnPosition);
	~Player();

	void Draw();
	bool CleanUp();


public:
	SDL_Texture* graphics = nullptr;
	int id;
	bool AI = false;
	Animation* current_animation = nullptr;
};

#endif