#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "PathFinding.h"
#include "Render.h"
#include "Gui.h"
#include "Window.h"
#include "Scene.h"
#include "Maps.h"
#include "UIelements.h"

#define FRAME_LOGIC 60
#define MS_PER_LOGIC_FRAME 1000.0f/FRAME_LOGIC

Scene::Scene() : Module()
{
	name.create("scene");
	move_locked = false;
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

	// ----- UI elements creation -----
	// Common font
	_TTF_Font *UIfont = app->fonts->load("fonts/open_sans/OpenSans-Regular.ttf", 14);
	// Main Window and title
	SDL_Rect win_rect = {32, 542, 420, 452 };
	const UIimage *w = app->gui->createImage({ 200, 100 }, NULL, win_rect, this);
	app->gui->createLabel({ 155, 50 }, "Window Title", UIfont, this, (UIelement*)w);

	//// Button and label of the button
	//SDL_Rect idle_section = { 648, 173, 218, 57 };
	//SDL_Rect hover_section = { 6, 117, 218, 57 };
	//SDL_Rect clicked_section = { 417, 173, 218, 57 };
	//const UIbutton *b = app->gui->createButton({ 120, 300 }, NULL, idle_section, NULL, hover_section, NULL, clicked_section, this, (UIelement*)w);
	//app->gui->createLabel({ 20, 15 }, "Hola, me llamo Carlos", UIfont, this, (UIelement*)b);

	//// Inputbox
	//SDL_Rect frame_section = { 495, 573, 332, 52 };
	//app->gui->createInputBox({ 50, 100 }, { 10, 15 }, NULL, frame_section, "Put text here", UIfont, this, (UIelement*)w);

	//// Inputbox
	//app->gui->createInputBox({ 50, 150 }, { 10, 15 }, NULL, frame_section, "Put text here", UIfont, this, (UIelement*)w);

	// EXERCISE 1 and 6
	SDL_Rect bar_rect = { 0, 11, 307, 11 };
	SDL_Rect thumb_rect = { 805, 318, 26, 15 };
	app->gui->createHorizontalScrollBar({ 50, 100 }, 5, -2, NULL, bar_rect, NULL, thumb_rect, this, (UIelement*)w);

	// EXERCISE 5
	_TTF_Font *UIfont_counter = app->fonts->load("fonts/open_sans/OpenSans-Regular.ttf", 26);
	counter = app->gui->createLabel({ 125, 200 }, "Value: -.--     ", UIfont_counter, this, (UIelement*)w);

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

	/*if (app->input->getKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->loadGame("save_game.xml");

	if (app->input->getKey(SDL_SCANCODE_K) == KEY_DOWN)
		app->saveGame("save_game.xml");*/

	if (!move_locked)
	{
		if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += cam_speed;

		if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= cam_speed;

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += cam_speed;

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= cam_speed;
	}

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
		app->input->getMousePosition(pos);
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
	for (uint i = 0; i < parray->getNumElements(); i++)
	{
		iPoint pos = app->map->mapToWorld((*parray)[i].x, (*parray)[i].y);
		app->render->blit(debug_tex, pos.x, pos.y);
	}
	
	app->input->getMousePosition(pos);
	iPoint map_coordinates = app->map->worldToMap(pos.x - app->render->camera.x, pos.y - app->render->camera.y);
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

void Scene::onGui(MOUSE_EVENTS mouse_event, UIelement *trigger)
{
	switch (trigger->type)
	{
	case UI_INPUTBOX:
	{
		UIinputBox *i = (UIinputBox*)trigger;
		switch (mouse_event)
		{
		case GAIN_FOCUS:
			move_locked = true;
			break;
		case LOST_FOCUS:
			move_locked = false;
			break;
		}
		break;
	}

	// EXERCISE 5
	case UI_HORIZONTALSCROLLBAR:
	{
		UIHorizontalScrollBar *h = (UIHorizontalScrollBar*)trigger;
		switch (mouse_event)
		{
		case GAIN_FOCUS:
			move_locked = true;
			break;
		case LOST_FOCUS:
			move_locked = false;
			break;
		case DRAGGED:
			char c[20];
			sprintf_s(c, 20, "Value: %0.2f", h->calculateValue());
			counter->setText(c);
			break;
		}
		break;
	}
	}

}
