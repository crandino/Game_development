#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum UI_TYPE
{
	UI_LABEL,
	UI_IMAGE,
	UI_BUTTON,
	UI_INPUTBOX
};

class UIelement
{
public:
	iPoint		pos;
	UI_TYPE		type;

	virtual bool update() { return true; }
	virtual bool draw() { return true; }
};

class UIlabel : public UIelement
{
public:

	const char		*text;
	SDL_Texture		*text_image;

	UIlabel();
	~UIlabel();
	
	bool draw();
};

class UIimage : public UIelement
{
public:

	SDL_Texture			*tex;
	SDL_Rect			rect;

	UIimage();
	~UIimage();

	bool draw();
};

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called before all Updates
	bool preUpdate();

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	const UIlabel* createLabel(const char *string, iPoint pos );
	const UIimage* createImage(SDL_Texture *tex, iPoint pos, SDL_Rect &r);

	const SDL_Texture* getAtlas() const;

private:

	SDL_Texture*			atlas;
	p2SString				atlas_file_name;
	DList<UIelement*>		UIelement_list;
};

#endif // __GUI_H__