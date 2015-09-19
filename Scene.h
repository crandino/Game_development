#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called before all Updates
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called before all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__