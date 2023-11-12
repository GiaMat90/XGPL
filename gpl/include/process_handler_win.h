#pragma once

#include "GPL.h"
#include "process_handler.h"

#include <Windows.h>

namespace gpl
{
	class GPL_API ProcessHandlerWin : public ProcessHandler
	{
	public:
		ProcessHandlerWin();
		~ProcessHandlerWin();
		// overrided methods
		bool isStillRunning() override;
		void killProcess() override;
		void createProcess(std::filesystem::path& executable, const bool isConsole = false) override;
	private:
		HANDLE m_hProcess = NULL;
		int m_exitCode{};
	};
}