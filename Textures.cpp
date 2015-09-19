#include "Globals.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

Textures::Textures() : Module()
{
	name.create("textures");
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::awake()
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool Textures::cleanUp()
{
	LOG("Freeing textures and Image library");
	doubleNode<SDL_Texture*>* item = textures.getFirst();

	for(; item != NULL; item = item->next)
	{
		SDL_DestroyTexture(item->data);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = loadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool Textures::unLoad(SDL_Texture* texture)
{
	doubleNode<SDL_Texture*>* item = textures.getFirst();

	for(; item != NULL; item = item->next)
	{
		if(texture == item->data)
		{
			SDL_DestroyTexture(item->data);
			textures.del(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::loadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.add(texture);
	}

	return texture;
}

// Retrieve size of a texture
void Textures::getSize(const SDL_Texture* texture, unsigned int& width, unsigned int& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
