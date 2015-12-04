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
	const UIlabel* createLabel(const char *string, iPoint pos, Module *mod, UIelement *parent = NULL);
	const UIimage* createImage(SDL_Texture *tex, iPoint pos, SDL_Rect r, Module *mod, UIelement *parent = NULL);
	const UIbutton* createButton(SDL_Texture *idle_tex, SDL_Texture *hover_tex, SDL_Texture *clicked_tex,
								 SDL_Rect idle_rect, SDL_Rect hover_rect, SDL_Rect clicked_rect,
								 iPoint pos, Module *mod, UIelement *parent = NULL);
	const UIinputBox* createInputBox(const char *string, SDL_Texture *frame_tex, SDL_Rect frame_rect, 
									 SDL_Rect write_section, iPoint pos, Module *mod, UIelement *parent = NULL);

	const SDL_Texture* getAtlas() const;
	

private:	

	UIelement				*screen;
	UIelement				*focus;

	DList<UIelement*>		UIelement_list;
	SDL_Texture*			atlas;
	p2SString				atlas_file_name;

	bool					debug;
	
};

#endif // __GUI_H__