#include "Player.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleBomb.h"

Player::Player(int id, bool AI, SDL_Texture* gfx, iPoint spawnPosition) : id(id), AI(AI), graphics(gfx)
{
	LOG("constructor de player!");
	type = EntityType::PLAYER;
	playerAnimation = PLAYER_IDLE;
	position = spawnPosition;
	
	offsetColliderAS = iPoint(0, 16 * SCREEN_CONVERT);
	offsetColliderWD = iPoint(0, 14 * SCREEN_CONVERT);
	offsetColliderIdle = iPoint(0, 16 * SCREEN_CONVERT);

	colliderAS = { position.x, position.y + offsetColliderAS.y, 15 * SCREEN_CONVERT, 12 * SCREEN_CONVERT };
	colliderWD = { position.x, position.y + offsetColliderWD.y, 17 * SCREEN_CONVERT, 12 * SCREEN_CONVERT };
	colliderIdle = { position.x, position.y+ offsetColliderIdle.y, 17 * SCREEN_CONVERT, 15 * SCREEN_CONVERT };
	collider = App->collision->AddCollider(colliderIdle);

	spriteAS = { position.x, position.y, 15 * SCREEN_CONVERT, 28 * SCREEN_CONVERT };
	spriteWD = { position.x, position.y, 17 * SCREEN_CONVERT, 26 * SCREEN_CONVERT };
	spriteIdle = { position.x, position.y, 17 * SCREEN_CONVERT, 30 * SCREEN_CONVERT };
	spriteDest = spriteIdle;
	
	if(!AI) collider->type = CPLAYER;
	else collider->type = CIA;

	numBombs = 1;
	lives = 3;
	hurtTimer = new Timer();
	invincible = false;
	invincibleShow = true;
	invincibleCount = 0;
}


Player::~Player()
{
}

void Player::Draw()
{
	if (invincible) {
		invincibleCount++;
		if (hurtTimer->EllapsedInSeconds() >= 4) {
			LOG("Fin de invencibilidad");
			invincible = false;
			invincibleShow = true;
			invincibleCount = 0;
			hurtTimer->Stop();
		}
		else {
			if (invincibleCount % 10 == 0) invincibleShow = !invincibleShow;
		}
	}

	if (!destroyed && invincibleShow)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &spriteDest);
}

void Player::CleanUp()
{
	RELEASE(hurtTimer);
}

void Player::ChangeAnimation(PlayerAnimation anim)
{
	playerAnimation = anim;
	switch (anim) {
	case PLAYER_LEFT:
	case PLAYER_DOWN:
		//collider->rect = { position.x, position.y + offsetColliderAS.y, colliderAS.w, colliderAS.h };
		spriteDest = { position.x, position.y, spriteAS.w, spriteAS.h };
		break;
	case PLAYER_RIGHT:
	case PLAYER_UP:
		//collider->rect = { position.x, position.y + offsetColliderWD.y, colliderWD.w, colliderWD.h };
		spriteDest = { position.x, position.y, spriteWD.w, spriteWD.h };
		break;
	case PLAYER_IDLE:
		//collider->rect = { position.x, position.y + offsetColliderIdle.y, colliderIdle.w, colliderIdle.h };
		spriteDest = { position.x, position.y, spriteIdle.w, spriteIdle.h };
		break;
	}
}

bool Player::CanMove(int x, int y)
{
	SDL_Rect newPos = {position.x + offsetColliderIdle.x + x, position.y + offsetColliderIdle.y + y, 0, 0};
	newPos.w = colliderIdle.w;
	newPos.h = colliderIdle.h;
	/*switch (playerAnimation)
	{
	case PLAYER_IDLE:
		newPos.w = colliderIdle.w;
		newPos.h = colliderIdle.h;
		break;
	case PLAYER_DOWN:
	case PLAYER_RIGHT:
		newPos.w = colliderAS.w;
		newPos.h = colliderAS.h;
		break;
	case PLAYER_LEFT:
	case PLAYER_UP:
		newPos.w = colliderWD.w;
		newPos.h = colliderWD.h;
		break;
	}*/
	//App->collision->FindCollision(newPos);
	//return true;
	//LOG("El new pos es: %d, %d, %d, %d", newPos.x, newPos.y, newPos.w, newPos.h);
	if (App->collision->FindCollision(newPos)) {
		//if (x != 0) position.x -= 1;
		//if (y != 0) position.y -= 1;
		return false;
	}
	else {
		return true;
	}
}

bool Player::CanPlantBomb()
{
	if (App->bombs->GetNumBombsFromPlayer(id) < numBombs) return true;
	else return false;
}

void Player::Hurt()
{
	if (!invincible) {
		lives--;
		LOG("AUX!");
		if (lives == 0) {
			LOG("YOU DIED!");
		}
		else {
			hurtTimer->Start();
			invincible = true;
			invincibleShow = true;
			invincibleCount = 0;
			LOG("Inicio de invencibilidad");
		}
	}
}
