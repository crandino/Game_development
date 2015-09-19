// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#include "String.h"

class Application;

class Module
{
public:

	Module() : active(false)
	{}

	void init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool awake()
	{
		return true;
	}

	// Called before the first frame
	virtual bool start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool preUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool postUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool cleanUp()
	{
		return true;
	}

public:

	String		name;
	bool		active;

};

#endif // __MODULE_H__