#ifndef __EXPLOTION_H__
#define __EXPLOTION	_H__

#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;

enum ExplotionType {
	FOURWAY,
	TWOWAY,
	ENDING
};

class Explotion :
	public Entity
{
public:
	Explotion(iPoint pos, SDL_Texture* gfx, ExplotionType type);
	~Explotion();

	void Draw();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	ExplotionType explotionType;
	Animation animation;
	iPoint position;
	SDL_Rect spriteDest;
};

#endif