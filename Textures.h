#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"
#include "DList.h"

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called before quitting
	bool cleanUp();

	// Load Texture
	SDL_Texture* const	load(const char* path);
	bool				unLoad(SDL_Texture* texture);
	SDL_Texture* const	loadSurface(SDL_Surface* surface);
	void				getSize(const SDL_Texture* texture, unsigned int& width, unsigned int& height) const;

public:

	DList<SDL_Texture*>	textures;
};


#endif // __TEXTURES_H__