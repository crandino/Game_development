#include "Globals.h"
#include "Application.h"
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
bool Scene::awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

/*Called before the first frame
  Carregar fitxers d'aquesta manera és molt dolent. 
  Aquests d'aquí utilitzen path relatius, que és correcte. 
  
  El que es fa aquí es utilitzar Sandbox, és a dir, zones delimitades
  on es permet l'escriptura, per evitar accedir a zones protegides.
  */
bool Scene::start()
{
	img = app->tex->load("game_test/textures/test.png");
	app->audio->playMusic("game_test/audio/music/music_sadpiano.ogg");
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
	app->render->blit(img, 0, 0);
	return true;
}

// Called each loop iteration
bool Scene::postUpdate()
{
	bool ret = true;

	if(app->input->getKeyDown(SDLK_ESCAPE) == true)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::cleanUp()
{
	LOG("Freeing scene");

	return true;
}
