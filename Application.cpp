#include "Globals.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
//#include "FileSystem.h"

#include "Application.h"

// Constructor
Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	//fsystem = new FileSystem();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	//addModule(fsystem);
	addModule(input);
	addModule(win);
	addModule(tex);
	addModule(audio);
	addModule(scene);

	// render last to swap buffer
	addModule(render);
}

// Destructor
Application::~Application()
{
	// release modules
	doubleNode<Module*>* item = modules.getLast();

	while(item != NULL)
	{
		if (item->data != NULL)
		{	
			delete item->data;
			item->data = NULL;
		};
		item = item->previous;
	}

	modules.clear();
}

void Application::addModule(Module* module)
{
	module->init();
	modules.add(module);
}

// Called before render is available
bool Application::awake()
{
	bool ret = true;

	doubleNode<Module*>* item = modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->awake();
		item = item->next;
	}

	return ret;
}

// Called before the first frame
bool Application::start()
{
	bool ret = true;
	doubleNode<Module*>* item = modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool Application::update()
{
	bool ret = true;
	prepareUpdate();

	if(input->getWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = preUpdate();

	if(ret == true)
		ret = doUpdate();

	if(ret == true)
		ret = postUpdate();

	finishUpdate();
	return ret;
}

// ---------------------------------------------
void Application::prepareUpdate()
{
}

// ---------------------------------------------
void Application::finishUpdate()
{
}

// Call modules before each loop iteration
bool Application::preUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item = modules.getFirst();
	Module* pModule = NULL;

	for(; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
			continue;

		ret = item->data->preUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool Application::doUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item = modules.getFirst();
	Module* pModule = NULL;

	for(; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
			continue;

		ret = item->data->update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool Application::postUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item = modules.getFirst();
	Module* pModule = NULL;

	for(; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
			continue;

		ret = item->data->postUpdate();
	}

	return ret;
}

// Called before quitting
bool Application::cleanUp()
{
	bool ret = true;
	doubleNode<Module*>* item = modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->cleanUp();
		item = item->previous;
	}

	return ret;
}

// ---------------------------------------
int Application::getArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Application::getArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}