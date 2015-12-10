#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "UIelements.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

Input::Input() : Module()
{
	name.create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	input_box = NULL;

}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::awake(pugi::xml_node &node)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::preUpdate()
{
	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	mouse_motion_x = mouse_motion_y = 0;

	p2SString t;
	if( input_box != NULL )	
		t.create(input_box->text.text.GetString());
	bool UI_text_changed = false;	

	for (int i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_TEXTINPUT:
			/* Add new text onto the end of our text */	
			if (t.Length() < MAX_STRING_UI)
			{
				t += event.text.text;
				UI_text_changed = true;
				input_box->cursor_index++;
			}		
			break;

		case SDL_MOUSEMOTION:
			int scale = app->win->getScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;		
		}
	}

	if (UI_text_changed)
	{
		input_box->text.setText(t.GetString());
		for (doubleNode<Module*> *item = input_box->mod_listeners.getLast(); item != NULL; item = item->previous)
		{
			item->data->onGui(TEXT_CHANGED, input_box);
		}
	}
	
	return true;
}

// Called before quitting
bool Input::cleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool Input::getWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::getMousePosition(iPoint &p) const
{
	p.x = mouse_x;
	p.y = mouse_y;
	/*y = mouse_y;
	x = mouse_x;*/
}

iPoint Input::getMouseMotion()
{
	return iPoint(mouse_motion_x, mouse_motion_y);
	/*x = mouse_motion_x;
	y = mouse_motion_y;*/
}

// Input text methods for GUI
void Input::startTextInput(UIinputBox* u)
{
	if (!SDL_IsTextInputActive())
		SDL_StartTextInput();
	input_box = u;
}

void Input::stopTextInput()
{
	if (SDL_IsTextInputActive())
		SDL_StopTextInput();
	input_box = NULL;
}