#ifndef __BRICK_H__
#define __BRICK_H__

#include "Entity.h"
#include "Item.h"

struct SDL_Texture;

class Brick :
	public Entity
{
public:
	Brick(iPoint spawnPosition, SDL_Texture* gfx, SDL_Rect brick);
	~Brick();

	void Draw();
	void CleanUp();

	Item* Loot();
	void AddLoot(ItemType itemType);

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect brickSprite;
	SDL_Rect spriteDest;
	bool hasLoot = false;
	Item* loot;
	bool looted = false;
};

#endif