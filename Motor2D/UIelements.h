#ifndef __UIELEMENTS__
#define __UIELEMENTS__

#include "DList.h"

struct StateImage
{
	SDL_Texture		*img;
	SDL_Rect		section;

	StateImage() { img = NULL; }
};

class Module;

class UIelement
{

private:
	
	iPoint					pos;
	int 					width, height;
	const UIelement*		parent;

public:

	bool					dragable;
	bool					interactable;
	UI_TYPE					type;
	DList<Module*>			mod_listeners;		// Module listener

	UIelement(iPoint pos, Module *module, UIelement *parent);
	~UIelement() {};

	virtual bool draw() { return true; }
	void drawDebug();

	bool isMouseIn(const iPoint &pos);
	void dragElement();

	iPoint getScreenPos() const;
	void getScreenRect(int &w, int &h) const;
	iPoint getLocalPos() const;
	void setLocalPos(iPoint local_pos);

	void addListener(Module *module);
	void setDimensions(const int &width, const int &height);
};

class UIlabel : public UIelement
{

private:
	
	const char		*text;
	StateImage		text_tex;
	// FONT, added please!!!
	_TTF_Font		*font;

public:

	UIlabel(iPoint pos, SDL_Texture *tex, const char *string, _TTF_Font *font, Module *module, UIelement *parent);
	~UIlabel();

	bool draw();
};

class UIimage : public UIelement
{
	
private:

	StateImage		image;

public:

	UIimage(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent);
	~UIimage();

	bool draw();
};

class UIbutton : public UIelement
{
private:

	StateImage		idle;
	StateImage		hover;
	StateImage		clicked;

public:

	StateImage *current_state;

	UIbutton(iPoint pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover, SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UIelement *parent);
	~UIbutton();

	bool draw();
	bool preUpdate();

	void setIdleState();
	void setHoverState();
	void setClickedState();
};

//class UIinputBox : public UIelement
//{
//public:
//
//	StateImage		frame;
//	const char		*text;
//	SDL_Texture		*text_image;
//	SDL_Rect		write_section;
//
//	UIinputBox(Module *mod);
//	~UIinputBox();
//
//	bool draw();
//	bool preUpdate();
//};

#endif //__UIELEMENTS__
