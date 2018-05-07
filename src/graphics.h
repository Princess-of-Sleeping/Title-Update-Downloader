#pragma once

typedef unsigned char u8;
typedef unsigned u32;
typedef u32 Color;

// allocates memory for framebuffer and initializes it
void psvDebugScreenInit();

// clears screen with a given color
void psvDebugScreenClear(int bg_color);

// printf to the screen
void psvDebugScreenPrintf(const char *format, ...);

void *psvDebugScreenGetVram();
int psvDebugScreenGetX();
int psvDebugScreenGetY();
void psvDebugScreenSetXY(int x, int y);

// set foreground (text) color
Color psvDebugScreenSetFgColor(Color color);

// set background color
Color psvDebugScreenSetBgColor(Color color);

enum {

/*
	COLOR_CYAN = 0xFFFFFF00,
	COLOR_WHITE = 0xFFFFFFFF,
	COLOR_BLACK = 0xFF000000,
	COLOR_RED = 0xFF0000FF,
	COLOR_YELLOW = 0xFF00FFFF,
	COLOR_GREY = 0xFF808080
*/



	COLOR_BLACK	= 0xFF000000,
	COLOR_RED	= 0xFF0000FF,
	COLOR_GREEN	= 0xFF00FF00,
	COLOR_YELLOW	= 0xFF00FFFF,
	COLOR_BLUE	= 0xFFFF0000,
	COLOR_MAGENTA	= 0xFFFF00FF,
	COLOR_CYAN	= 0xFFFFFF00,
	COLOR_WHITE	= 0xFFFFFFFF,
	COLOR_GREY	= 0xFF808080

};
