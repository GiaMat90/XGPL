#pragma once

#include "gpl.h"
#include"process_handler.h"
#include "slave_console.h"

// standard library dependeces
#include <string>
#include <filesystem>

namespace gpl
{
	class GPL_API MasterConsole
	{
	public:
		MasterConsole(std::filesystem::path execPath);
		~MasterConsole();
	protected:
		virtual void startSlaveConsole();
		virtual void stopSlaveConsole();
	protected:
		ProcessHandler* m_pProcessHandler = nullptr;
		Socket m_socket{ "127.0.0.1", "8080", SocketType::SERVER };
		std::filesystem::path m_execPath{};

	};
}