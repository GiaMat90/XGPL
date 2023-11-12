#include "process_handler_win.h"

#include <Windows.h>

namespace gpl
{
	ProcessHandlerWin::ProcessHandlerWin() : ProcessHandler()
	{

	}

	ProcessHandlerWin::~ProcessHandlerWin()
	{

	}

	// public overridded methods
	void ProcessHandlerWin::createProcess(std::filesystem::path& executable, const bool isConsole)
	{
		DWORD creationFlag = isConsole ? CREATE_NEW_CONSOLE : 0;
		STARTUPINFO si{ 0 };
		PROCESS_INFORMATION pi{ 0 };
		bool bResult = CreateProcess(executable.string().c_str(), NULL, NULL, NULL, FALSE, creationFlag, NULL, NULL, &si, &pi);
		m_hProcess = pi.hProcess;
	}

	void ProcessHandlerWin::killProcess()
	{
		TerminateProcess(m_hProcess, m_exitCode);
	}

	bool ProcessHandlerWin::isStillRunning()
	{
		return true;
	}

	// protected overrided methods
	ProcessHandler* ProcessHandler::createProcessHandler()
	{
		return new ProcessHandlerWin();
	}

}