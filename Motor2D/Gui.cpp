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
	//SDL_Rect screen_rect = { 0, 0, 800, 600 };
	screen = new UIelement();
	screen->interactable = false;
	focus = screen;
	previous_UIelement = screen;
	return true;
}

// Update all guis
bool Gui::preUpdate()
{
	iPoint p = app->input->getMousePosition();
	current_UIelement = whichUIelemOnMouse();

	doubleNode<Module*> *listener = current_UIelement->mod_listeners.getLast();
	while (listener != NULL)
	{

		if (current_UIelement != previous_UIelement)
			listener->data->onGui(MOUSE_LEAVE, previous_UIelement);

		if (current_UIelement->interactable)
		{
			if (!current_UIelement->is_inside && current_UIelement->isMouseIn(p))
				listener->data->onGui(MOUSE_ENTER, current_UIelement);

			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
				listener->data->onGui(MOUSE_REPEAT_LEFT, current_UIelement);

			// Behaviour for left mouse button
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				listener->data->onGui(MOUSE_CLICK_LEFT, current_UIelement);
			else if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				listener->data->onGui(MOUSE_LEAVE_LEFT, current_UIelement);

			// Behaviour for right mouse button
			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
				listener->data->onGui(MOUSE_CLICK_RIGHT, current_UIelement);
			else if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
				listener->data->onGui(MOUSE_LEAVE_RIGHT, current_UIelement);
		}
		listener = listener->previous;
	}

	//if (focus->type == UI_INPUTBOX)
	//{
	//	UIinputBox *b = (UIinputBox*)focus;
	//	if (b->active)
	//		b->checkInput();
	//}	

	previous_UIelement = current_UIelement;

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
const UIlabel* Gui::createLabel(iPoint p, const char *string, _TTF_Font *font, Module *mod, UIelement *parent)
{
	UIlabel *l = new UIlabel();
	l->init(p, string, font, mod, parent);
	UIelement_list.add(l);
	return l;
}

const UIimage* Gui::createImage(iPoint p, SDL_Texture *tex, SDL_Rect &section, Module *mod, UIelement *parent)
{
	UIimage *i = new UIimage();
	i->init(p, tex, section, mod, parent);
	UIelement_list.add(i);
	return i;
}

const UIbutton *Gui::createButton(iPoint p, SDL_Texture *tex_idle, SDL_Rect& section_idle, SDL_Texture *tex_hover,
								  SDL_Rect& section_hover, SDL_Texture *tex_clicked, SDL_Rect& section_clicked,
								  Module *mod, UIelement *parent)
{
	UIbutton *b = new UIbutton();
	b->init(p, tex_idle, section_idle, tex_hover, section_hover, tex_clicked, section_clicked, mod, parent);
	UIelement_list.add(b);

	return b;	
}

const UIinputBox *Gui::createInputBox(iPoint pos, iPoint text_offset, SDL_Texture *frame_tex, SDL_Rect &frame_section, const char *initial_text,
									  _TTF_Font *font, Module *module, UIelement *parent)
{
	UIinputBox *i = new UIinputBox();
	i->init(pos, text_offset, frame_tex, frame_section, initial_text, font, module, parent);
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
			b->setHoverState();
			b->is_inside = true;	
			break;
		case MOUSE_LEAVE:
			b->setIdleState();
			b->is_inside = false;
			break;
		case MOUSE_CLICK_LEFT:
		case MOUSE_CLICK_RIGHT:
			b->setClickedState();
			if (focus != trigger)
			{
				focus = trigger;
				app->input->stopTextInput();
			}
			break;
		case MOUSE_REPEAT_LEFT:
			b->dragElement();
			break;
		case MOUSE_LEAVE_LEFT:
		case MOUSE_LEAVE_RIGHT:
			b->setHoverState();
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
			break;
		case MOUSE_LEAVE:
			break;
		case MOUSE_REPEAT_LEFT:
			i->dragElement();
			break;
		}
		break;
	}

	case UI_INPUTBOX:
	{
		UIinputBox *i = (UIinputBox*)trigger;
		switch (mouse_event)
		{
		case MOUSE_ENTER:
			break;
		case MOUSE_LEAVE:
			break;
		case MOUSE_CLICK_LEFT:
			if (focus != trigger)
			{
				focus = trigger;
				app->input->startTextInput();
				i->sendUIinputBox();
				i->text.setText("");
				i->active = true;				
			}
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
		if (u->data->isMouseIn(p) && u->data->interactable)
			return u->data;
		u = u->previous;
	}

	return screen;
}
