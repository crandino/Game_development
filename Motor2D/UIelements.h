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
	

public:

	const UIelement*		parent;
	bool					dragable;
	bool					interactable;
	UI_TYPE					type;
	DList<Module*>			mod_listeners;		// Module listener

	UIelement() {};
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
public:

	const char		*text;
	StateImage		text_tex;
	_TTF_Font		*font;

	UIlabel();
	~UIlabel();

	void init(iPoint pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent);
	bool draw();	
};

class UIimage : public UIelement
{
	
public:

	StateImage		image;

	UIimage();
	~UIimage();

	void init(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent);
	bool draw();
	
};

class UIbutton : public UIelement
{

public:

	StateImage		idle;
	StateImage		hover;
	StateImage		clicked;
	StateImage      *current_state;

	UIbutton();
	~UIbutton();

	void init(iPoint pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover,
		SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UIelement *parent);
	bool draw();
	bool preUpdate();

	void setIdleState();
	void setHoverState();
	void setClickedState();
};

class UIinputBox : public UIelement
{

public:
	UIimage			frame;
	UIlabel			text;
	iPoint			offset;
	int				cursor_width;

	UIinputBox();
	~UIinputBox();

	void init(iPoint pos,iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
		_TTF_Font *font, Module *module, UIelement *parent);
	bool draw();
	bool preUpdate();
	void setCursor();
};

#endif //__UIELEMENTS__
