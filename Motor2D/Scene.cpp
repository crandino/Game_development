#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "PathFinding.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Maps.h"

#define FRAME_LOGIC 60
#define MS_PER_LOGIC_FRAME 1000.0f/FRAME_LOGIC

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
	path_selected = false;

	//app->map->load("far_west.tmx");
	//app->map->load("iso.tmx");
	app->map->load("navigation_map.tmx");
	uchar *buffer = NULL;
	int width, height;
	if (app->map->createWalkabilityMap(width, height, &buffer))
		app->path->setMap(width, height, buffer);

	debug_tex = app->tex->loadTexture("textures/path.png");
	player_x = player_y = 10;
	origin.set(0, 0);
	destination.set(0, 0);

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
	float cam_speed = ceil(25.0f * (dt/MS_PER_LOGIC_FRAME));

	if (app->input->getKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->loadGame("save_game.xml");

	if (app->input->getKey(SDL_SCANCODE_K) == KEY_DOWN)
		app->saveGame("save_game.xml");

	if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= cam_speed;

	if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += cam_speed;

	if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += cam_speed;

	if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= cam_speed;

	if (app->input->getKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		player_x--; player_y--;
	}	

	if (app->input->getKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		player_x++; player_y++;
	}

	if (app->input->getKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player_x++; player_y--;
	}

	if (app->input->getKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player_x--; player_y++;
	}

	if (app->input->getKey(SDL_SCANCODE_KP_PLUS) == KEY_UP)
		app->audio->volumeUp();

	if (app->input->getKey(SDL_SCANCODE_KP_MINUS) == KEY_UP)
		app->audio->volumeDown();
	
	app->map->draw();

	iPoint pos;
	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		app->input->getMousePosition(pos.x, pos.y);
		if (path_selected == false)
		{
			origin = app->render->screenToWorld(pos.x, pos.y);
			path_selected = true;
		}
		else
		{
			destination = app->render->screenToWorld(pos.x, pos.y);
			app->path->createPath(app->map->worldToMap(origin.x, origin.y), app->map->worldToMap(destination.x, destination.y));
			path_selected = false;
		}
	}

	iPoint p;
	p = app->map->worldToMap(origin.x, origin.y);
	p = app->map->mapToWorld(p.x, p.y);
	app->render->blit(debug_tex, p.x, p.y);
	p = app->map->worldToMap(destination.x, destination.y);
	p = app->map->mapToWorld(p.x, p.y);
	app->render->blit(debug_tex, p.x, p.y);

	const DynArray<iPoint> *parray = app->path->getLastPath();
	for (int i = 0; i < parray->getNumElements(); i++)
	{
		iPoint pos = app->map->mapToWorld((*parray)[i].x, (*parray)[i].y);
		app->render->blit(debug_tex, pos.x, pos.y);
	}
	
	int x, y;
	app->input->getMousePosition(x, y);
	iPoint map_coordinates = app->map->worldToMap(x - app->render->camera.x, y - app->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Walkable: %d",
		app->map->data.width, app->map->data.height,
		app->map->data.tile_width, app->map->data.tile_height,
		app->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y,
		app->path->isWalkable(map_coordinates));

	//app->win->setTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::postUpdate()
{
	bool ret = true;

	if(app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::cleanUp()
{
	LOG("Freeing scene");

	return true;
}
