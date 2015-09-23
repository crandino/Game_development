#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KS_IDLE = 0,
	KS_DOWN,
	KS_REPEAT,
	KS_UP
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();

	// Called before quitting
	bool cleanUp();

	// Gather relevant win events
	bool GetWindowEvent(EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	bool GetKeyDown(int code);
	bool GetKeyRepeat(int code);
	bool GetKeyUp(int code);

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	bool GetMouseButtonDown(int code);
	bool GetMouseButtonRepeat(int code);
	bool GetMouseButtonUp(int code);

private:
	void CleanKeys();

private:
	bool		windowEvents[WE_COUNT];
	KeyState	keyState[NUM_KEYS];
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;
};

#endif // __INPUT_H__
