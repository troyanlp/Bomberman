#ifndef __BRICK_H__
#define __BRICK_H__

#include "Entity.h"

struct SDL_Texture;

class Brick :
	public Entity
{
public:
	Brick(iPoint spawnPosition, SDL_Texture* gfx, SDL_Rect brick);
	~Brick();

	void Draw();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect brickSprite;
	SDL_Rect spriteDest;
};

#endif