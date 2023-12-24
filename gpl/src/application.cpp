#include "application.h"

#include <iostream>

namespace gpl
{
	// Public methods
	Application::Application(const std::string nameApp) : m_appName(nameApp), m_logger(std::filesystem::current_path(), nameApp, "log")
	{		
		m_helpMsg =
		{
			"Hello everyone, this is an " + m_appName + "\n"
			"These are options for lunch\n"
			"[-h | --help]\t print an usage message\n"
			"[-d | --debug]\t set debug mode while computing\n"
			"During the run there are these commands avaible:\n"
			"[e | exit]\t to exit the application\n"
			"[h | help]\t to print the help menu\n"
		};
	}

	void Application::init(int argc, char* argv[])
	{
		if (argc > 1)
		{
			consoleCommandInit(argc, argv);
		}
		else
		{
			configFileInit(argv[0]);
		}
	}

	void Application::run()
	{
		printHelp();
		std::string inp = "";
		m_bRun = true;
		while (m_bRun)
		{
			inp = m_console.getInput();
			processInput(inp);
		}
	}

	void Application::exit()
	{
		// Stop the run.
		m_bRun = false;
	}

	// Private methods
	bool Application::processDefaultInput(const std::string inp)
	{
		bool bRecognized = false;
		if (inp == "h" || inp == "help")
		{
			bRecognized = true;
			printHelp();
		}
		else if (inp == "e" || inp == "exit")
		{
			bRecognized = true;
			exit();
		}
		else if (inp.empty())
		{
			bRecognized = true;
		}
		return bRecognized;
	}

	void Application::addHelpMessageLine(const std::string& shortCmd, const std::string& longCmd, const std::string& description, const std::string& inputs)
	{
		if (!shortCmd.empty())
		{
			if (inputs.empty())
			{
				m_helpMsg += "[" + shortCmd + " | " + longCmd + "]\t" + description + "\n";
			}
			else
			{
				m_helpMsg += "[" + shortCmd + " | " + longCmd + " <" + inputs + ">" + "]\t" + description + "\n";
			}
		}
		else
		{
			if (inputs.empty())
			{
				m_helpMsg += "[" + longCmd + "]\t" + description + "\n";
			}
			else
			{
				m_helpMsg += "[" + longCmd + " <" + inputs + ">" + "]\t" + description + "\n";
			}
		}
	}

	bool Application::findCommand(std::string inp, std::string shortName, std::string longName)
	{
		if ((shortName.length() == 1U) && (shortName.compare(0, 1, inp, 0, 1) == 0) || (inp.find(longName) != std::string::npos))
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	void Application::configFileInit(std::filesystem::path configFilePath)
	{
		// ToDo: read from config file all app settings.
	}

	void Application::consoleCommandInit(int argc, char* argv[])
	{
		if (argc == 0U)
		{
			configFileInit(argv[0]);
		}
		else
		{
			for (auto i = 1U; i < argc; ++i)
			{
				std::string command{ argv[i] };
				if (command == "-h" || command == "-help")
				{
					printHelp();
				}
				else if (command == "-d" || command == "--debug")
				{
					m_bDebugMode = true;
				}
				else
				{
					printHelp();
				}
			}
		}
	}

	void Application::printHelp()
	{
		m_console.print(m_helpMsg);
	}

	void Application::printNotValidCommand()
	{
		m_console.print("Command not recognised", gpl::ConsoleColor::RED);
	}

	void Application::printNotValidParam(const std::string& command) const
	{
		m_console.print("Param not valid for command " + command, gpl::ConsoleColor::RED);
	}

	template<>
	bool Application::getParams<std::string>(const std::string& inp, const std::string commandName, std::string& param)
	{
		// get params
		param = inp.substr((commandName + " ").size());
		return !param.empty();
	}
}
