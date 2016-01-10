
#include "Console.h"
#include "p2Log.h"

Console::Console() : Module()
{
	name.create("console");
}

// Destructor
Console::~Console()
{ }

// Called before render is available
bool Console::awake(pugi::xml_node &node)
{
	LOG("Loading Console");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Console::start()
{
	return true;
}

// Called each loop iteration
bool Console::preUpdate()
{
	return true;
}

// Called each loop iteration
bool Console::update(float dt)
{
	return true;
}

// Called each loop iteration
bool Console::postUpdate()
{
	return true;
}

// Called before quitting
bool Console::cleanUp()
{
	LOG("Freeing console");
	return true;
}

bool Console::execute(const char *func_to_execute) const
{
	return true;
}

bool Console::createCommand(const char *name_command, Module *listener)
{
	bool command_exists = false;
	
	for (doubleNode<Command*> *item = list_of_commands.getFirst(); item != NULL; item = item->next)
	{
		if (strcmp(item->data->name, name_command) == 0)
		{
			command_exists = true;
			break;
		}
	}

	if (!command_exists)
	{
		Command *c = new Command(name_command, listener);
		list_of_commands.add(c);
	}
	else
	{
		LOG("The command %s is already implemented.", name_command);
	}
	
	return true;
}

