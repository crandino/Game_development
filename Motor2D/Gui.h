#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"
#include "Point2d.h"
#include "DList.h"
#include "SDL\include\SDL_render.h"

class UIimage;
class UIbutton;
class UIlabel;
class UIinputBox;
class UIelement;
struct _TTF_Font;

#define CURSOR_WIDTH 2

enum MOUSE_EVENTS
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK_LEFT,
	MOUSE_CLICK_RIGHT,
	MOUSE_LEAVE_LEFT,
	MOUSE_LEAVE_RIGHT,
	MOUSE_REPEAT_LEFT	
};

enum UI_TYPE
{
	UI_LABEL,
	UI_IMAGE,
	UI_BUTTON,
	UI_INPUTBOX
};

// ---------------------------------------------------
class Gui : public Module
{
public:

	UIelement				*screen;

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

	// On Gui for own behaviour
	void onGui(MOUSE_EVENTS mouse_event, UIelement *trigger);

	// Which element is under the mouse
	
	UIelement *whichUIelemOnMouse() const;

	// Gui creation functions
	const UIlabel *createLabel(iPoint p, const char *string, _TTF_Font *font = NULL, Module *mod = NULL, UIelement *parent = NULL);
	const UIimage *createImage(iPoint p, SDL_Texture *tex, SDL_Rect &section, Module *mod = NULL, UIelement *parent = NULL);
	const UIbutton *Gui::createButton(iPoint p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
									  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
									  Module *mod, UIelement *parent);
	const UIinputBox* createInputBox(const char *string, SDL_Texture *frame_tex, SDL_Rect frame_rect, 
									 SDL_Rect write_section, iPoint pos, Module *mod, UIelement *parent = NULL);

	const SDL_Texture* getAtlas() const;
	

private:	

	UIelement				*focus;

	DList<UIelement*>		UIelement_list;
	SDL_Texture*			atlas;
	p2SString				atlas_file_name;

	bool					debug;
	
};

#endif // __GUI_H__