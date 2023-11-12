#pragma once

#include "GPL.h"

#include <filesystem>

namespace gpl
{
	class GPL_API ProcessHandler
	{
	public:
		static ProcessHandler* createProcessHandler();	// Factory method
		~ProcessHandler();
		virtual bool isStillRunning() = 0;
		virtual void createProcess(std::filesystem::path& executable, const bool isConsole = false) = 0;
		virtual void killProcess() = 0;
	protected:
		ProcessHandler();
	};
}