#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

enum ItemType {
	IBOMB,
	ILIFE,
	ISPEED,
	IFLAME,
	IKILL,
	NONE
};

class Item :
	public Entity
{
public:
	Item(iPoint pos, ItemType item);
	~Item();

	void Draw();
	void CleanUp();
private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation animation;
	SDL_Rect spriteDest;
	ItemType itemType;
};

#endif