#pragma once

#include "console.h"

#include <Windows.h>
#include <conio.h>

namespace gpl
{
	void Console::changeColor(ConsoleColor color)  const 
	{
		WORD c = 0U;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (color)
		{
		case gpl::ConsoleColor::DEFAULT:
			break;
		case gpl::ConsoleColor::NATIVE:
			c = 7U;
			break;
		case gpl::ConsoleColor::GREY:
			c = 8U;
			break;
		case gpl::ConsoleColor::BLUE:
			c = 9U;
			break;
		case gpl::ConsoleColor::GREEN:
			c = 10U;
			break;
		case gpl::ConsoleColor::AZURE:
			c = 11U;
			break;
		case gpl::ConsoleColor::RED:
			c = 12U;
			break;
		case gpl::ConsoleColor::MAGENTA:
			c = 13U;
			break;
		case gpl::ConsoleColor::YELLOW:
			c = 14U;
			break;
		case gpl::ConsoleColor::ORANGE:
			c = FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		default:
			break;
		}

		SetConsoleTextAttribute(hConsole, c);
	}
}