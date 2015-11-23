#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "DList.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class Fonts : public Module
{
public:

	Fonts();

	// Destructor
	virtual ~Fonts();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Load Font
	_TTF_Font* const load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool calcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	DList<_TTF_Font*>	fonts;
	_TTF_Font*			default;
};

#endif // __FONTS_H__