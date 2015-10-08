#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::awake(pugi::xml_node &node)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::start()
{
	img = app->tex->Load("textures/test.png");
	app->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	return true;
}

// Called each loop iteration
bool Scene::preUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::update(float dt)
{
	app->render->Blit(img, 0, 0);
	return true;
}

// Called each loop iteration
bool Scene::postUpdate()
{
	bool ret = true;

	if(app->input->GetKeyDown(SDLK_ESCAPE) == true)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::cleanUp()
{
	LOG("Freeing scene");

	return true;
}
