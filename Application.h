#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "DList.h"

class Module;

class Application
{
public:

	int frames;

	Application(int argc, char** argv) : argc(argc), argv(argv)
	{ }

	virtual ~Application()
	{ }

	bool awake()
	{
		return true;
	}

	bool start()
	{
		return true;
	}

	bool update()
	{
		return true;
	}

	bool clean()
	{
		return true;
	}


private:

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

	/*void addModule(Module *module)
	{
		module->init();
		modules.add(module);
	}*/



};

#endif //!__APPLICATION_H__