#ifndef __UIELEMENTS__
#define __UIELEMENTS__

#include "DList.h"
#include "Gui.h"

#define MAX_STRING_UI 10

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
	bool					interactable;
	bool					is_inside;
	bool					can_be_focused = false;
	UI_TYPE					type;
	DList<Module*>			mod_listeners;			// Module listener

	UIelement() {}
	virtual ~UIelement() {}

	virtual bool draw() { return true; }
	virtual bool preUpdate() { return true; }
	virtual void drawDebug();

	bool isMouseIn(const iPoint &pos);
	virtual void dragElement();

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

	p2SString		text;
	StateImage		text_tex;
	_TTF_Font		*font;

	UIlabel();
	~UIlabel();

	void init(iPoint pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent);
	bool draw();
	void setText(const char *t);
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

	void setIdleState();
	void setHoverState();
	void setClickedState();
};

#define CURSOR_WIDTH 2

class UIinputBox : public UIelement
{

public:
	UIimage			frame;
	UIlabel			text;
	iPoint			offset;
	int				cursor_pos;
	uint			cursor_index;
	int				cursor_height;
	int				cursor_width;

	bool			active;
	const char*		default_string;

	UIinputBox();
	~UIinputBox();

	void init(iPoint pos,iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
		_TTF_Font *font, Module *module, UIelement *parent);
	bool draw();
	bool preUpdate();
	void drawDebug();

	void moveCursor();
};

// EXERCISE 1 and 6
class UIHorizontalScrollBar : public UIelement
{

public:

	UIimage			bar;
	UIimage			thumb;
	iPoint			thumb_pos;
	int				left_limit;
	int				right_limit;

	UIHorizontalScrollBar();
	~UIHorizontalScrollBar();

	void init(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
			  SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent);
	bool draw();
	// EXERCISE 3
	bool preUpdate();
	void drawDebug();
	// EXERCISE 2
	void dragElement();
	// EXERCISE 5
	float calculateValue();
	// EXERCISE 2 and 3
	bool checkLimits(int new_x_pos);
	
};


#endif //__UIELEMENTS__
