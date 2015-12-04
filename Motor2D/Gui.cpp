#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "Input.h"
#include "Gui.h"
#include "UIelements.h"
#include "SDL\include\SDL_render.h"

Gui::Gui() : Module()
{
	name.create("gui");
	debug = false;
}

// Destructor
Gui::~Gui()
{}

// Called before render is available
bool Gui::awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool Gui::start()
{
	atlas = app->tex->loadTexture(atlas_file_name.GetString());
	screen = new UIimage(this);
	screen->interactable = false;
	screen->dragable = true;
	focus = screen;
	return true;
}

// Update all guis
bool Gui::preUpdate()
{
	iPoint p = app->input->getMousePosition();
	UIelement *u = whichUIelemOnMouse();

	doubleNode<Module*> *listener = u->mod_listeners.getLast();
	while (listener != NULL)
	{
		if (u != focus)
		{
			listener->data->onGui(MOUSE_LEAVE, focus);
			listener->data->onGui(MOUSE_ENTER, u);
		}	
				
		if (u->dragable && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			listener->data->onGui(MOUSE_REPEAT_LEFT, u);

		if (u->interactable)
		{
			// Behaviour for left mouse button
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				listener->data->onGui(MOUSE_CLICK_LEFT, u);
			else if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				listener->data->onGui(MOUSE_LEAVE_LEFT, u);

			// Behaviour for right mouse button
			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
				listener->data->onGui(MOUSE_CLICK_RIGHT, u);
			else if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
				listener->data->onGui(MOUSE_LEAVE_RIGHT, u);
		}
		listener = listener->previous;
	}

	return true;
}

// Called after all Updates
bool Gui::postUpdate()
{
	if (app->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	doubleNode<UIelement*> *item = UIelement_list.getFirst();
	while (item != NULL)
	{
		if (debug)
			item->data->drawDebug();
		item->data->draw();
		item = item->next;
	}
	return true;
}

// Called before quitting
bool Gui::cleanUp()
{
	LOG("Freeing GUI");

	doubleNode<UIelement*> *item = UIelement_list.getFirst();
	while (item != NULL)
	{
		delete item->data;
		item = item->next;
	}
	
	UIelement_list.clear();

	// Deleting screen root for tree
	delete screen;

	return true;
}

// const getter for atlas
const SDL_Texture* Gui::getAtlas() const
{
	return atlas;
}

// Factories for class Gui ---------------------------------------------------
const UIlabel* Gui::createLabel(const char *string, iPoint p, Module *mod, UIelement *parent)
{
	UIlabel *l = new UIlabel(mod);
	l->parent = parent != NULL ? parent : screen;
	l->text = string;
	l->text_image = app->fonts->print(string);
	l->setLocalPos(p);
	SDL_QueryTexture(l->text_image, NULL, NULL, &l->width, &l->height);

	UIelement_list.add(l);
	return l;
}

const UIimage* Gui::createImage(SDL_Texture *tex, iPoint p, SDL_Rect r_to_draw, Module *mod, UIelement *parent)
{
	UIimage *i = new UIimage(mod);
	i->parent = parent != NULL ? parent : screen;

	i->image.img = tex != NULL ? tex : (SDL_Texture*)app->gui->getAtlas();
	i->setLocalPos(p);
	i->width = r_to_draw.w;
	i->height = r_to_draw.h;
	i->image.draw_area = r_to_draw;	
	UIelement_list.add(i);
	return i;
}

const UIbutton *Gui::createButton(SDL_Texture *idle_tex, SDL_Texture *hover_tex, SDL_Texture *clicked_tex, 
								  SDL_Rect idle_rect, SDL_Rect hover_rect, SDL_Rect clicked_rect, iPoint p,
								  Module *mod, UIelement *parent)
{
	UIbutton *b = new UIbutton(mod);
	b->parent = parent != NULL ? parent : screen;

	b->idle.img = idle_tex != NULL ? idle_tex : (SDL_Texture*) app->gui->getAtlas();
	b->idle.draw_area = idle_rect;

	b->hover.img = hover_tex != NULL ? hover_tex : (SDL_Texture*)app->gui->getAtlas();
	b->hover.draw_area = hover_rect;
	
	b->clicked.img = clicked_tex != NULL ? clicked_tex : (SDL_Texture*)app->gui->getAtlas();
	b->clicked.draw_area = clicked_rect;

	b->current_state = &b->idle;
	b->setLocalPos(p);
	b->width = idle_rect.w;
	b->height = idle_rect.h;
	UIelement_list.add(b);
	return b;	
}

const UIinputBox *Gui::createInputBox(const char *string, SDL_Texture *frame_tex, SDL_Rect frame_rect,
									  SDL_Rect write_section, iPoint pos, Module *mod, UIelement *parent)
{
	UIinputBox *i = new UIinputBox(mod);
	i->parent = parent != NULL ? parent : screen;

	i->frame.img = frame_tex != NULL ? frame_tex : (SDL_Texture*)app->gui->getAtlas();
	i->setLocalPos(pos);
	i->width = frame_rect.w;
	i->height = frame_rect.h;
	i->frame.draw_area = frame_rect;
	i->write_section = write_section;
	i->text = string;
	i->text_image = app->fonts->print(string);
	//SDL_QueryTexture(l->text_image, NULL, NULL, &l->width, &l->height);
	UIelement_list.add(i);
	return i;
}

void Gui::onGui(MOUSE_EVENTS mouse_event, UIelement *trigger)
{
	switch (trigger->type)
	{
	case UI_BUTTON:
	{
		UIbutton *b = (UIbutton*)trigger;
		switch (mouse_event)
		{
		case MOUSE_ENTER:
			b->current_state = &b->hover;
			focus = b;
			break;
		case MOUSE_LEAVE:
			b->current_state = &b->idle;
			break;
		case MOUSE_CLICK_LEFT:
		case MOUSE_CLICK_RIGHT:
			b->current_state = &b->clicked;
			break;
		case MOUSE_REPEAT_LEFT:
			b->dragElement();
			break;
		case MOUSE_LEAVE_LEFT:
		case MOUSE_LEAVE_RIGHT:
			b->current_state = &b->hover;
			break;
		}
		break;
	}

	case UI_IMAGE:
	{
		UIimage *i = (UIimage*)trigger;
		switch (mouse_event)
		{
		case MOUSE_ENTER:
			focus = i;
			break;
		case MOUSE_LEAVE:
			break;
		case MOUSE_REPEAT_LEFT:
			i->dragElement();
			break;
		}
		break;
	}
	}
}

UIelement *Gui::whichUIelemOnMouse() const
{
	iPoint p = app->input->getMousePosition();

	doubleNode<UIelement*> *u = UIelement_list.getLast();
	while (u != NULL)
	{
		if (u->data->isMouseIn(p) && u->data->dragable)
			return u->data;
		u = u->previous;
	}

	return screen;
}
