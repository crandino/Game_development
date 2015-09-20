#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"
#include "SDL\include\SDL.h"

class FileSystem : public Module
{
public:

	FileSystem();

	// Destructor
	virtual ~FileSystem();

	// Called before render is available
	bool awake();

	// Called before quitting
	bool cleanUp();

	// Method to deal with files, abstracting its
	// functionality with PhysFS library.
	SDL_RWops *loadFile(const char *file_name);
};

#endif // __FILESYSTEM_H__