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

struct ExplotionInstance {
	ExplotionType type;
	SDL_Rect position;
	int rotation;
};

class Explotion :
	public Entity
{
public:
	Explotion(SDL_Rect position, SDL_Texture* gfx, ExplotionType type);
	~Explotion();

	void Draw();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	ExplotionType explotionType;
	Animation animation;
	int idPlayer;
	iPoint position;
	SDL_Rect spriteDest;
};

#endif