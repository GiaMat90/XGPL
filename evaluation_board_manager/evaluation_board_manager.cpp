#include "evaluation_board_manager.h"

#include <Windows.h>


EvaMan::EvaMan(std::string name) : Application(name), MasterConsole("C:/Workspace/GiaMat90/XGPL/cmakebuild/Debug/SlavePrompt.exe")
{	
	m_console.setColor(gpl::ConsoleColor::AZURE);

	addHelpMessageLine("", "log on", "Activate log on the nucleo. These are saved on ", "");
	addHelpMessageLine("", "log off", "Deactivate logging from the nucleo", "");
	addHelpMessageLine("", "set flow", "Set the flow", "double");
	addHelpMessageLine("", "start pid", "Start the flow in closed loop", "");
	addHelpMessageLine("", "stop pid", "Stop the flow in closed loop", "");
	addHelpMessageLine("", "set pid", "Set the parameters for the closed loop (Kp, Ki, Kd)", "double, double, double");
	addHelpMessageLine("", "add procedure", "Set the procedure", "double,double double,double");
	addHelpMessageLine("", "reset procedure", "Clear the old procedure", "");
	addHelpMessageLine("", "get com status", "Print the status of the communication with the nucleo", "");

	m_pComPort = gpl::createComPort("COM7");
	startSlaveConsole();
}

EvaMan::~EvaMan()
{
	if (m_pComPort && m_pComPort->isPolling())
	{
		m_pComPort->stopPolling();
	}
	if (m_receivedThread.joinable())
	{
		m_receivedThread.join();
	}
	if (m_writeCSVThread.joinable())
	{
		m_writeCSVThread.join();
	}
	if (m_pComPort)
	{
		delete m_pComPort;
	}
}

void EvaMan::processInput(const std::string inp)
{
	if (!Application::processDefaultInput(inp))
	{
		if (findCommand(inp, "", "log on"))
		{
			logHandle(true);
		}
		else if (findCommand(inp, "", "log off"))
		{
			logHandle(false);
		}
		else if (findCommand(inp, "", "send"))
		{
			std::string param = "";
			if (getParams(inp, "send", param))
			{
				m_pComPort->send(param);
			}
			else
			{
				printNotValidParam("send");
			}
		}
		else if (findCommand(inp, "", "get com status"))
		{
			m_console.print(m_pComPort->getComStatus(), gpl::ConsoleColor::GREEN);
		}
		else if (findCommand(inp, "", "set volts"))
		{
			std::string param = "";
			if (getParams(inp, "set volts", param))
			{
				m_pComPort->send("set_volts" + param);
			}
		}
		else
		{
			printNotValidCommand();
			printHelp();
		}
	}	
}

void EvaMan::logHandle(const bool active)
{
	if (active)
	{
		m_pComPort->startPolling();
		// avoid to recreate a thread already running
		if (!m_receivedThread.joinable())
		{
			m_receivedThread = std::thread(&gpl::COM::received, m_pComPort);
		}
		if (!m_writeCSVThread.joinable())
		{
			m_writeCSVThread = std::thread(&EvaMan::writeCSV, this);
		}
	}
	else
	{
		m_pComPort->stopPolling();
		m_receivedThread.join();
		m_writeCSVThread.join();
	}
}

void EvaMan::writeCSV()
{
	while (m_pComPort->isPolling())
	{
		std::string line{};
		m_pComPort->popLastBuffer(line);
		if (!line.empty())
		{
			try
			{
				m_csv.writeLine(line);
				if (line.back() == '\n')
				{
					line = line.substr(0, line.size() - 1);
				}
				m_socket.send("print " + line);
			}
			catch (std::exception& e)
			{
				m_pComPort->stopPolling();
				m_pComPort->send("log off");
				m_console.print("Exception" + std::string(e.what()) + " during csv file open, log switched off", gpl::ConsoleColor::RED);
			}
		}
	}
}
