#include "network.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// this is for avoid order in windows include
#include "serial_port_win.h"
#include "socket_win.h"
#else
#include "serial_port_unix.h"
#include "socket_linux.h"
#endif


namespace gpl
{
	// COM
	COM::COM(const std::string& name) : m_name(name) {};

	void COM::received() 
	{
		// To override on ComImpl
	};

	void COM::send(const std::string& msg)
	{
		// To override on ComImpl
	};

	std::string COM::getComStatus()
	{
		return ComStateToString(m_state);
	}

	std::string COM::popLastBuffer()
	{
		std::string buffer = m_receivedBuffers.front();
		m_receivedBuffers.pop();
		return buffer;
	}

	void COM::popLastBuffer(std::string& sBuffer)
	{
		if(!m_receivedBuffers.empty())
		{
			while (m_flag.test_and_set()) {};
			sBuffer = m_receivedBuffers.front();
			m_receivedBuffers.pop();
			m_flag.clear();
		}
	}

	//	free function
	std::string ComStateToString(const COM_STATE error)
	{
		std::string retStr{};
		switch (error)
		{
		case(COM_STATE::COM_READY):
			retStr = "Ready";
			break;
		case(COM_STATE::POLLING_FROM):
			retStr = "In polling receiving";
			break;
		case(COM_STATE::POLLING_TO):
			retStr = "In polling sending";
			break;
		case(COM_STATE::ERROR_OPENING_SERIAL_PORT):
			retStr = "Opening serial port error";
			break;
		case(COM_STATE::GETTING_STATE_ERROR):
			retStr = "Getting state error";
			break;
		case(COM_STATE::RECEIVING_ERROR):
			retStr = "Receiving error";
			break;
		case(COM_STATE::SENDING_ERROR):
			retStr = "Sending error";
			break;
		case(COM_STATE::SERIAL_PORT_DOESNT_EXIST):
			retStr = "Serial port doesn't exist";
			break;
		case(COM_STATE::SETTING_STATE_ERROR):
			retStr = "Setting state error";
			break;
		case(COM_STATE::BUILDING_DCB_ERROR):
			retStr = "Building DCB error";
			break;
		case(COM_STATE::SETTING_TIMEOUT_ERROR):
			retStr = "Setting timeout error";
			break;
		case(COM_STATE::UNKNOWN):
			retStr = "Unknown error";
			break;
		default:
			break;
		}
		return retStr;
	}

	COM* createComPort(const std::string& name)
	{
		return createComPortOS(name);
	}

	// Socket constructor is demanded on the os specific header	
	Socket::~Socket() 
	{
		if (m_pSocket)
		{
			delete m_pSocket;
		}
	};

	void Socket::send(const std::string& msg)
	{
		m_pSocket->sendMessage(msg);
	}

	void Socket::connect(const size_t retry)
	{
		m_pSocket->tryConnect(retry);
	}

	void Socket::getMessage(std::string& message)
	{
		m_pSocket->getMessage(message);
	}

	void Socket::update(const Event& event)
	{
		notify(event);
	}

	

}