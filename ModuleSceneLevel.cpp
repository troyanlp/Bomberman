#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleSceneLevel.h"
#include "ModuleBomb.h"
#include "Entity.h"
#include "Block.h"
#include "Brick.h"

ModuleSceneLevel::ModuleSceneLevel(bool active) : Module(active)
{}

ModuleSceneLevel::~ModuleSceneLevel()
{}

// Load assets
bool ModuleSceneLevel::Start()
{
	LOG("Loading space scene");
	
	graphics = App->textures->Load("Super Bomberman 5 JPN - Stage 01.png");

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 50;
	//Set up hud rect
	hud.h = 50;
	hud.w = SCREEN_WIDTH;
	hud.x = 0;
	hud.y = 0;

	//Set up external block
	externalBlock.x = 9;
	externalBlock.y = 20;
	externalBlock.w = 15;
	externalBlock.h = 13;

	//Set up block
	block.x = 146;
	block.y = 240;
	block.w = 14;
	block.h = 15;

	//Set up brick
	brick.x = 78;
	brick.y = 240;
	brick.w = 15;
	brick.h = 15;

	App->player->Enable();
	App->bombs->Enable();
	//App->particles->Enable();
	//App->collision->Enable();

	//App->audio->PlayMusic("stage1.ogg", 1.0f);
	
	//create some colliders for the walls
	CreateExternalBlocks();
	CreateBlocks();
	CreateBricks();
	

	return true;
}

// UnLoad assets
bool ModuleSceneLevel::CleanUp()
{
	LOG("Unloading space scene");

 	App->textures->Unload(graphics);
	App->player->Disable();
	App->bombs->Disable();
	int count = 0;
	for (std::list<Entity*>::iterator it = Entities.begin(); it != Entities.end(); ++it) {
		count++;
		//LOG("%d",count);
		RELEASE(*it);
	}
	Entities.clear();
	//App->collision->Disable();
	//App->particles->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneLevel::PreUpdate()
{
	//Draw hud background color
	App->renderer->DrawQuad(hud, 136, 136, 136, 255, true);
	//Draw background floor color
	App->renderer->DrawQuad(background, 64, 120, 16, 255, true);
	return UPDATE_CONTINUE;
}

update_status ModuleSceneLevel::Update()
{
	// Draw everything --------------------------------------
	for (std::list<Entity*>::iterator it = Entities.begin(); it != Entities.end(); ++it) {
		(*it)->Draw();
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneLevel::CreateExternalBlocks()
{
	//Left
	for (int i = 0; i < 11; i++) {
		Block *obj = new Block(iPoint(0, 50 + (50 * i)), true, graphics, externalBlock);
		Entities.push_back(obj);
	}
	//Up
	for (int i = 1; i <= 14; i++) {
		Block *obj = new Block(iPoint(0 + (50 * i), 50), true, graphics, externalBlock);
		Entities.push_back(obj);
	}
	//Right
	for (int i = 1; i <= 10; i++) {
		Block *obj = new Block(iPoint(700, 50 + (50 * i)), true, graphics, externalBlock);
		Entities.push_back(obj);
	}
	//Down
	for (int i = 1; i <= 13; i++) {
		Block *obj = new Block(iPoint(0 + (50 * i), 550), true, graphics, externalBlock);
		Entities.push_back(obj);
	}
}

void ModuleSceneLevel::CreateBlocks()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			Block *obj = new Block(iPoint(100 + (100 * j), 150 + (100 * i)), false, graphics, block);
			Entities.push_back(obj);
		}
	}
}

void ModuleSceneLevel::CreateBricks()
{
	//SDL_Rect dest;
	Brick* obj = new Brick(iPoint(150, 100), graphics, brick);
	Entities.push_back(obj);
	obj = new Brick(iPoint(200, 100), graphics, brick);
	Entities.push_back(obj);
}
