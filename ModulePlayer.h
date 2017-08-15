#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;
class Player;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void CreatePlayer1(iPoint position, bool AI = false);
	void CreatePlayer2(iPoint position, bool AI = false);
	
	iPoint CalculateBombPosition(int idPlayer);
	bool BombCanBePlaced(iPoint testPosition);

	void ControlPlayer1();
	void ControlPlayer2();
	void Player2AI();

private:
	int playerSpeed = 1;
public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idleDown;
	Animation idleUp;
	Animation idleLeft;
	Animation idleRight;
	Animation walkLeft;
	Animation walkRight;
	Animation walkUp;
	Animation walkDown;
	Animation celebrate;
	Animation die;
	iPoint position;
	bool destroyed = false;
	Collider* collider;
	//refactoring
	int numPlayers = 0;
	Player* player1 = nullptr;
	Player* player2 = nullptr;
};

#endif