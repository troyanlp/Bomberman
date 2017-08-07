#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Entity.h"

struct SDL_Texture;

class Block :
	public Entity
{
public:
	Block(iPoint spawnPosition, bool ext, SDL_Texture* gfx, SDL_Rect block);
	~Block();

	void Draw();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	bool external = false;
	SDL_Rect blockSprite;
	SDL_Rect spriteDest;
};

#endif