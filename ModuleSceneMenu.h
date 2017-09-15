#ifndef __MODULESCENEMENU_H__
#define __MODULESCENEMENU_H__

#include "Module.h"
#include "SDL/include/SDL_ttf.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneMenu : public Module
{
public:
	ModuleSceneMenu(bool active = true);
	~ModuleSceneMenu();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	void BackToMenu();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect logo;
	SDL_Rect logoDest;
	iPoint position;
	uint fx = 0;
private:
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textSurface;
	SDL_Texture* play;
	SDL_Rect playDest;
	SDL_Texture* exit;
	SDL_Rect exitDest;
	SDL_Texture* level1;
	SDL_Rect level1Dest;
	SDL_Texture* level2;
	SDL_Rect level2Dest;
	SDL_Texture* back;
	SDL_Rect backDest;
	SDL_Rect background;
	SDL_Texture* bombGraphics = nullptr;
	Animation* bomb_animation = nullptr;
	Animation bomb;
	SDL_Rect bombDest;
	int focus = 1;
	int focusLevel = 1;
	bool levelSelection = false;
	bool backToMenu = false;
};

#endif // __MODULESCENEMENU_H__