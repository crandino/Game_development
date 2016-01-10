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
class UIHorizontalScrollBar;
class UIelement;
struct _TTF_Font;

enum MOUSE_EVENTS
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK_LEFT,
	MOUSE_CLICK_RIGHT,
	MOUSE_LEAVE_LEFT,
	MOUSE_LEAVE_RIGHT,
	MOUSE_REPEAT_LEFT,
	GAIN_FOCUS,
	LOST_FOCUS,
	TEXT_CHANGED,
	DRAGGED
};

enum UI_TYPE
{
	UI_LABEL,
	UI_IMAGE,
	UI_BUTTON,
	UI_INPUTBOX,
	UI_HORIZONTALSCROLLBAR
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
	UIlabel *createLabel(iPoint p, const char *string, _TTF_Font *font = NULL, Module *mod = NULL, UIelement *parent = NULL);
	UIimage *createImage(iPoint p, SDL_Texture *tex, SDL_Rect &section, Module *mod = NULL, UIelement *parent = NULL);
	UIbutton *Gui::createButton(iPoint p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
									  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
									  Module *mod = NULL, UIelement *parent = NULL);
	UIinputBox *Gui::createInputBox(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
									      _TTF_Font *font, Module *module = NULL, UIelement *parent = NULL);
	// EXERCISE 1 and 6
	UIHorizontalScrollBar *Gui::createHorizontalScrollBar(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
								SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module = NULL, UIelement *parent = NULL);

	const SDL_Texture* getAtlas() const;
	const UIelement* whichFocus() const;
	

private:	

	UIelement				*focus;
	UIelement				*current_UIelement;
	UIelement				*previous_UIelement;

	DList<UIelement*>		UIelement_list;
	SDL_Texture*			atlas;
	p2SString				atlas_file_name;

	bool					debug;
	
};

#endif // __GUI_H__