#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

class Enemy :
	public Entity
{
public:
	Enemy(int x, int y);
	~Enemy();

	void Update();
	void Draw();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation horizontal;
	bool right;
	SDL_Rect spriteDest;
};

#endif
