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

// UI Element
UIelement::UIelement(iPoint position, Module *module, UIelement *parent)
{
	pos.set(position.x, position.y);
	this->parent = parent != NULL ? parent : app->gui->screen;
	mod_listeners.add(app->gui);
	if (module != NULL)
		mod_listeners.add(module);
}

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

// ----- CONSTRUCTORS -----
UIlabel::UIlabel(iPoint pos, SDL_Texture *tex, const char *string, _TTF_Font *f, Module *module, UIelement *parent) : UIelement(pos, module, parent)
{
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

UIimage::UIimage(iPoint pos, SDL_Texture *tex, SDL_Rect &section, Module *module, UIelement *parent) : UIelement(pos, module, parent)
{
	
	dragable = true;
	interactable = false;
	type = UI_IMAGE;
	image.img = tex != NULL ? tex : (SDL_Texture*)app->gui->getAtlas();
	image.section = section;
	setDimensions(section.w, section.h);
}

UIbutton::UIbutton(iPoint pos, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
				   SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
				   Module *mod, UIelement *parent) : UIelement(pos, mod, parent)
{
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


//UIinputBox::UIinputBox(Module *mod) : UIelement(mod)
//{
//	dragable = false;
//	interactable = true;
//	type = UI_INPUTBOX;
//	text = NULL;
//	text_image = NULL;
//}

// ----- DRAW METHOD -----

bool UIlabel::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(text_tex.img, p.x, p.y, &text_tex.section);
	return true;
}

bool UIimage::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(image.img, p.x, p.y, &image.section);
	return true;
}

bool UIbutton::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(current_state->img, p.x, p.y, &current_state->section);
	return true;
}

//bool UIinputBox::draw()
//{
//	iPoint p = getScreenPos();
//	app->render->blit(frame.img, p.x, p.y, &frame.section);
//	app->render->blit(text_image, p.x + write_section.x, p.y + write_section.y);
//	return true;
//}

// ----- Drag METHOD -----

void UIelement::dragElement()
{
	iPoint p = app->input->getMouseMotion();
	pos += p;
}

// ----- Auxiliary methods for buttons -----

void UIbutton::setIdleState()
{
	current_state = &idle;
}

void UIbutton::setHoverState()
{
	current_state = &hover;
}

void UIbutton::setClickedState()
{
	current_state = &clicked;
}