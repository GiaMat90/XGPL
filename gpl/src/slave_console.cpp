#include "slave_console.h"

namespace gpl
{
	SlaveConsole::SlaveConsole()
	{
		m_socket.connect(10u);
	}

	SlaveConsole::~SlaveConsole()
	{

	}	
}
