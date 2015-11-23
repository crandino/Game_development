#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "Input.h"
#include "Gui.h"

Gui::Gui() : Module()
{
	name.create("gui");
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
	return true;
}

// Update all guis
bool Gui::preUpdate()
{
	return true;
}

// Called after all Updates
bool Gui::postUpdate()
{
	doubleNode<UIelement*> *item = UIelement_list.getFirst();
	while (item != NULL)
	{
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

	return true;
}

// const getter for atlas
const SDL_Texture* Gui::getAtlas() const
{
	return atlas;
}

// Factories for class Gui ---------------------------------------------------
const UIlabel* Gui::createLabel(const char *string, iPoint pos)
{
	UIlabel *ret = new UIlabel();
	ret->text = string;
	ret->text_image = app->fonts->print(string);
	ret->pos = pos;

	UIelement_list.add(ret);
	return ret;
}

const UIimage* Gui::createImage(SDL_Texture *tex, iPoint pos, SDL_Rect &r)
{
	UIimage *ret = new UIimage();
	if (tex != NULL)
		ret->tex = tex;
	else
		ret->tex = (SDL_Texture*)app->gui->getAtlas();

	ret->pos = pos;
	ret->rect = r;	
	UIelement_list.add(ret);
	return ret;
}

// UI Label
UIlabel::UIlabel()
{
	pos.setZero();
	type = UI_LABEL;
	text = NULL;
	text_image = NULL;	
}

bool UIlabel::draw()
{
	app->render->blit(text_image, pos.x, pos.y);
	return true;
}

// UI image
UIimage::UIimage()
{
	pos.setZero();
	rect = { 0, 0, 0, 0 };
	type = UI_IMAGE;
	tex = NULL;
}

bool UIimage::draw()
{
	app->render->blit(tex, pos.x, pos.y, &rect);
	return true;
}

