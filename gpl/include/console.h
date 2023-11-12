#pragma once

#include "GPL.h"

#include <iostream>
#include <string>
#include <mutex>

namespace gpl
{
	enum class ConsoleColor 
	{
		DEFAULT,
		NATIVE,
		GREY,
		BLUE,
		GREEN,
		AZURE,
		RED,
		MAGENTA,
		YELLOW,
		ORANGE
	};	

	class GPL_API Console
	{
	public:
		Console(ConsoleColor c = ConsoleColor::NATIVE, std::string prompt = "", bool autoEndl = true) :
			m_color(c), m_prompt(prompt), m_bAutoEndl(autoEndl) { changeColor(m_color); };
		~Console() { changeColor(ConsoleColor::NATIVE); };
		// template method
		template<typename T>
		void print(T s, ConsoleColor color = ConsoleColor::DEFAULT) const
		{
			//std::lock_guard<std::mutex> lock(m_consoleMutex);
			if (color != ConsoleColor::DEFAULT)
			{
				changeColor(color);
			}
			std::cout << std::boolalpha;
			if (m_bAutoEndl)
			{
				std::cout << m_prompt << s << std::endl;
			}
			else
			{
				std::cout << m_prompt << s;
			}
			changeColor(m_color);
		};
		//
		std::string getInput(ConsoleColor color = ConsoleColor::DEFAULT);
		void setColor(const ConsoleColor& color);
	private:
		ConsoleColor m_color;
		bool m_bAutoEndl;
		std::string m_prompt = "";
		std::mutex m_consoleMutex;
		void printPrompt(ConsoleColor color = ConsoleColor::DEFAULT);
		void changeColor(ConsoleColor color) const;
	};
}
