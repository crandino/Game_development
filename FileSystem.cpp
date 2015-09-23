#include "p2Defs.h"
#include "App.h"
#include "p2Log.h"
#include "FileSystem.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

FileSystem::FileSystem(const char* game_path) : Module()
{
	name.create("file_system");

	// need to be created before awake so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	addPath(".");
	addPath(game_path);
}

// Destructor
FileSystem::~FileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool FileSystem::awake()
{
	LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(ORGANIZATION, APPNAME);

	if(PHYSFS_setWriteDir(write_path) == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	SDL_free(write_path);

	return ret;
}

// Called before quitting
bool FileSystem::cleanUp()
{
	//LOG("Freeing File System subsystem");

	return true;
}

// Add a new zip file or folder
bool FileSystem::addPath(const char* path_or_zip)
{
	bool ret = false;

	if(PHYSFS_mount(path_or_zip, NULL, 1) == 0)
		LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool FileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

// Read a whole file and put it in a new buffer
unsigned int FileSystem::Load(const char* file, char** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file != NULL)
	{
		PHYSFS_sint32 size = PHYSFS_fileLength(fs_file);

		if(size > 0)
		{
			*buffer = new char[size];
			int readed = PHYSFS_read(fs_file, *buffer, 1, size);
			if(readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = readed;
		}

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* FileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if(size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if(r != NULL)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return NULL;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int FileSystem::Save(const char* file, const char* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if(fs_file != NULL)
	{
		unsigned int written = PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if(written != size)
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		else
			ret = written;

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}