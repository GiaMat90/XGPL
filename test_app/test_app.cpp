#include "test_app.h"

#include <Windows.h>


TestApp::TestApp(std::string name) : Application(name)
{	
	m_console.setColor(gpl::ConsoleColor::AZURE);
	addHelpMessageLine("", "", "", "");
}

TestApp::~TestApp()
{
}

void TestApp::processInput(const std::string inp)
{
	if (!Application::processDefaultInput(inp))
	{
		if (findCommand(inp, "s", "long"))
		{
			//logHandle(true);
		}
		else
		{
			printNotValidCommand();
			printHelp();
		}
	}	
}