#ifndef __TRAPSQUARE_H__
#define __TRAPSQUARE_H__

#include "Entity.h"

struct SDL_Texture;

class TrapSquare :
	public Entity
{
public:
	TrapSquare(iPoint spawnPosition);
	~TrapSquare();

	void Draw();
	void CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect squareSprite;
	SDL_Rect spriteDest;
};

#endif