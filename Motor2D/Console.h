#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Module.h"
#include "DList.h"

struct Command
{
	const char *	name;
	Module			*listener;
	uint			min_args, max_args;

	Command(const char* n, Module* l)
	{
		name = n;
		listener = l;
	}
};


class Console : public Module
{
public:
	
	Console();

	// Destructor
	virtual ~Console();

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

	// Function to check string and 
	bool execute(const char *func_to_execute) const;

	// Function to create new commands
	bool createCommand(const char *name_command, Module *listener);

private:

	DList<Command*> list_of_commands;

};

#endif // __CONSOLE_H__
