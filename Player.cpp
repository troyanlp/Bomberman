#include "Player.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleBomb.h"
#include "ModuleGUI.h"
#include "ModuleSceneLevel.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

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

	if (App->auxFx == 0) App->auxFx = App->audio->LoadFx("Audio/auxx.ogg");
	if (App->dieFx == 0) App->dieFx = App->audio->LoadFx("Audio/die.ogg");
	if (App->plantBombFx == 0) App->plantBombFx = App->audio->LoadFx("Audio/bomb.ogg");
	if (App->itemFx == 0) App->itemFx = App->audio->LoadFx("Audio/item.ogg");

	numBombs = 1;
	flamePower = 1;
	lives = 3;
	playerSpeed = 1;
	hurtTimer = new Timer();
	invincible = false;
	invincibleShow = true;
	invincibleCount = 0;
	steps = 0;
	meters = 0;
	bombsPlanted = 0;
}


Player::~Player()
{
}

void Player::Draw()
{
	if (steps >= 50) {
		meters++;
		App->gui->ChangeMeters(meters);
		steps = 0;
		LOG("Llevas %d metros!!",meters);
	}

	if(id == 1) points = App->gui->player1Points;
	else points = App->gui->player2Points;

	if (!destroyed) {
		if (collider->collided) {
			switch (collider->message) {
			case HURT:
				Hurt();
				break;
			case ITEMSPEED:
				App->audio->PlayFx(App->itemFx);
				playerSpeed++;
				LOG("Speed: %d", playerSpeed);
				App->gui->ChangePlayerPoints(500, id);
				break;
			case ITEMBOMB:
				App->audio->PlayFx(App->itemFx);
				numBombs++;
				LOG("NumBombs: %d", numBombs);
				App->gui->ChangePlayerPoints(500, id);
				break;
			case ITEMFIRE:
				App->audio->PlayFx(App->itemFx);
				flamePower++;
				LOG("NumBombs: %d", flamePower);
				App->gui->ChangePlayerPoints(500, id);
				break;
			case ITEMLIFE:
				App->audio->PlayFx(App->itemFx);
				lives++;
				App->gui->ChangePlayerLife(1, id);
				LOG("Num Lives: %d", lives);
				App->gui->ChangePlayerPoints(500, id);
				break;
			case ITEMDEATH:
				//App->audio->PlayFx(App->itemFx);
				Die();
				break;
			}
			if (!destroyed) {
				collider->message = NOTHING;
				collider->collided = false;
			}
		}

		if (invincible && !destroyed) {
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
		spriteDest = { position.x, position.y, spriteAS.w, spriteAS.h };
		break;
	case PLAYER_RIGHT:
	case PLAYER_UP:
		spriteDest = { position.x, position.y, spriteWD.w, spriteWD.h };
		break;
	case PLAYER_IDLE:
		spriteDest = { position.x, position.y, spriteIdle.w, spriteIdle.h };
		break;
	}
}

bool Player::CanMove(int x, int y)
{
	SDL_Rect newPos = {position.x + offsetColliderIdle.x + x, position.y + offsetColliderIdle.y + y, 0, 0};
	newPos.w = colliderIdle.w;
	newPos.h = colliderIdle.h;
	if (App->collision->FindCollision(newPos)) {
		return false;
	}
	else {
		return true;
	}
}

bool Player::CanPlantBomb()
{
	if (App->bombs->GetNumBombsFromPlayer(id) < numBombs) {
		App->audio->PlayFx(App->plantBombFx);
		bombsPlanted++;
		App->gui->ChangeBombs(bombsPlanted);
		return true;
	}
	else return false;
}

void Player::Hurt()
{
	if (!invincible) {
		lives--;
		App->gui->ChangePlayerLife(-1, id);
		LOG("AUX!");
		if (lives == 0) {
			
			Die();
		}
		else {
			App->audio->PlayFx(App->auxFx);
			hurtTimer->Start();
			invincible = true;
			invincibleShow = true;
			invincibleCount = 0;
			LOG("Inicio de invencibilidad");
		}
	}
}

void Player::Die()
{
	LOG("YOU DIED!");
	App->audio->PlayFx(App->dieFx);
	destroyed = true;
	App->scene_level->GoToResult(false, points);
}
