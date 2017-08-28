#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleBomb.h"
#include "Player.h"
#include "SDL/include/SDL.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{

	

}

ModulePlayer::~ModulePlayer()
{
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// idle animation
	idleDown.frames.push_back({ 113, 2, 17, 30 });
	idleDown.frames.push_back({ 133, 2, 17, 30 });
	idleDown.frames.push_back({ 153, 2, 17, 30 });
	idleDown.speed = 0.075f;

	// walk right animation
	walkRight.frames.push_back({ 5, 36, 17, 29 });
	walkRight.frames.push_back({ 27, 36, 17, 29 });
	walkRight.frames.push_back({ 47, 36, 17, 29 });
	walkRight.frames.push_back({ 67, 36, 17, 29 });
	walkRight.frames.push_back({ 87, 36, 17, 29 });
	walkRight.frames.push_back({ 47, 36, 17, 29 });
	walkRight.speed = 0.1f;

	// walk left animation
	walkLeft.frames.push_back({ 5, 99, 15, 28 });
	walkLeft.frames.push_back({ 25, 99, 15, 28 });
	walkLeft.frames.push_back({ 45, 99, 15, 28 });
	walkLeft.frames.push_back({ 65, 99, 15, 28 });
	walkLeft.frames.push_back({ 87, 99, 15, 28 });
	walkLeft.frames.push_back({ 45, 99, 15, 28 });
	walkLeft.speed = 0.1f;

	// walk up animation
	walkUp.frames.push_back({ 4, 69, 17, 26 });
	walkUp.frames.push_back({ 25, 69, 17, 26 });
	walkUp.frames.push_back({ 45, 69, 17, 26 });
	walkUp.frames.push_back({ 66, 69, 17, 26 });
	walkUp.frames.push_back({ 88, 69, 17, 26 });
	walkUp.frames.push_back({ 45, 69, 17, 26 });
	walkUp.speed = 0.1f;

	// walk down animation
	walkDown.frames.push_back({ 3, 3, 17, 29 });
	walkDown.frames.push_back({ 24, 3, 17, 29 });
	walkDown.frames.push_back({ 45, 3, 17, 29 });
	walkDown.frames.push_back({ 66, 3, 17, 29 });
	walkDown.frames.push_back({ 87, 3, 17, 29 });
	walkDown.frames.push_back({ 45, 3, 17, 29 });
	walkDown.speed = 0.1f;

	// celebrate animation
	celebrate.frames.push_back({ 177, 98, 15, 29 });
	celebrate.frames.push_back({ 195, 98, 21, 29 });
	celebrate.frames.push_back({ 219, 98, 19, 29 });
	celebrate.speed = 0.1f;

	// die animation
	die.frames.push_back({ 1, 135, 17, 30 });
	die.frames.push_back({ 22, 135, 17, 30 });
	die.frames.push_back({ 44, 135, 21, 30 });
	die.frames.push_back({ 69, 135, 19, 30 });
	die.frames.push_back({ 93, 135, 21, 30 });
	die.frames.push_back({ 118, 135, 17, 30 });
	die.frames.push_back({ 139, 135, 26, 30 });
	die.frames.push_back({ 167, 135, 20, 30 });
	die.frames.push_back({ 191, 135, 17, 30 });
	die.frames.push_back({ 211, 135, 19, 30 });
	die.speed = 0.1f;
	die.loop = false;

	graphics = App->textures->Load("Bomberman.png");

	destroyed = false;

	// initial position of the player
	position.x = 55;
	position.y = 150;

	numPlayers = 0;

	CreatePlayer1(position,false);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	if (current_animation != nullptr) RELEASE(current_animation);
	if (player1 != nullptr) player1->CleanUp(); RELEASE(player1); LOG("RELEASING PLAYER 1")
	if (player2 != nullptr) player2->CleanUp(); RELEASE(player2);

	return true;
}

void ModulePlayer::CreatePlayer1(iPoint position, bool AI)
{
	if (numPlayers < 2) {
		numPlayers++;
		player1 = new Player(1, false, graphics, position);
	}
}

void ModulePlayer::CreatePlayer2(iPoint position, bool AI)
{
	if (numPlayers < 2) {
		numPlayers++;
		player2 = new Player(2, true, graphics, position);
	}
}

// Update
update_status ModulePlayer::Update()
{
	if (numPlayers == 1) {
		ControlPlayer1();
	}
	else if (numPlayers == 2){
		ControlPlayer1();
		ControlPlayer2();
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::ControlPlayer1()
{
	// Player movement
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player1->CanMove(-player1->playerSpeed,0))
	{
		player1->position.x -= player1->playerSpeed;
		player1->current_animation = &walkLeft;
		player1->ChangeAnimation(PLAYER_LEFT);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player1->CanMove(player1->playerSpeed,0))
	{
		player1->position.x += player1->playerSpeed;
		player1->current_animation = &walkRight;
		player1->ChangeAnimation(PLAYER_RIGHT);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && player1->CanMove(0, player1->playerSpeed))
	{
		player1->position.y += player1->playerSpeed;
		player1->current_animation = &walkDown;
		player1->ChangeAnimation(PLAYER_DOWN);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && player1->CanMove(0,-player1->playerSpeed))
	{
		player1->position.y -= player1->playerSpeed;
		player1->current_animation = &walkUp;
		player1->ChangeAnimation(PLAYER_UP);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		if (player1->CanPlantBomb()) {
			iPoint aux = CalculateBombPosition(1);
			App->bombs->AddBomb(aux.x, aux.y,1,player1->flamePower);
		}
		
	}
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
		player1->current_animation = &idleDown;
		player1->ChangeAnimation(PLAYER_IDLE);
	}

	player1->Draw();
}

void ModulePlayer::ControlPlayer2()
{
}

void ModulePlayer::Player2AI()
{
}

iPoint ModulePlayer::CalculateBombPosition(int idPlayer)
{
	Player* aux;
	if (idPlayer == 1) {
		if (player1 != nullptr) aux = player1;
		else LOG("Player 1 doesnt exist!");
	}
	else if (idPlayer == 2) {
		if (player2 != nullptr) aux = player2;
		else LOG("Player 2 doesnt exist!");
	}

	//aux->position.x += aux->spriteIdle.w / 2;
	//aux->position.y += aux->spriteIdle.h / 2;

	iPoint bombPosition;

	if (aux != nullptr) {
		
		int quotient = (aux->position.x + aux->collider->rect.w / 2) / 50;
		int residue = (aux->position.x + aux->collider->rect.w / 2) % 50;
		if (residue > 40 && BombCanBePlaced(iPoint(50*(quotient+1),aux->position.y))) {
			bombPosition.x = 50 * (quotient + 1);
		}
		else {
			bombPosition.x = 50 * quotient;
		}
		LOG("Quotient is: %d and residue is: %d", quotient, residue);

		quotient = (aux->position.y + aux->collider->rect.h) / 50;
		residue = (aux->position.y + aux->collider->rect.h) % 50;
		if (residue < 20  && residue > 45 && BombCanBePlaced(iPoint(bombPosition.x, 50 * (quotient+1)))) {
			bombPosition.y = 50 * (quotient);
		}
		else {
			if(residue > 45) bombPosition.y = 50 * (quotient+1);
			else bombPosition.y = 50 * quotient;
		}
	}
	

	return bombPosition;
}

bool ModulePlayer::BombCanBePlaced(iPoint testPosition)
{
	SDL_Rect dest = {testPosition.x, testPosition.y, 50, 50};
	if (App->collision->FindCollision(dest)) return false;
	else return true;
}


