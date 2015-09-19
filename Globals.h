#ifndef __LOG_H__
#define __LOG_H__

#include <Windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
void log(const char file[], int line, const char* format, ...);

// Configuration

#define R_FULLSCREEN false
#define R_BORDERLESS false
#define R_RESIZABLE false
#define R_FULLSCR_WINDOWED false
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define WINDOW_TITLE "My Game"
#define SCALE 1

#endif // !__LOG_H__
