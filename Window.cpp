#include "Globals.h"
#include "Application.h"
#include "Window.h"

#include "SDL/include/SDL.h"


Window::Window() : Module()
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::awake()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		title.create(WINDOW_TITLE);
		Uint32 flags = SDL_WINDOW_SHOWN;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		scale = SCALE;

		if(R_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(R_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(R_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(R_FULLSCR_WINDOWED == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool Window::cleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::setTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void Window::getWindowSize(unsigned int& width, unsigned int& height) const
{
	width = this->width;
	height = this->height;
}

unsigned int Window::getScale() const
{
	return scale;
}