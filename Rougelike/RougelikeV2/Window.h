#pragma once
#include <windows.h>

//RENDERING
int const WIN_WIDTH = 80; //80
int const WIN_HEIGHT = 50; //50

int const MAP_HEIGHT = 40; //Uses the same width as the screen
int const UI_HEIGHT = 10; //Uses the same width as the screen

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

			 /* Window size coordinates, be sure to start index at zero! */
SMALL_RECT windowSize = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };

/* A COORD struct for specificying the console's screen buffer dimensions */
COORD bufferSize = { WIN_WIDTH, WIN_HEIGHT };

/* Setting up different variables for passing to WriteConsoleOutput */
COORD characterBufferSize = { WIN_WIDTH, WIN_HEIGHT };
COORD characterPosition = { 0, 0 };
SMALL_RECT consoleWriteArea = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };

/* A CHAR_INFO structure containing data about a single character */
CHAR_INFO consoleBuffer[WIN_WIDTH * WIN_HEIGHT];

/*---Window Handle---*/
HWND hWnd;

void initWindow() 
{
	/* initialize handles */
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title */
	SetConsoleTitle("Rougelike");

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	/* Set the window size */
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/*---Turn off Cursor---*/

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(wHnd, &cursorInfo);
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(wHnd, &cursorInfo);

	/*---Move Window---*/
	//hWnd = GetActiveWindow();
	//MoveWindow(hWnd, -10, -10, WIDTH, HEIGHT, TRUE);
	//SetWindowPos(hWnd, hWnd, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW);
}