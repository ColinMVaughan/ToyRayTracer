#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

// x is the column, y is the row. The origin (0,0) is top-left.
void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void setConsoleColour(unsigned short colour)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	SetConsoleTextAttribute(hOut, colour);
}

void DrawProgressUpdate(float percent, float TimeEstimate)
{
	setCursorPosition(0,0);
	//std::cout << "                            ";
	setConsoleColour(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	std::cout << "Frame Progress: ";
	setConsoleColour(FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << percent << "%   ";



	int min = int(TimeEstimate/60);
	float sec = TimeEstimate - min * 60;

	std::cout <<"\nTime Left: " << min << " Minutes "<< sec<< " Seconds           ";




	setConsoleColour(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "\n[";
	for (float i = 0; i < 100.0f; i += 4.95f)
	{
		i < percent? setConsoleColour(FOREGROUND_GREEN | FOREGROUND_INTENSITY) 
			: setConsoleColour(FOREGROUND_RED | FOREGROUND_INTENSITY);

		std::cout << "=";
	}
	std::cout << "]";


	//std::cout << "\n[====================]";



	std::cout.flush();
}