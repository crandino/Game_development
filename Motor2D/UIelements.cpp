#include "Gui.h"
#include "UIelements.h"
#include "Point2d.h"
#include "DList.h"
#include "App.h"
#include "Input.h"
#include "Fonts.h"
#include "Render.h"
#include "p2Log.h"
#include "SDL\include\SDL_rect.h"
#include "SDL\include\SDL_keycode.h"

// ---- UI Element -----
bool UIelement::isMouseIn(const iPoint &p)
{
	iPoint p_elem = getScreenPos();

	return (p.x >= p_elem.x &&
		p.x <= (p_elem.x + width) &&
		p.y >= p_elem.y &&
		p.y <= (p_elem.y + height));
}

iPoint UIelement::getScreenPos() const
{
	iPoint p(0, 0);
	const UIelement *item = this;
	while (item != NULL)
	{
		p += { item->pos.x, item->pos.y };
		item = item->parent;
	}
	return p;
}

void UIelement::getScreenRect(int &w, int &h) const
{
	w = width;
	h = height;
}

void UIelement::setLocalPos(iPoint local_pos)
{
	pos.x = local_pos.x;
	pos.y = local_pos.y;
}

iPoint UIelement::getLocalPos() const
{
	return pos;
}

void UIelement::addListener(Module *module)
{
	if (module != NULL)
		mod_listeners.add(module);
}

void UIelement::setDimensions(const int &width, const int &height)
{
	this->width = width;
	this->height = height;
}

//GetLocalRect()

void UIelement::drawDebug()
{
	iPoint p = getScreenPos();
	SDL_Rect r = { p.x, p.y, width, height };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);
}

// DragElement:
void UIelement::dragElement()
{
	iPoint p = app->input->getMouseMotion();
	pos += p;
}

// ---- UI Label -----
// Constructor:
UIlabel::UIlabel( )
{ }

// Destructor:
UIlabel::~UIlabel()
{ }

// Init:
void UIlabel::init(iPoint pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
		// Listeners.
	addListener(app->gui);
	addListener(module);

	dragable = false;
	interactable = false;
	type = UI_LABEL;
	text = string;
	font = f != NULL ? f : app->fonts->default;
	SDL_Color white = { 255, 255, 255, 255 };
	text_tex.img = app->fonts->print(text, white, font);
	text_tex.section.x = 0;
	text_tex.section.y = 0;
	app->fonts->calcSize(text, text_tex.section.w, text_tex.section.h, font);
	setDimensions(text_tex.section.w, text_tex.section.h);
}
// Draw:
bool UIlabel::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(text_tex.img, p.x, p.y, &text_tex.section);
	return true;
}

// ----- UIimage -----
// Constructor:
UIimage::UIimage()
{ }

// Destructor:
UIimage::~UIimage()
{ }

// Init:
void UIimage::init(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	dragable = true;
	interactable = false;
	type = UI_IMAGE;
	image.img = tex != NULL ? tex : (SDL_Texture*)app->gui->getAtlas();
	image.section = section;
	setDimensions(section.w, section.h);
}

// Draw:
bool UIimage::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(image.img, p.x, p.y, &image.section);
	return true;
}

// ----- UIbutton -----
// Constructor:
UIbutton::UIbutton()
{ }

// Destructor:
UIbutton::~UIbutton()
{ }

// Draw:
void UIbutton::init(iPoint pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover,
	SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	dragable = true;
	interactable = true;
	type = UI_BUTTON;
	idle.img = tex_idle != NULL ? tex_idle : (SDL_Texture*)app->gui->getAtlas();
	hover.img = tex_hover != NULL ? tex_hover : (SDL_Texture*)app->gui->getAtlas();
	clicked.img = tex_clicked != NULL ? tex_clicked : (SDL_Texture*)app->gui->getAtlas();
	idle.section = section_idle;
	hover.section = section_hover;
	clicked.section = section_clicked;
	current_state = &idle;
	setDimensions(idle.section.w, idle.section.h);
}

// Draw:
bool UIbutton::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(current_state->img, p.x, p.y, &current_state->section);
	return true;
}

// State Button methods:
void UIbutton::setIdleState()
{	current_state = &idle;  }

void UIbutton::setHoverState()
{   current_state = &hover; }

void UIbutton::setClickedState()
{	current_state = &clicked; }

// ----- UIinputBox -----
// Constructor:
UIinputBox::UIinputBox()
{ }

// Init:
void UIinputBox::init(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
	_TTF_Font *font, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	frame.init(pos, frame_tex, frame_section,module, this);
	text.init(pos + text_offset, initial_text, font, module, this);
	offset = text_offset;
	dragable = false;
	interactable = true;
	type = UI_INPUTBOX;
	setDimensions(frame_section.w, frame_section.h);
}

// Draw:
bool UIinputBox::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(frame.image.img, p.x, p.y, &frame.image.section);
	app->render->blit(text.text_tex.img, p.x + offset.x, p.y + offset.y);
	return true;
}

// SetCursor:
void UIinputBox::setCursor()
{
	int dummy;
	app->fonts->calcSize("X", cursor_width, dummy, text.font);
}
