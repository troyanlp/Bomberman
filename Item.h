#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

class Item :
	public Entity
{
public:
	Item();
	~Item();

	void Draw();
	void CleanUp();
private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation bomb;
	SDL_Rect spriteDest;
};

#endif