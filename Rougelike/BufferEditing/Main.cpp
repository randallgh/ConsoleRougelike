#include <windows.h> /* for HANDLE type, and console functions */
#include <stdio.h> /* standard input/output */
#include <stdlib.h> /* included for rand */
#include <chrono>

#define WIDTH 70
#define HEIGHT 35

HANDLE wHnd; /* write (output) handle */
HANDLE rHnd; /* read (input handle */

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
	SetConsoleTitle("Our shiny new title!");

	/* Set the window size */
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	/* Set the screen's buffer size */
	SetConsoleScreenBufferSize(wHnd, bufferSize);


	while (true) {
		for (y = 0; y < HEIGHT; ++y)
		{
			for (x = 0; x < WIDTH; ++x)
			{
				consoleBuffer[x + WIDTH * y].Char.AsciiChar = (unsigned char)3;
				consoleBuffer[x + WIDTH * y].Attributes = rand() % 256;
			}
		}


		// int x, int y, char character, color
		consoleBuffer[0 + WIDTH * 1].Char.AsciiChar = (unsigned char)8;
		/*RGB VALUES*/
		consoleBuffer[0 + WIDTH * 1].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;


		/* Write our character buffer (a single character currently) to the console buffer */
		WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
		getchar();
	}
}