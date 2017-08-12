#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

enum PlayerAnimation {
	PLAYER_IDLE,
	PLAYER_DOWN,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_UP
};

class Player :
	public Entity
{
public:
	Player(int id, bool AI, SDL_Texture* gfx, iPoint spawnPosition);
	~Player();

	void Draw();
	bool CleanUp();

	void ChangeAnimation(PlayerAnimation anim);
	bool CanMove(int x, int y);


public:
	SDL_Texture* graphics = nullptr;
	int id;
	bool AI = false;
	PlayerAnimation playerAnimation;
	Animation* current_animation = nullptr;
	iPoint offsetColliderAS;
	iPoint offsetColliderWD;
	iPoint offsetColliderIdle;
	SDL_Rect colliderAS;
	SDL_Rect colliderWD;
	SDL_Rect colliderIdle;
	SDL_Rect spriteAS;
	SDL_Rect spriteWD;
	SDL_Rect spriteIdle;
	SDL_Rect spriteDest;
};

#endif