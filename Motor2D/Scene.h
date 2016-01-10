#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
class UIlabel;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called before all updates
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called before all updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	void Scene::onGui(MOUSE_EVENTS mouse_event, UIelement *trigger);

private:

	// EXERCISE 5
	UIlabel *counter;

	float player_x;
	float player_y;
	SDL_Texture* debug_tex;

	bool path_selected;
	iPoint origin;
	iPoint destination;

	bool move_locked;

};

#endif // __SCENE_H__
