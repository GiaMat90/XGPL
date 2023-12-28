#include "slave_prompt.h"

using gpl::SocketEvent;

SlavePromptApp::SlavePromptApp(std::string name) : Application(name), SlaveConsole()
{
	m_socket.addListener(this);
}

void SlavePromptApp::processInput(const std::string inp)
{
	if (!Application::processDefaultInput(inp))
	{
		if (findCommand(inp, "p", "print"))
		{
			std::string s;
			if (getParams(inp, "print", s))
			{
				m_console.print(s);
			}
			else
			{
				printNotValidCommand();
				printHelp();
			}
		}
		else
		{
			printNotValidCommand();
			printHelp();
		}
	}	
}

void SlavePromptApp::update(const Event& event)
{
	if (event.getType() == SocketEvent::Type::RECEIVED_MESSAGE)
	{
		std::string s;
		m_socket.getMessage(s);
		processInput(s);
	}
}
