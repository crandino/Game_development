#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "DList.h"
#include "Module.h"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class FileSystem;
class Scene;

class Application
{
public:

	// Constructor
	Application(int argc, char* args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool update();

	// Called before quitting
	bool cleanUp();

	// Add a new module to handle
	void addModule(Module* module);

	int getArgc() const;
	const char* getArgv(int index) const;

private:

	// Call modules before each loop iteration
	void prepareUpdate();

	// Call modules before each loop iteration
	void finishUpdate();

	// Call modules before each loop iteration
	bool preUpdate();

	// Call modules on each loop iteration
	bool doUpdate();

	// Call modules after each loop iteration
	bool postUpdate();

public:

	unsigned int		frames;
	float				dt;

	// Modules
	Window*				win;
	Input*				input;
	Render*				render;
	Textures*			tex;
	Audio*				audio;
	Scene*				scene;
	FileSystem*		    fsystem;

private:

	DList<Module*>  	modules;
	int					argc;
	char**				args;
};

extern Application* app;

#endif