#ifndef __DOOR_H__
#define __DOOR_H__

#include "Entity.h"

struct SDL_Texture;

class Door :
	public Entity
{
public:
	Door(iPoint pos);
	~Door();

	void Draw();
	void CleanUp();

	bool CheckFinishedLevel();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect doorSprite;
	SDL_Rect spriteDest;
};

#endif