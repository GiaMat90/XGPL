#pragma once

#include "GPL.h"
#include "network.h"


// start-because windows sucks


#include <winsock2.h>	// this must be before windows.h
#include <ws2tcpip.h>
#include <windows.h>	// this must be after winsock2.h

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
// end-because windows sucks

// concurrency
#include <thread>
#include <mutex>

#define BUFLEN 512

namespace gpl
{
	enum class WinSocketStatus
	{
		NONE,
		INVALID,
		SOCKET_OK,
		WSA_STARTUP_FAILED, 
		GET_ADDRESS_ERROR,
		CONNECTION_FAILED,
		SEND_ERROR,
		ERROR_BINDING,
		ERROR_LISTEN,
		WAITING_FOR_CLIENTS,
		ACCEPT_CLIENT_ERROR,
		CONNECTION_CLOSED,
		SHUTDOWN_ERROR,
		UNKNOWN
	};

	class WinClient : public SocketPrototype
	{
	public:
		WinClient(const std::string& address, const std::string& port, const ProtocolType& protocol);
		~WinClient();
		void tryConnect(const size_t retry = 10u) override;
		void sendMessage(const std::string& msg) override;
		void getMessage(std::string& message) override;
	private:
		addrinfo* m_serverInfo{ NULL };
		SOCKET m_server{ INVALID_SOCKET };
		SOCKET m_client{ INVALID_SOCKET };
		WSADATA m_wsaData{};
		WinSocketStatus m_status = WinSocketStatus::NONE;
		std::thread m_comThread;	// thread of communication receive
		// sharing messages
		std::string m_msg{};
		std::mutex m_msgMutex;
	private:
		void shutdownConnection() override;
		void receiveLoop() override;
		void WaitForOther() override;
	};

	class WinServer : public SocketPrototype
	{
	public:
		WinServer(const std::string& address, const std::string& port, const ProtocolType& protocol);
		~WinServer();
		void tryConnect(const size_t retry = 10u) override;
		void sendMessage(const std::string& msg) override;
		void getMessage(std::string& message) override;
	private:
		addrinfo* m_serverInfo{ NULL };
		SOCKET m_server{ INVALID_SOCKET };
		SOCKET m_client{ INVALID_SOCKET };
		WSADATA m_wsaData{};
		WinSocketStatus m_status = WinSocketStatus::NONE;
		std::thread m_comThread;	// thread of communication receive
		// sharing messages
		std::string m_msg{};
		std::mutex m_msgMutex;
	private:
		void shutdownConnection() override;
		void receiveLoop() override;
		void WaitForOther() override;
	};
}