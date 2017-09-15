#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;

// Game modules ---
class ModulePlayer;
class ModuleSceneMenu;
class ModuleSceneLevel;
class ModuleSceneResult;
class ModuleBomb;
class ModuleGUI;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneMenu* scene_menu;
	ModuleSceneLevel* scene_level;
	ModuleSceneResult* scene_result;
	ModuleBomb* bombs;
	ModuleGUI* gui;

	//FPS
	Timer* fpsTimer;

	//Level
	int currentLevel = 0;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__