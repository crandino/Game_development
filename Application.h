#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "DList.h"
#include "Module.h"

class Application
{
public:

	int frames;
	float dt;

	Application(int argc, char** argv) : argc(argc), argv(argv)
	{
		frames = 0;
	}

	virtual ~Application()
	{ 
		doubleNode<Module*>* item = modules.getLast();

		while (item != NULL)
		{
			if (item->data != NULL)
			{
				delete item->data;
				item->data = NULL;
			}
			item = item->previous;
		}

		modules.clear();
	}

	bool awake()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getFirst();

		while (item != NULL && ret == true)
		{
			ret = item->data->awake();
			item = item->next;
		}

		return ret;
	}

	bool start()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getFirst();

		while (item != NULL && ret == true)
		{
			ret = item->data->start();
			item = item->next;
		}

		return ret;
	}

	bool update()
	{
		bool ret = true;
		prepareUpdate();

		/*if (input->getWindowEvent(WE_QUIT) == true)
			ret = false;*/

		if (ret == true)
			ret = preUpdate();

		if (ret == true)
			ret = doUpdate();

		if (ret == true)
			ret = postUpdate();

		finishUpdate();
		return ret;
	}

	bool cleanUp()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getLast();

		while (item != NULL && ret == true)
		{
			ret = item->data->cleanUp();
			item = item->previous;
		}

		return ret;
	}

private:

	void prepareUpdate() {}
	void finishUpdate() {}
	
	bool preUpdate()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getFirst();
		Module *pModule = NULL;

		for ( ; item != NULL && ret == true; item = item->next)
		{
			pModule = item->data;

			if (pModule->active == false)
				continue;

			ret = item->data->preUpdate();
		}
		return ret;
	}

	bool doUpdate()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getFirst();
		Module *pModule = NULL;

		for (; item != NULL && ret == true; item = item->next)
		{
			pModule = item->data;

			if (pModule->active == false)
				continue;

			ret = item->data->update(dt);
		}
		return ret;
	}

	bool postUpdate()
	{
		bool ret = true;
		doubleNode<Module*>* item = modules.getFirst();
		Module *pModule = NULL;

		for (; item != NULL && ret == true; item = item->next)
		{
			pModule = item->data;

			if (pModule->active == false)
				continue;

			ret = item->data->postUpdate();
		}
		return ret;
	}
	
	int argc;
	char **argv;
	DList<Module*> modules;

	int getArgc() const
	{
		return argc;
	}

	const char* GetArgv(int index) const
	{
		if (index < argc)
			return argv[index];
		else
			return NULL;
	}

	void addModule(Module *module)
	{
		module->init();
		modules.add(module);
	}
};

extern Application *app;

#endif //!__APPLICATION_H__