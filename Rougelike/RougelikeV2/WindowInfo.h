#pragma once
#include <windows.h>

//RENDERING
int const WIDTH = 80; //70
int const HEIGHT = 50; //35

int const MAPHEIGHT = 40; //Uses the same width as the screen
int const UIHEIGHT = 10; //Uses the same width as the screen

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

					/* Window size coordinates, be sure to start index at zero! */
SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* A COORD struct for specificying the console's screen buffer dimensions */
COORD bufferSize = { WIDTH, HEIGHT };

/* Setting up different variables for passing to WriteConsoleOutput */
COORD characterBufferSize = { WIDTH, HEIGHT };
COORD characterPosition = { 0, 0 };
SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

/* A CHAR_INFO structure containing data about a single character */
static CHAR_INFO consoleBuffer[WIDTH * HEIGHT];


class WindowInfo
{
public:

	WindowInfo();
	~WindowInfo();
};
