#include "Gui.h"
#include "UIelements.h"
#include "Point2d.h"
#include "DList.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "p2Log.h"
#include "SDL\include\SDL_rect.h"
#include "SDL\include\SDL_keycode.h"

// UI Element
UIelement::UIelement(Module *mod)
{
	pos.setZero();
	width = height = 0;
	interactable = false;
	parent = NULL;
	mod_listeners.add(app->gui);
	mod_listeners.add(mod);
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

//GetLocalRect()

void UIelement::drawDebug()
{
	iPoint p = getScreenPos();
	SDL_Rect r = { p.x, p.y, width, height };
	app->render->DrawQuad(r, 255, 125, 0, 255, false);
}

// ----- CONSTRUCTORS -----
UIlabel::UIlabel(Module *mod) : UIelement(mod)
{
	dragable = false;
	interactable = false;
	type = UI_LABEL;
	text = NULL;
	text_image = NULL;
}

UIimage::UIimage(Module *mod) : UIelement(mod)
{
	dragable = true;
	interactable = false;
	type = UI_IMAGE;
	image.draw_area = { 0, 0, 0, 0 };
}

UIbutton::UIbutton(Module *mod) : UIelement(mod)
{
	dragable = true;
	interactable = true;
	type = UI_BUTTON;
}

UIinputBox::UIinputBox(Module *mod) : UIelement(mod)
{
	dragable = false;
	interactable = true;
	type = UI_INPUTBOX;
	text = NULL;
	text_image = NULL;
}

// ----- DRAW METHOD -----

bool UIlabel::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(text_image, p.x, p.y);
	return true;
}

bool UIimage::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(image.img, p.x, p.y, &image.draw_area);
	return true;
}

bool UIbutton::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(current_state->img, p.x, p.y, &current_state->draw_area);
	return true;
}

bool UIinputBox::draw()
{
	iPoint p = getScreenPos();
	app->render->blit(frame.img, p.x, p.y, &frame.draw_area);
	app->render->blit(text_image, p.x + write_section.x, p.y + write_section.y);
	return true;
}

// ----- Drag METHOD -----

void UIelement::dragElement()
{
	iPoint p = app->input->getMouseMotion();
	pos += p;
}