#include "Globals.h"	
#include "FileSystem.h"

#include "PhysFS\include\physfs.h"

FileSystem::FileSystem() : Module()
{ }

// Destructor
FileSystem::~FileSystem()
{ }

// Called before render is available
bool FileSystem::awake()
{
	LOG("Init PHYSFS library");
	bool ret = true;

	if (PHYSFS_init(NULL) == 0)
		ret = false;

	return ret;
}

// Called before quitting
bool FileSystem::cleanUp()
{
	LOG("Destroying PHYSFS library");
	PHYSFS_deinit();

	return true;
}