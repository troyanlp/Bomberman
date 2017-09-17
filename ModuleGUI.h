#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include "SDL/include/SDL_ttf.h"

struct SDL_Texture;

class ModuleGUI :
	public Module
{
public:
	ModuleGUI(bool active = true);
	~ModuleGUI();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void ChangePlayerLife(int increment, int player);
	void ChangePlayerPoints(int increment, int player);
	void ChangeMeters(int meters);
	void ChangeBombs(int bombsUsed);

	char* GetCharPointer(char* text, int num);

public:
	int player1Points = 0;
	int player2Points = 0;

private:
	SDL_Rect background;
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textSurface;
	SDL_Texture* life;
	SDL_Rect lifeDest;
	SDL_Texture* points;
	SDL_Rect pointsDest;
	SDL_Texture* steps;
	SDL_Rect stepsDest;
	SDL_Texture* bombs;
	SDL_Rect bombsDest;
	//Data
	int player1Life = 3;
	int player2Life = 3;
	
};

#endif