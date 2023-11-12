#pragma once

#include "GPL.h"
#include "network.h"
#include "publisher.h"

namespace gpl
{
	class GPL_API SlaveConsole : public Listener
	{
	public:
		SlaveConsole();
		~SlaveConsole();
		//void update(const Event& event) override;
	protected:
		Socket m_socket{ "127.0.0.1", "8080", SocketType::CLIENT };
	};
}