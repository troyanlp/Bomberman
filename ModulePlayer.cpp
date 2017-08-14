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

}

ModulePlayer::~ModulePlayer()
{
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Bomberman.png");

	destroyed = false;

	// initial position of the player
	position.x = 55;
	position.y = 150;

	CreatePlayer1(position,false);
	//player1->collider = App->collision->AddCollider({ position.x, position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT });
	//Set colliider
	//collider = App->collision->AddCollider({ position.x, position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT });

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);
	if (current_animation != nullptr) RELEASE(current_animation);
	if (player1 != nullptr) RELEASE(player1); LOG("RELEASING PLAYER 1")
	if (player2 != nullptr) RELEASE(player2);

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
/*update_status ModulePlayer::Update()
{	
	// Player movement
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= playerSpeed;
		current_animation = &walkLeft;
		collider->rect = { position.x, position.y, 15 * SCREEN_CONVERT, 28 * SCREEN_CONVERT};
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += playerSpeed;
		current_animation = &walkRight;
		collider->rect = { position.x, position.y, 17 * SCREEN_CONVERT, 29 * SCREEN_CONVERT };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += playerSpeed;
		current_animation = &walkDown;
		collider->rect = { position.x, position.y, 15 * SCREEN_CONVERT, 28 * SCREEN_CONVERT };
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		iPoint aux = CalculateBombPosition();
		App->bombs->AddBomb(aux.x, aux.y);
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= playerSpeed;
		current_animation = &walkUp;
		collider->rect = { position.x, position.y, 17 * SCREEN_CONVERT, 26 * SCREEN_CONVERT };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
			current_animation = &idleDown;
			collider->rect = { position.x, position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT };
	}

	// Draw everything --------------------------------------
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),&(collider->rect));

	SDL_Rect aux = {position.x + collider->rect.w/2, position.y + collider->rect.h/2, 10, 10};
	SDL_Rect auxSprite = { 182, 15, 18, 18 };
	//App->renderer->Blit(graphics, position.x, position.y, &auxSprite, &aux);


	return UPDATE_CONTINUE;
}*/

update_status ModulePlayer::Update()
{
	if (numPlayers == 1) {
		ControlPlayer1();
	}
	else if (numPlayers == 2){

	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::ControlPlayer1()
{
	// Player movement
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && player1->CanMove(-playerSpeed,0))
	{
		player1->position.x -= playerSpeed;
		player1->current_animation = &walkLeft;
		//player1->collider->rect = { player1->position.x, player1->position.y, 15 * SCREEN_CONVERT, 28 * SCREEN_CONVERT };
		player1->ChangeAnimation(PLAYER_LEFT);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && player1->CanMove(playerSpeed,0))
	{
		player1->position.x += playerSpeed;
		//LOG("La posicion x es: %d", player1->position.x);
		player1->current_animation = &walkRight;
		//player1->collider->rect = { player1->position.x, player1->position.y, 17 * SCREEN_CONVERT, 29 * SCREEN_CONVERT };
		player1->ChangeAnimation(PLAYER_RIGHT);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && player1->CanMove(0,playerSpeed))
	{
		player1->position.y += playerSpeed;
		player1->current_animation = &walkDown;
		//player1->collider->rect = { player1->position.x, player1->position.y, 15 * SCREEN_CONVERT, 28 * SCREEN_CONVERT };
		player1->ChangeAnimation(PLAYER_DOWN);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && player1->CanMove(0,-playerSpeed))
	{
		player1->position.y -= playerSpeed;
		player1->current_animation = &walkUp;
		//player1->collider->rect = { player1->position.x, player1->position.y, 17 * SCREEN_CONVERT, 26 * SCREEN_CONVERT };
		player1->ChangeAnimation(PLAYER_UP);
		player1->collider->rect = { player1->position.x + player1->offsetColliderIdle.x, player1->position.y + player1->offsetColliderIdle.y, player1->collider->rect.w, player1->collider->rect.h };
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		iPoint aux = CalculateBombPosition(1);
		App->bombs->AddBomb(aux.x, aux.y);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
		player1->current_animation = &idleDown;
		//player1->collider->rect = { player1->position.x, player1->position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT };
		player1->ChangeAnimation(PLAYER_IDLE);
	}

	player1->Draw();

	// Draw everything --------------------------------------
	//if (destroyed == false)
		//App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &(collider->rect));
}

void ModulePlayer::ControlPlayer2()
{
}

void ModulePlayer::Player1AI()
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

	iPoint bombPosition;

	if (aux != nullptr) {
		
		int quotient = (aux->position.x + aux->collider->rect.w / 2) / 50;
		int residue = (aux->position.x + aux->collider->rect.w / 2) % 50;
		if (residue > 40) bombPosition.x = 50 * (quotient + 1);
		else bombPosition.x = 50 * quotient;
		LOG("Quotient is: %d and residue is: %d", quotient, residue);

		quotient = (aux->position.y + aux->collider->rect.h / 2) / 50;
		residue = (aux->position.y + aux->collider->rect.h / 2) % 50;
		if (residue > 40) bombPosition.y = 50 * (quotient + 1);
		else bombPosition.y = 50 * quotient;
	}
	

	return bombPosition;
}


