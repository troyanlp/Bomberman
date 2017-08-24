#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy :
	public Entity
{
public:
	Enemy(iPoint pos, char direction);
	~Enemy();

	void Update();
	void Draw();
	void CleanUp();

	void Die();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation side;
	bool goingDown = false;
	bool goingRight = false;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_Rect spriteDest;
};

#endif
