/*
Used some code from these sources
http://cecilsunkure.blogspot.com/2011/11/windows-console-game-event-handling.html
https://github.com/axthomas-aie/CppSideProjects-AIE
*/


#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>

#define WIDTH 80
#define HEIGHT 40

char map[WIDTH * HEIGHT];
char current = 'w';

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

INPUT_RECORD* eventBuffer;
DWORD numEventsRead;

struct vec2
{
	int x;
	int y;
};

vec2 mouse{ 0, 0 };
vec2 lastPos{ 0,0 };
bool lclick = false;
bool rclick = false;

/* Read console input buffer and return malloc'd INPUT_RECORD array */
DWORD getInput(INPUT_RECORD **eventBuffer)
{
	/* Variable for holding the number of current events, and a point to it */
	DWORD numEvents = 0;


	/* Variable for holding how many events were read */
	DWORD numEventsRead = 0;


	/* Put the number of console input events into numEvents */
	GetNumberOfConsoleInputEvents(rHnd, &numEvents);


	if (numEvents) /* if there's an event */
	{
		/* Allocate the correct amount of memory to store the events */
		*eventBuffer = (INPUT_RECORD*)malloc(sizeof(INPUT_RECORD) * numEvents);

		/* Place the stored events into the eventBuffer pointer */
		ReadConsoleInput(rHnd, *eventBuffer, numEvents, &numEventsRead);
	}


	/* Return the amount of events successfully read */
	return numEventsRead;
}

void outputMapFromMem(std::string fileName)
{
	std::string outputMapString;
	std::ofstream output;
	char outputCharA[WIDTH * HEIGHT] = { ' ' };

	output.open(fileName);

	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			outputCharA[x] = map[x + WIDTH * ((HEIGHT - 1) - y)];
		}
		output << outputCharA << "\n";
	}
	output.close();
}

int main(void)
{
	int x, y;

	srand(time(0));

	/* Window size coordinates, be sure to start index at zero! */
	SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };

	/* A COORD struct for specificying the console's screen buffer dimensions */
	COORD bufferSize = { WIDTH, HEIGHT };

	/* Setting up different variables for passing to WriteConsoleOutput */
	COORD characterBufferSize = { WIDTH, HEIGHT };
	COORD characterPosition = { 0, 0 };
	SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

	/* A CHAR_INFO structure containing data about a single character */
	CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

	/* initialize handles */
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	/* Set the console's title */
	SetConsoleTitle("Rougelike");

	/* Set the window size */
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			//consoleBuffer[x + WIDTH * y].Char.AsciiChar = '.';
			map[x + WIDTH * y] = '.';
		}
	}


	while (true) {

		numEventsRead = getInput(&eventBuffer);

		for (int i = 0; i < numEventsRead; ++i) 
		{
			switch (eventBuffer[i].EventType)
			{
			case KEY_EVENT:

				if (eventBuffer[i].Event.KeyEvent.bKeyDown)
				{
					switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode)
					{
					case VK_NUMPAD0:
						for (y = 0; y < HEIGHT; ++y)
						{
							for (x = 0; x < WIDTH; ++x)
							{
								//consoleBuffer[x + WIDTH * y].Char.AsciiChar = '.';
								map[x + WIDTH * y] = '.';
							}
						}
						break;
					case VK_NUMPAD1:
						current = 'w';
						break;
					case VK_NUMPAD2:
						current = '@';
						break;
					case VK_NUMPAD3:
						break;
					case VK_NUMPAD4:
						break;
					case VK_NUMPAD5:
						break;
					case VK_NUMPAD6:
						break;
					case VK_NUMPAD7:
						break;
					case VK_NUMPAD8:
						break;
					case VK_NUMPAD9:
						break;
					case VK_RETURN:
						outputMapFromMem("test.txt");
						break;
					}
				}
			case MOUSE_EVENT:
				if (eventBuffer[i].Event.MouseEvent.dwButtonState == 0x0001)
				{
					lclick = true;
				}
				if (eventBuffer[i].Event.MouseEvent.dwButtonState == 0x0002)
				{
					rclick = true;
				}
				if (eventBuffer[i].Event.MouseEvent.dwEventFlags == 0x0001)
				{
					mouse.x = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					mouse.y = HEIGHT - 1 - eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
				}
				break;
			default:
				break;
			}
		}


		if (lclick) 
		{
			map[mouse.x + WIDTH * mouse.y] = current;
			lclick = false;
		}

		if (rclick) 
		{
			map[mouse.x + WIDTH * mouse.y] = '.';
			rclick = false;
		}

		char tempChar;
		for (y = 0; y < HEIGHT; ++y)
		{
			for (x = 0; x < WIDTH; ++x)
			{
				tempChar = map[x + WIDTH * ((HEIGHT - 1) - y)];
				consoleBuffer[x + WIDTH * y].Char.AsciiChar = tempChar;
				consoleBuffer[x + WIDTH * y].Attributes = 15;
			}
		}

		//set last pos
		consoleBuffer[(lastPos.x + WIDTH * ((HEIGHT - 1) - lastPos.y))].Char.AsciiChar = '.';
		consoleBuffer[(lastPos.x + WIDTH * ((HEIGHT - 1) - lastPos.y))].Attributes = 15;
		//set current
		consoleBuffer[(mouse.x + WIDTH * ((HEIGHT - 1) - mouse.y))].Char.AsciiChar = current;
		consoleBuffer[(mouse.x + WIDTH * ((HEIGHT - 1) - mouse.y))].Attributes = 15;
		lastPos = { mouse.x, mouse.y };


		/* Write our character buffer (a single character currently) to the console buffer */
		WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	}
}