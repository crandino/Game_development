#include "p2Defs.h"
#include "p2Log.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "FileSystem.h"
#include "App.h"

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	fs = new FileSystem("data.zip");

	// Ordered for awake / start / update
	// Reverse order of cleanUp
	addModule(fs);
	addModule(input);
	addModule(win);
	addModule(tex);
	addModule(audio);
	addModule(scene);

	// render last to swap buffer
	addModule(render);
}

// Destructor
App::~App()
{
	// release modules
	doubleNode<Module*>* item = modules.getLast();

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->previous;
	}

	modules.clear();

	config_file.reset();
}

void App::addModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::awake()
{
	bool ret = true;

	// --- load config file ---
	char* buf;
	int size = app->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
		config = config_file.child("config");
	// ---

	doubleNode<Module*>* item;
	item = modules.getFirst();

	pugi::xml_node module_node;

	while(item != NULL && ret == true)
	{
		// TODO 1: Every awake to receive a xml node with their section of the config file if exists

		module_node = config.child(item->data->name.GetString());

		ret = item->data->awake();
		item = item->next;
	}

	return ret;
}

// Called before the first frame
bool App::start()
{
	bool ret = true;
	doubleNode<Module*>* item;
	item = modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::update()
{
	bool ret = true;
	prepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
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
void App::prepareUpdate()
{
}

// ---------------------------------------------
void App::finishUpdate()
{
}

// Call modules before each loop iteration
bool App::preUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item;
	item = modules.getFirst();
	Module* pModule = NULL;

	for(item = modules.getFirst(); item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->preUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::doUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item;
	item = modules.getFirst();
	Module* pModule = NULL;

	for(item = modules.getFirst(); item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::postUpdate()
{
	bool ret = true;
	doubleNode<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.getFirst(); item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->postUpdate();
	}

	return ret;
}

// Called before quitting
bool App::cleanUp()
{
	bool ret = true;
	doubleNode<Module*>* item;
	item = modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->cleanUp();
		item = item->previous;
	}

	return ret;
}

// ---------------------------------------
int App::gerArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::gerArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}