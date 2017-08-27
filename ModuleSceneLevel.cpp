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
#include "Explotion.h"
#include "Enemy.h"
#include "Item.h"
#include "ModuleGUI.h"

ModuleSceneLevel::ModuleSceneLevel(bool active) : Module(active)
{}

ModuleSceneLevel::~ModuleSceneLevel()
{}

// Load assets
bool ModuleSceneLevel::Start()
{
	LOG("Loading space scene");

	App->gui->Enable();
	
	graphics = App->textures->Load("Super Bomberman 5 JPN - Stage 01.png");

	//Set up background rect
	background.h = SCREEN_HEIGHT;
	background.w = SCREEN_WIDTH;
	background.x = 0;
	background.y = 50;

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
	CreateEnemies();

	InitializeSquareMatrix();

	return true;
}

// UnLoad assets
bool ModuleSceneLevel::CleanUp()
{
	LOG("Unloading scene");

 	App->textures->Unload(graphics);
	App->player->Disable();
	App->bombs->Disable();
	App->gui->Disable();

	for (std::list<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it) {
		(*it)->CleanUp();
		RELEASE(*it);
	}
	Enemies.clear();
	
	int count = 0;
	for (std::list<Entity*>::iterator it = Entities.begin(); it != Entities.end(); ++it) {
		count++;
		LOG("%d",count);
		if ((*it)->type == Entity::ITEM) 
			LOG("Item");
		(*it)->CleanUp();
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
	//Draw background floor color
	App->renderer->DrawQuad(background, 64, 120, 16, 255, true);
	return UPDATE_CONTINUE;
}

update_status ModuleSceneLevel::Update()
{
	// Draw everything --------------------------------------
	for (std::list<Entity*>::iterator it = Entities.begin(); it != Entities.end(); ++it) {
		if (!(*it)->destroyed) {
			(*it)->Draw();
		}
		else {
			(*it)->CleanUp();
			RELEASE(*it);
			it = Entities.erase(it);
			if (it == Entities.end()) break;
		}
		
	}

	for (std::list<Enemy*>::iterator it = Enemies.begin(); it != Enemies.end(); ++it) {
		if (!(*it)->destroyed) {
			(*it)->Update();
			(*it)->Draw();
		}
		else {
			(*it)->CleanUp();
			RELEASE(*it);
			it = Enemies.erase(it);
			if (it == Enemies.end()) break;
		}
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneLevel::PrintLevelMap()
{
	for (int row = 0; row < 11; row++) {
		LOG("%c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c", levelMap[row][0].type, levelMap[row][1].type,
			levelMap[row][2].type, levelMap[row][3].type, levelMap[row][4].type, levelMap[row][5].type, levelMap[row][6].type,
			levelMap[row][7].type, levelMap[row][8].type, levelMap[row][9].type, levelMap[row][10].type, levelMap[row][11].type,
			levelMap[row][12].type, levelMap[row][13].type, levelMap[row][14].type);
	}
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
	obj->AddLoot(ISPEED);
	Entities.push_back(obj);
	obj = new Brick(iPoint(200, 100), graphics, brick);
	Entities.push_back(obj);
}

void ModuleSceneLevel::CreateEnemies()
{
	Enemy* obj = new Enemy(iPoint(350+10, 100+5),EnemyDirection::HORIZONTALRIGHT);
	Enemies.push_back(obj);
	Item* item = new Item(iPoint(350, 200), IBOMB);
	Entities.push_back(item);
}

void ModuleSceneLevel::InitializeSquareMatrix()
{
	//External Blocks
	int row = 0;
	for (int column = 0; column < 15; column++) {
		levelMap[row][column].type = 'e';
	}
	row = 10;
	for (int column = 0; column < 15; column++) {
		levelMap[row][column].type = 'e';
	}
	int column = 0;
	for (int row = 0; row < 11; row++) {
		levelMap[row][column].type = 'e';
	}
	column = 14;
	for (int row = 0; row < 11; row++) {
		levelMap[row][column].type = 'e';
	}
	//Internal Blocks
	for (row = 2; row < 9; row += 2 ) {
		for (column = 2; column < 13; column += 2) {
			levelMap[row][column].type = 'i';
		}
	}
	//All else will be empty
	for (row = 0; row < 11; row++) {
		for (column = 0; column < 15; column++) {
			if(levelMap[row][column].type == 'n') levelMap[row][column].type = '0';
		}
	}
	//Bricks
	levelMap[1][3].type = 'b';
	levelMap[1][4].type = 'b';
	//Player
	levelMap[2][1].type = '1';

	//Fill position and rects
	for (row = 0; row < 11; row++) {
		for (column = 0; column < 15; column++) {
			levelMap[row][column].position = iPoint(column * 50, 50 + row * 50);
			levelMap[row][column].squareRect = { column * 50, 50 + row * 50, 50, 50 };
		}
	}

	//Print matrix
	//PrintLevelMap();
}

void ModuleSceneLevel::BreakBrick(SDL_Rect position)
{
	for (std::list<Entity*>::iterator it = Entities.begin(); it != Entities.end(); ++it) {
		if ((*it)->type == Entity::EntityType::BRICK) {
			Brick* aux = (Brick*)(*it);
			if (SDL_RectEquals(&aux->spriteDest, &position) == SDL_TRUE) {
				LOG("Romper el ladrillo (%d, %d, %d, %d)", aux->spriteDest.x, aux->spriteDest.y, aux->spriteDest.w, aux->spriteDest.h);
				Item* item = aux->Loot();
				if (item != nullptr) {
					Entities.push_back(item);
				}
				aux->CleanUp();
				EraseBrickFromMapLevel(position.x, position.y);
				RELEASE(aux);
				it = Entities.erase(it);
				break;
				//if (it == Entities.end()) break;
			}
		}
	}
}

void ModuleSceneLevel::AddBombToMapLevel(int x, int y)
{
	//Add bomb from map
	for (int row = 0; row < 11; row++) {
		for (int column = 0; column < 15; column++) {
			if (levelMap[row][column].position.x == x && levelMap[row][column].position.y == y) levelMap[row][column].type = 'o';
		}
	}
	//Print matrix
	//PrintLevelMap();
}

std::list<ExplotionInstance> ModuleSceneLevel::AddExplotionToMapLevel(int x, int y, int flameLevel)
{
	std::list<ExplotionInstance> ex;
	
	//Remove bomb from map
	int row;
	int column;
	SDL_Rect center;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 15; j++) {
			if (levelMap[i][j].position.x == x && levelMap[i][j].position.y == y) {
				row = i;
				column = j;
				center = levelMap[i][j].squareRect;
				levelMap[i][j].type = '0';
			}
		}
	}
	//Calculate explotion
	bool* up = new bool[flameLevel];
	bool* down = new bool[flameLevel];
	bool* right = new bool[flameLevel];
	bool* left = new bool[flameLevel];
	bool upShort = false;
	bool downShort = false;
	bool rightShort = false;
	bool leftShort = false;
	std::list<ExplotionInstance> exUp;
	std::list<ExplotionInstance> exDown;
	std::list<ExplotionInstance> exRight;
	std::list<ExplotionInstance> exLeft;

	for (int i = 0; i < flameLevel; i++) {
		//Inicialization
		up[i] = false;
		down[i] = false;
		right[i] = false;
		left[i] = false;
		
		//Up
		if ((row - i) != 0 && (levelMap[row - (i+1)][column].type == '0' || levelMap[row - (i+1)][column].type == '1' || levelMap[row - (i + 1)][column].type == 'e'
			|| levelMap[row - (i + 1)][column].type == 'b') && !upShort) {
			up[i] = true;
			if (levelMap[row - (i + 1)][column].type != 'e' || levelMap[row - (i + 1)][column].type == 'b') {
				if (levelMap[row - (i + 1)][column].type == 'b') {
					BreakBrick(levelMap[row - (i + 1)][column].squareRect);
					upShort = true;
				}
				ExplotionInstance aux;
				aux.type = TWOWAY;
				aux.position = levelMap[row - (i + 1)][column].squareRect;
				aux.rotation = -90;
				aux.flipType = SDL_FLIP_NONE;
				exUp.push_back(aux);
			}
		}
		else {
			upShort = true;
		}

		//Down
		if ((row + i) != 10 && (levelMap[row + (i + 1)][column].type == '0' || levelMap[row + (i + 1)][column].type == '1' || levelMap[row + (i + 1)][column].type == 'e'
			|| levelMap[row + (i + 1)][column].type == 'b') && !downShort) {
			down[i] = true;
			if (levelMap[row + (i + 1)][column].type != 'e' || levelMap[row + (i + 1)][column].type == 'b') {
				if (levelMap[row + (i + 1)][column].type == 'b') {
					BreakBrick(levelMap[row + (i + 1)][column].squareRect);
					downShort = true;
				}
				ExplotionInstance aux;
				aux.type = TWOWAY;
				aux.position = levelMap[row + (i + 1)][column].squareRect;
				aux.rotation = 90;
				aux.flipType = SDL_FLIP_NONE;
				exDown.push_back(aux);
			}
		}
		else {
			downShort = true;
		}

		//Right
		if ((column + i) != 14 && (levelMap[row][column + (i + 1)].type == '0' || levelMap[row][column + (i + 1)].type == '1' || levelMap[row][column + (i + 1)].type == 'e'
			|| levelMap[row][column + (i + 1)].type == 'b') && !rightShort) {
			right[i] = true;
			if (levelMap[row][column + (i + 1)].type != 'e' || levelMap[row][column + (i + 1)].type == 'b') {
				if (levelMap[row][column + (i + 1)].type == 'b') {
					BreakBrick(levelMap[row][column + (i + 1)].squareRect);
					rightShort = true;
				}
				ExplotionInstance aux;
				aux.type = TWOWAY;
				aux.position = levelMap[row][column + (i + 1)].squareRect;
				aux.rotation = 0;
				aux.flipType = SDL_FLIP_NONE;
				exRight.push_back(aux);
			}
		}
		else {
			rightShort = true;
		}

		//Left
		if ((column - i) != 0 && (levelMap[row][column - (i + 1)].type == '0' || levelMap[row][column - (i + 1)].type == '1' || levelMap[row][column - (i + 1)].type == 'e'
			|| levelMap[row][column - (i + 1)].type == 'b') && !leftShort) {
			left[i] = true;
			if (levelMap[row][column - (i + 1)].type != 'e' || levelMap[row][column - (i + 1)].type == 'b') {
				if (levelMap[row][column - (i + 1)].type == 'b') {
					BreakBrick(levelMap[row][column - (i + 1)].squareRect);
					leftShort = true;
				}
				ExplotionInstance aux;
				aux.type = TWOWAY;
				aux.position = levelMap[row][column - (i + 1)].squareRect;
				aux.rotation = 0;
				aux.flipType = SDL_FLIP_HORIZONTAL;
				exLeft.push_back(aux);
			}
		}
		else {
			leftShort = true;
		}
	
	}

	ExplotionInstance central;
	central.position = center;
	central.type = FOURWAY;
	central.rotation = 0;
	central.flipType = SDL_FLIP_NONE;
	

	if ((exUp.size() == 0 && exDown.size() == 0) || (exRight.size() == 0 && exLeft.size() == 0)) {
		central.type = TWOWAY;
		if (exUp.size() == 0 && exDown.size() == 0) {
			central.rotation = 0;
		}
		else {
			central.rotation = 90;
		}
		central.flipType = SDL_FLIP_NONE;
	}
	ex.push_back(central);

	if (!exUp.empty())exUp.rbegin()->type = ENDING;
	if (!exDown.empty())exDown.rbegin()->type = ENDING;
	if (!exRight.empty()) exRight.rbegin()->type = ENDING;
	if (!exLeft.empty())exLeft.rbegin()->type = ENDING;

	//Add all explotions
	for (std::list<ExplotionInstance>::iterator it = exUp.begin(); it != exUp.end(); ++it) {
		ex.push_back(*it);
	}
	for (std::list<ExplotionInstance>::iterator it = exDown.begin(); it != exDown.end(); ++it) {
		ex.push_back(*it);
	}
	for (std::list<ExplotionInstance>::iterator it = exRight.begin(); it != exRight.end(); ++it) {
		ex.push_back(*it);
	}
	for (std::list<ExplotionInstance>::iterator it = exLeft.begin(); it != exLeft.end(); ++it) {
		ex.push_back(*it);
	}

	//Create explotions
	for (list<ExplotionInstance>::iterator it = ex.begin(); it != ex.end(); ++it) {
		App->bombs->AddExplotion((it)->position, (*it));
	}
	
	RELEASE(up);
	RELEASE(down);
	RELEASE(right);
	RELEASE(left);

	//Print matrix
	//PrintLevelMap();

	return ex;
}

void ModuleSceneLevel::EraseBrickFromMapLevel(int x, int y)
{
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 15; j++) {
			if (levelMap[i][j].position.x == x && levelMap[i][j].position.y == y && levelMap[i][j].type == 'b') {
				levelMap[i][j].type = '0';
			}
		}
	}
	//PrintLevelMap();
}
