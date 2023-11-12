#pragma once

#include "gpl.h"
#include "publisher.h"

#include <string>
#include <queue>
#include <atomic>
#include <memory>

// Windows case
#ifdef _WIN32

namespace gpl
{
	enum class COM_STATE
	{
		COM_READY,
		POLLING_FROM,
		POLLING_TO,
		ERROR_OPENING_SERIAL_PORT,
		SERIAL_PORT_DOESNT_EXIST,
		SETTING_STATE_ERROR,
		GETTING_STATE_ERROR,
		BUILDING_DCB_ERROR,
		SETTING_TIMEOUT_ERROR,
		RECEIVING_ERROR,
		SENDING_ERROR,
		UNKNOWN
	};

	class GPL_API COM
	{
	public:
		COM(const std::string& name = "");
		virtual ~COM() {};
		virtual void received();
		virtual void send(const std::string& msg = "");
		// getters
		std::string getComStatus();
		std::string popLastBuffer();	
		void popLastBuffer(std::string& sBuffer);	
		// inline methods
		bool isPolling() { return m_bPollingActive; };
		void startPolling() { m_bPollingActive = true; };
		void stopPolling() { m_bPollingActive = false; };
	protected:
		std::string m_name{};
		COM_STATE m_state  = COM_STATE::COM_READY;
		std::queue<std::string> m_receivedBuffers{};
		bool m_bPollingActive{ false };
		std::atomic_flag m_flag{ATOMIC_FLAG_INIT};
	};

	GPL_API std::string ComStateToString(const COM_STATE state);
	GPL_API COM* createComPort(const std::string& name);

	class SocketPrototype;

	enum class SocketType 
	{
		UNKNOWN,
		CLIENT,
		SERVER
	};

	enum class ProtocolType
	{
		TCP,
		UDP
	};

	class GPL_API SocketEvent : public Event
	{
	public:
		enum Type : int
		{
			RECEIVED_MESSAGE = 1
		};
		SocketEvent(Type type) : Event(m_type) {};
		~SocketEvent() {};
	};

	class GPL_API Socket : public Publisher, public Listener
	{
	public:
		Socket(const std::string& address = "", const std::string& port = "", const SocketType& type = SocketType::UNKNOWN, const ProtocolType& protocol = ProtocolType::TCP);
		~Socket();
		void connect(const size_t retry);
		void send(const std::string& msg);
		void getMessage(std::string& message);
		void update(const Event& event) override;
	protected:
		SocketPrototype* m_pSocket;
	};

	class GPL_API SocketPrototype : public Publisher
	{
	public:
		SocketPrototype(const std::string& address = "", const std::string& port = "", const SocketType& socket_type = SocketType::UNKNOWN, const ProtocolType& protocol = ProtocolType::TCP) : 
			m_address(address), m_port(port), m_type(socket_type),  m_protocol(protocol) {};
		virtual ~SocketPrototype() {};
		virtual void tryConnect(const size_t retry) = 0;
		virtual void sendMessage(const std::string& msg) = 0;
		virtual void getMessage(std::string& message) = 0;
	protected:
		virtual void shutdownConnection() = 0;
		virtual void receiveLoop() = 0;
		virtual void WaitForOther() = 0;
	protected:
		std::string m_address{};
		std::string m_port{};
		SocketType m_type{ SocketType::UNKNOWN };
		ProtocolType m_protocol{ ProtocolType::TCP };
	};
}

#else
// put here the code for other OS
#endif