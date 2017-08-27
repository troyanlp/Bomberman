#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Globals.h"

struct SDL_Texture;

enum EnemyDirection {
	VERTICALUP,
	VERTICALDOWN,
	HORIZONTALLEFT,
	HORIZONTALRIGHT
};

class Enemy :
	public Entity
{
public:
	Enemy(iPoint pos, EnemyDirection dir);
	~Enemy();

	void Update();
	void Draw();
	void CleanUp();

	void ChangeDirection(EnemyDirection dir);
	bool CanMove(int x, int y);
	void Die();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation up;
	Animation down;
	Animation side;
	EnemyDirection direction;
	int enemySpeed = 1;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_Rect spriteDest;
};

#endif
