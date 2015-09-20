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

SDL_RWops *loadFile(const char *file_name)
{
	void *ret = NULL;

	PHYSFS_File *pfile = PHYSFS_openRead(file_name);
	PHYSFS_uint64 size = PHYSFS_fileLength(pfile);
	PHYSFS_read(pfile, ret, PHYSFS_fileLength(pfile), 0);
	PHYSFS_close(pfile);

	return SDL_RWFromConstMem(ret, size);
}