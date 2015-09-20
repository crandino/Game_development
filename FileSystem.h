#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

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
};

#endif // __FILESYSTEM_H__