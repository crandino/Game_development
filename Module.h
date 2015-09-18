#ifndef __MODULE_H__
#define __MODULE_H__

#include "String.h"

class Module
{
public:

	bool active;
	String name;

	Module(): active(false)
	{ }

	void init()
	{
		active = true;
	}

	virtual bool awake()
	{
		return true;
	}

	virtual bool start()
	{
		return true;
	}

	virtual bool preUpdate()
	{
		return true;
	}

	virtual bool update(float dt)
	{
		return true;
	}

	virtual bool postUpdate()
	{
		return true;
	}

	virtual bool cleanUp()
	{
		return true;
	}

};

#endif !// __MODULE_H__