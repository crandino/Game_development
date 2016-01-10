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
	setLocalPos(pos + p);
}

// ---- UI Label -----
// Constructor:
UIlabel::UIlabel( )
{ }

// Destructor:
UIlabel::~UIlabel()
{ 
	SDL_DestroyTexture(text_tex.img);
}

// Init:
void UIlabel::init(iPoint pos, const char *string, _TTF_Font *f, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
		// Listeners.
	addListener(app->gui);
	addListener(module);

	interactable = false;
	is_inside = false;
	type = UI_LABEL;
	text.create(string);
	font = f != NULL ? f : app->fonts->default;
	SDL_Color white = { 255, 255, 255, 255 };
	text_tex.img = app->fonts->print(text.GetString(), white, font);
	text_tex.section.x = 0;
	text_tex.section.y = 0;
	app->fonts->calcSize(text.GetString(), text_tex.section.w, text_tex.section.h, font);
	setDimensions(text_tex.section.w, text_tex.section.h);
}
// Draw:
bool UIlabel::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(text_tex.img, p.x, p.y, &text_tex.section);
	return true;
}

void UIlabel::setText(const char *t)
{
	text.create(t);
	SDL_DestroyTexture(text_tex.img);
	text_tex.img = app->fonts->print(text.GetString(), { 255, 255, 255, 255 }, font);
}

// ----- UIimage -----
// Constructor:
UIimage::UIimage()
{ }

// Destructor:
UIimage::~UIimage()
{
	SDL_DestroyTexture(image.img);
}

// Init:
void UIimage::init(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	interactable = false;
	is_inside = false;
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
{ 
	SDL_DestroyTexture(idle.img);
	SDL_DestroyTexture(hover.img);
	SDL_DestroyTexture(clicked.img);
}

// Draw:
void UIbutton::init(iPoint pos, SDL_Texture *tex_idle, SDL_Rect &section_idle, SDL_Texture *tex_hover, SDL_Rect &section_hover,
	SDL_Texture *tex_clicked, SDL_Rect &section_clicked, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	interactable = true;
	is_inside = false;
	can_be_focused = true;
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

// Destructor:
UIinputBox::~UIinputBox()
{
	SDL_DestroyTexture(frame.image.img);
	SDL_DestroyTexture(text.text_tex.img);
}

// Init:
void UIinputBox::init(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
	_TTF_Font *font, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	frame.init({ 0, 0 }, frame_tex, frame_section, module, this);
	text.init(text_offset, initial_text, font, module, this);
	offset = text_offset;
	default_string = initial_text;
	cursor_pos = 0;
	cursor_index = 0;
	interactable = true;
	is_inside = false;
	active = false;
	can_be_focused = true;
	type = UI_INPUTBOX;
	setDimensions(frame_section.w, frame_section.h);

	// Cursor
	cursor_width = CURSOR_WIDTH;
	int dummy;
	app->fonts->calcSize("X", dummy, cursor_height, font);
}

// Draw:
bool UIinputBox::draw()
{
	iPoint p = getScreenPos();
	// Frame
	app->render->blit(frame.image.img, p.x, p.y, &frame.image.section);	
	// Label
	if(strcmp(text.text.GetString() ,"") != 0)		
		app->render->blit(text.text_tex.img, p.x + offset.x, p.y + offset.y);	
	// Cursor
	if(active)							
		app->render->DrawQuad({ p.x + offset.x + cursor_pos, p.y + offset.y, cursor_width, cursor_height }, 255, 255, 255);   
	return true;
}

// preUpdate:
bool UIinputBox::preUpdate()
{
	if (active)
	{
		uint length_string = text.text.Length();
		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			if(cursor_index > 0) cursor_index--;
			moveCursor();
		}
		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			if (cursor_index < length_string) cursor_index++;
			moveCursor();
		}
	}

	return true;
}

void UIinputBox::moveCursor()
{
	
	if (cursor_index == 0)
		cursor_pos = 0;
	else
	{
		const char *t = text.text.GetString();
		char c[MAX_STRING_UI + 1 ];
		memset(c, '\0', MAX_STRING_UI + 1);

		for (int i = 0; i < cursor_index; i++)
			c[i] = t[i];

		int dummy;
		app->fonts->calcSize(c, cursor_pos, dummy, text.font);
			
	}
}

void UIinputBox::drawDebug()
{
	// Frame
	iPoint p = frame.getScreenPos();
	int w, h; frame.getScreenRect(w, h);
	SDL_Rect r = { p.x, p.y, w,h };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);

	// Label
	p = text.getScreenPos();
	text.getScreenRect(w, h);
	r = { p.x, p.y, w, h };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);
}

// ----- UIHorizontalScrollBar -----

UIHorizontalScrollBar::UIHorizontalScrollBar()
{ }

UIHorizontalScrollBar::~UIHorizontalScrollBar()
{
	SDL_DestroyTexture(bar.image.img);
	SDL_DestroyTexture(thumb.image.img);
}

// EXERCISE 1 and 6
void UIHorizontalScrollBar::init(iPoint pos, int bar_offset, int thumb_vert_offset, SDL_Texture *bar_tex, SDL_Rect &section_bar,
	SDL_Texture *thumb_tex, SDL_Rect &section_thumb, Module *module, UIelement *parent)
{
	setLocalPos(pos);
	this->parent = parent != NULL ? parent : app->gui->screen;
	// Listeners.
	addListener(app->gui);
	addListener(module);

	bar.init({ 0, 0 }, bar_tex, section_bar, module, this);
	thumb_pos.x = bar_offset;
	thumb_pos.y = thumb_vert_offset;
	thumb.init( thumb_pos, thumb_tex, section_thumb, module, this);
	left_limit = bar_offset;
	right_limit = section_bar.w - bar_offset;
	
	interactable = true;
	can_be_focused = true;
	type = UI_HORIZONTALSCROLLBAR;
	setDimensions(section_bar.w, section_bar.h);

}

// Draw:
bool UIHorizontalScrollBar::draw()
{
	iPoint p = getScreenPos();
	// Bar
	app->render->blit(bar.image.img, p.x, p.y, &bar.image.section);
	// Thumb
	app->render->blit(thumb.image.img, p.x + thumb_pos.x, p.y + thumb_pos.y, &thumb.image.section);
	
	return true;
}

// Draw:
// EXERCISE 5
float UIHorizontalScrollBar::calculateValue()
{
	return ((float)(thumb_pos.x - left_limit) / ((right_limit - thumb.image.section.w)  - left_limit));
}

// Draw:
// EXERCISE 2
void UIHorizontalScrollBar::dragElement()
{
	iPoint p = app->input->getMouseMotion();
	int final_pos = thumb_pos.x + p.x;
	if (checkLimits(final_pos))
	{
		thumb_pos.x = final_pos;
		iPoint new_pos = { final_pos, thumb_pos.y };
		thumb.setLocalPos(new_pos);

		for (doubleNode<Module*> *item = mod_listeners.getLast(); item != NULL; item = item->previous)
			item->data->onGui(DRAGGED, this);
	}
}

// EXERCISE 2 and 3
bool UIHorizontalScrollBar::checkLimits(int new_x_pos)
{
	return (new_x_pos >= left_limit && new_x_pos + thumb.image.section.w <= right_limit);
}

void UIHorizontalScrollBar::drawDebug()
{
	// Bar
	iPoint p = bar.getScreenPos();
	int w, h; bar.getScreenRect(w, h);
	SDL_Rect r = { p.x, p.y, w, h };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);

	// Thumb
	p = thumb.getScreenPos();
	thumb.getScreenRect(w, h);
	r = { p.x, p.y, w, h };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);
}

// EXERCISE 3
bool UIHorizontalScrollBar::preUpdate()
{
	if (this == app->gui->whichFocus())
	{
		int speed = 1;
		iPoint new_pos;

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && checkLimits(thumb_pos.x - speed))
		{
			thumb_pos.x -= speed;
			thumb.setLocalPos(thumb_pos);			
		}
		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && checkLimits(thumb_pos.x + speed))
		{
			thumb_pos.x += speed;
			thumb.setLocalPos(thumb_pos);
		}

		for (doubleNode<Module*> *item = mod_listeners.getLast(); item != NULL; item = item->previous)
			item->data->onGui(DRAGGED, this);
	}

	return true;
}