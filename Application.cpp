#include "Application.h"
#include "Timer.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"

#include "ModuleSceneMenu.h"
#include "ModuleSceneLevel.h"
#include "ModuleSceneResult.h"
#include "ModulePlayer.h"
#include "ModuleBomb.h"
#include "ModuleGUI.h"

#include "SDL/include/SDL_ttf.h"

using namespace std;

Application::Application()
{
	//Initialize SDL_ttf
	TTF_Init();

	//FPS
	fpsTimer = new Timer();

	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	// Game Modules
	modules.push_back(scene_result = new ModuleSceneResult(false));
	modules.push_back(scene_level = new ModuleSceneLevel(false));
	modules.push_back(scene_menu = new ModuleSceneMenu(false));
	modules.push_back(gui = new ModuleGUI(false));
	
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(bombs = new ModuleBomb(false));
	

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	fade->FadeToBlack(scene_result, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	//Frames
	fpsTimer->Start();

	//Updates
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	//Frames
	//LOG("FPS are: %f", 1/fpsTimer->EllapsedInSeconds());
	fpsTimer->Stop();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it) {
		if ((*it)->IsEnabled() == true) {
			ret = (*it)->CleanUp();
		}
	}

	RELEASE(fpsTimer);

	TTF_Quit();

	return ret;
}

