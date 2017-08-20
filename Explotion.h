#ifndef __EXPLOTION_H__
#define __EXPLOTION	_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

class Explotion :
	public Entity
{
public:
	Explotion(iPoint pos);
	~Explotion();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	iPoint position;
	SDL_Rect spriteDest;
};

#endif