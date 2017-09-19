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
#include "ModuleSceneMenu.h"
#include "ModuleSceneResult.h"
#include "ModuleFadeToBlack.h"
#include "Door.h"
#include "ModuleInput.h"
#include "Player.h"
#include "TrapSquare.h"

ModuleSceneLevel::ModuleSceneLevel(bool active) : Module(active)
{
}

ModuleSceneLevel::~ModuleSceneLevel()
{}

// Load assets
bool ModuleSceneLevel::Start()
{
	LOG("Loading level scene");

	App->scene_menu->Disable();

	App->gui->Enable();
	
	graphics = App->textures->Load("Sprites/Super Bomberman 5 JPN - Stage 01.png");

	if (App->startFx == 0) App->startFx = App->audio->LoadFx("Audio/game_start.ogg");
	App->audio->PlayFx(App->startFx);

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

	if (App->currentLevel == 1) {
		App->audio->PlayMusic("Audio/Area 1.ogg", 1.0f);
	}
	else {
		App->audio->PlayMusic("Audio/Area 2.ogg", 1.0f);
	}
	
	
	//create some colliders for the walls
	InitializeLevelsList();
	CreateExternalBlocks();
	CreateBlocks();
	CreateDoor();
	CreateTrapSquares();
	CreateBricks();
	CreateEnemies();

	InitializeSquareMatrix();

	return true;
}

// UnLoad assets
bool ModuleSceneLevel::CleanUp()
{
	LOG("Unloading scene");

	App->audio->StopMusic();

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
	/*if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		GoToResult(true, App->gui->player1Points);
	}*/
	
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

	//Level Dependent
	if (App->currentLevel == 1) {
		for (std::list<iPoint>::iterator it = internalBlocksLevel1.begin(); it != internalBlocksLevel1.end(); ++it)
		{
			Block *obj = new Block(*it, true, graphics, block);
			Entities.push_back(obj);
		}
		
	}
	else {
		for (std::list<iPoint>::iterator it = internalBlocksLevel2.begin(); it != internalBlocksLevel2.end(); ++it)
		{
			Block *obj = new Block(*it, true, graphics, block);
			Entities.push_back(obj);
		}
	}
}

void ModuleSceneLevel::CreateDoor()
{
	if (App->currentLevel == 1) {
		Door* door = new Door(iPoint(510, 510));
		Entities.push_back(door);
	}
	else {
		Door* door = new Door(iPoint(60, 360));
		Entities.push_back(door);
	}
}

void ModuleSceneLevel::CreateTrapSquares()
{
	if (App->currentLevel == 1) {
		TrapSquare* trap = new TrapSquare(iPoint(450, 400));
		Entities.push_back(trap);
	}
	else {
		TrapSquare* trap = new TrapSquare(iPoint(300, 100));
		Entities.push_back(trap);
		trap = new TrapSquare(iPoint(150, 250));
		Entities.push_back(trap);
		trap = new TrapSquare(iPoint(650, 250));
		Entities.push_back(trap);
	}
}

void ModuleSceneLevel::CreateBricks()
{
	if (App->currentLevel == 1) {
		for (std::list<BrickInfo>::iterator it = bricksLevel1.begin(); it != bricksLevel1.end(); ++it)
		{
			Brick *obj = new Brick(it->position, graphics, brick);
			if(it->lootType != ItemType::NONE) obj->AddLoot(it->lootType);
			Entities.push_back(obj);
		}

	}
	else {
		for (std::list<BrickInfo>::iterator it = bricksLevel2.begin(); it != bricksLevel2.end(); ++it)
		{
			Brick *obj = new Brick(it->position, graphics, brick);
			if (it->lootType != ItemType::NONE) obj->AddLoot(it->lootType);
			Entities.push_back(obj);
		}
	}
	
}

void ModuleSceneLevel::CreateEnemies()
{
	if (App->currentLevel == 1) {
		Enemy* obj = new Enemy(iPoint(50 + 10, 400 + 5), EnemyDirection::HORIZONTALRIGHT);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(150 + 10, 200 + 5), EnemyDirection::HORIZONTALRIGHT);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(450 + 10, 100 + 5), EnemyDirection::VERTICALDOWN);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(200 + 10, 500 + 5), EnemyDirection::HORIZONTALRIGHT);
		Enemies.push_back(obj);
	}
	else {
		Enemy* obj = new Enemy(iPoint(250 + 10, 100 + 5), EnemyDirection::VERTICALDOWN);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(350 + 10, 200 + 5), EnemyDirection::VERTICALUP);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(150 + 10, 300 + 5), EnemyDirection::VERTICALDOWN);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(550 + 10, 200 + 5), EnemyDirection::HORIZONTALRIGHT);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(300 + 10, 500 + 5), EnemyDirection::HORIZONTALRIGHT);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(350 + 10, 300 + 5), EnemyDirection::VERTICALDOWN);
		Enemies.push_back(obj);
		obj = new Enemy(iPoint(550 + 10, 300 + 5), EnemyDirection::VERTICALUP);
		Enemies.push_back(obj);
	}
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
	//Bricks and extra blocks
	if (App->currentLevel == 1) {
		levelMap[2][7].type = 'i';
		levelMap[3][2].type = 'i';
		levelMap[3][6].type = 'i';
		levelMap[9][1].type = 'i';
		levelMap[9][8].type = 'i';
		levelMap[9][9].type = 'i';
		levelMap[9][12].type = 'i';
		levelMap[5][11].type = 'i';

		levelMap[1][3].type = 'b';
		levelMap[1][7].type = 'b';
		levelMap[1][8].type = 'b';
		levelMap[1][10].type = 'b';
		levelMap[1][12].type = 'b';
		levelMap[1][13].type = 'b';
		levelMap[2][3].type = 'b';
		levelMap[2][11].type = 'b';
		levelMap[3][5].type = 'b';
		levelMap[3][7].type = 'b';
		levelMap[3][11].type = 'b';
		levelMap[3][13].type = 'b';
		levelMap[4][3].type = 'b';
		levelMap[4][9].type = 'b';
		levelMap[5][1].type = 'b';
		levelMap[5][6].type = 'b';
		levelMap[5][10].type = 'b';
		levelMap[5][12].type = 'b';
		levelMap[5][13].type = 'b';
		levelMap[6][1].type = 'b';
		levelMap[6][5].type = 'b';
		levelMap[6][9].type = 'b';
		levelMap[6][13].type = 'b';
		levelMap[7][7].type = 'b';
		levelMap[7][9].type = 'b';
		levelMap[7][13].type = 'b';
		levelMap[8][1].type = 'b';
		levelMap[8][3].type = 'b';
		levelMap[8][7].type = 'b';
		levelMap[8][9].type = 'b';
		levelMap[8][11].type = 'b';
		levelMap[9][2].type = 'b';
		levelMap[9][3].type = 'b';
		levelMap[9][7].type = 'b';
		levelMap[9][10].type = 'b';
		levelMap[9][13].type = 'b';
	}
	else
	{
		levelMap[2][3].type = 'i';
		levelMap[3][2].type = 'i';
		levelMap[3][9].type = 'i';
		levelMap[4][11].type = 'i';
		levelMap[5][1].type = 'i';
		levelMap[6][13].type = 'i';
		levelMap[8][1].type = 'i';
		levelMap[9][4].type = 'i';

		levelMap[1][3].type = 'b';
		levelMap[1][4].type = 'b';
		levelMap[1][6].type = 'b';
		levelMap[1][9].type = 'b';
		levelMap[1][10].type = 'b';
		levelMap[1][13].type = 'b';
		levelMap[2][9].type = 'b';
		levelMap[3][4].type = 'b';
		levelMap[3][8].type = 'b';
		levelMap[3][10].type = 'b';
		levelMap[4][3].type = 'b';
		levelMap[4][5].type = 'b';
		levelMap[4][7].type = 'b'; 
		levelMap[4][13].type = 'b';
		levelMap[5][2].type = 'b';
		levelMap[5][4].type = 'b';
		levelMap[5][5].type = 'b';
		levelMap[5][6].type = 'b';
		levelMap[5][9].type = 'b';
		levelMap[6][1].type = 'b';
		levelMap[6][5].type = 'b';
		levelMap[7][1].type = 'b';
		levelMap[7][6].type = 'b';
		levelMap[7][8].type = 'b';
		levelMap[7][12].type = 'b';
		levelMap[7][13].type = 'b';
		levelMap[8][5].type = 'b';
		levelMap[8][11].type = 'b';
		levelMap[9][1].type = 'b';
		levelMap[9][2].type = 'b';
		levelMap[9][5].type = 'b';
		levelMap[9][12].type = 'b';
		levelMap[9][13].type = 'b';

	}
	//Player
	levelMap[1][1].type = '1';

	//Fill position and rects
	for (row = 0; row < 11; row++) {
		for (column = 0; column < 15; column++) {
			levelMap[row][column].position = iPoint(column * 50, 50 + row * 50);
			levelMap[row][column].squareRect = { column * 50, 50 + row * 50, 50, 50 };
		}
	}

	//Print matrix
	PrintLevelMap();
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

std::list<ExplotionInstance> ModuleSceneLevel::AddExplotionToMapLevel(int x, int y, int flameLevel, int idPlayer)
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
					App->gui->ChangePlayerPoints(100, idPlayer);
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
					App->gui->ChangePlayerPoints(100, idPlayer);
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
					App->gui->ChangePlayerPoints(100, idPlayer);
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
					App->gui->ChangePlayerPoints(100, idPlayer);
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
		App->bombs->AddExplotion((it)->position, (*it), idPlayer);
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

void ModuleSceneLevel::BackToMenu()
{
	App->scene_menu->Enable();
	//CleanUp();
	//App->collision->CleanUp();
	//App->scene_level->Disable();
	App->fade->FadeToBlack((Module*)App->scene_menu, this);
	App->scene_menu->BackToMenu();
	
	
}

void ModuleSceneLevel::GoToResult(bool result, int points)
{
	App->scene_result->Enable();
	App->scene_result->SetData(result, points);
	App->scene_result->SetExtraData(App->player->player1->meters, App->player->player1->bombsPlanted);
	App->fade->FadeToBlack((Module*)App->scene_result, this);
	
	//App->scene_menu->BackToMenu();


}

int ModuleSceneLevel::CheckEnemiesLeft()
{
	return Enemies.size();
}

void ModuleSceneLevel::InitializeLevelsList()
{
	internalBlocksLevel1.clear();
	internalBlocksLevel1.push_back(iPoint(350, 150));
	internalBlocksLevel1.push_back(iPoint(100, 200));
	internalBlocksLevel1.push_back(iPoint(300, 200));
	internalBlocksLevel1.push_back(iPoint(550, 300));
	internalBlocksLevel1.push_back(iPoint(50, 500));
	internalBlocksLevel1.push_back(iPoint(400, 500));
	internalBlocksLevel1.push_back(iPoint(450, 500));
	internalBlocksLevel1.push_back(iPoint(600, 500));
	
	internalBlocksLevel2.clear();
	internalBlocksLevel2.push_back(iPoint(150, 150));
	internalBlocksLevel2.push_back(iPoint(450, 200));
	internalBlocksLevel2.push_back(iPoint(100, 200));
	internalBlocksLevel2.push_back(iPoint(550, 250));
	internalBlocksLevel2.push_back(iPoint(650, 350));
	internalBlocksLevel2.push_back(iPoint(50, 300));
	internalBlocksLevel2.push_back(iPoint(50, 450));
	internalBlocksLevel2.push_back(iPoint(200, 500));

	/*
	Level 1
	150,100 speed
	350,200 fire
	150,250 bomb
	650,500 life


	Level2
	450,300 kill
	200,200 bomb
	450,150 fire
	650,100 speed
	650,400 life
	*/

	BrickInfo aux;
	bricksLevel1.clear();
	aux.position = iPoint(150, 100); aux.lootType = ItemType::IFLAME;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(350, 100); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(400, 100); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(500, 100); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(600, 100); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 100); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(150, 150); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(550, 150); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(50, 300); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(300, 300); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(500, 300); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(600, 300); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 300); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(100, 500); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(150, 500); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(350, 500); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(500, 500); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 500); aux.lootType = ItemType::ILIFE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(250, 200); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(350, 200); aux.lootType = ItemType::ISPEED;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(550, 200); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 200); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(150, 250); aux.lootType = ItemType::IBOMB;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(450, 250); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(50, 350); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(250, 350); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(450, 350); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 350); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(350, 400); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(450, 400); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(650, 400); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(50, 450); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(150, 450); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(350, 450); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(450, 450); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);
	aux.position = iPoint(550, 450); aux.lootType = ItemType::NONE;
	bricksLevel1.push_back(aux);


	bricksLevel2.clear();
	aux.position = iPoint(150, 100); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(200, 100); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(300, 100); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(450, 100); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(500, 100); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(650, 100); aux.lootType = ItemType::ISPEED;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(450, 150); aux.lootType = ItemType::IFLAME;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(200, 200); aux.lootType = ItemType::IBOMB;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(400, 200); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(500, 200); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(150, 250); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(250, 250); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(350, 250); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(650, 250); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(100, 300); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(250, 300); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(450, 300); aux.lootType = ItemType::IKILL;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(50, 350); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(250, 350); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(50, 400); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(200, 300); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(300, 300); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(300, 400); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(400, 400); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(600, 400); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(650, 400); aux.lootType = ItemType::ILIFE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(250, 450); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(550, 450); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(50, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(100, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(250, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(5000, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(600, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);
	aux.position = iPoint(650, 500); aux.lootType = ItemType::NONE;
	bricksLevel2.push_back(aux);

}