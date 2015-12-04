#ifndef __UIELEMENTS__
#define __UIELEMENTS__

#include "DList.h"

struct StateImage
{
	SDL_Texture		*img;
	SDL_Rect		draw_area;

	StateImage() { img = NULL; }
};

class Module;

class UIelement
{

public:
	
	iPoint					pos;
	int 					width, height;
	UI_TYPE					type;
	const UIelement*		parent;
	DList<Module*>			mod_listeners;		// Module listener
	bool					dragable;
	bool					interactable;

	UIelement(Module *m);
	~UIelement() {};

	virtual bool draw() { return true; }

	bool isMouseIn(const iPoint &pos);
	void dragElement();
	
	void drawDebug();

	iPoint getScreenPos() const;
	void getScreenRect(int &w, int &h) const;
	iPoint getLocalPos() const;
	void setLocalPos(iPoint local_pos);
};

class UIlabel : public UIelement
{
public:

	// Afegir una clase font. Aquest mètode té una funció
	// (print) que et genera la textura amb el text y la font.
	const char		*text;
	SDL_Texture		*text_image;

	UIlabel(Module *mod);
	~UIlabel();

	bool draw();
};

class UIimage : public UIelement
{
public:

	StateImage		image;
	
	UIimage(Module *mod);
	~UIimage();

	bool draw();
};

class UIbutton : public UIelement
{
public:

	StateImage idle;
	StateImage hover;
	StateImage clicked;
	StateImage *current_state;

	UIbutton(Module *mod);
	~UIbutton();

	bool draw();
	bool preUpdate();
};

class UIinputBox : public UIelement
{
public:

	StateImage		frame;
	const char		*text;
	SDL_Texture		*text_image;
	SDL_Rect		write_section;

	UIinputBox(Module *mod);
	~UIinputBox();

	bool draw();
	bool preUpdate();
};

#endif //__UIELEMENTS__
