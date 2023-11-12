#include "socket_win.h"

#include <stdlib.h>
#include <stdio.h>
#include <thread>


namespace gpl
{
	Socket::Socket(const std::string& address, const std::string& port, const SocketType& type, const ProtocolType& protocol)
	{
		if (SocketType::CLIENT == type)
		{
			m_pSocket = new WinClient(address, port, protocol);
		}
		else if (SocketType::SERVER == type)
		{
			m_pSocket = new WinServer(address, port, protocol);
		}
		else
		{
			m_pSocket = nullptr;
		}
		m_pSocket->addListener(this);
	}

	WinServer::WinServer(const std::string& address, const std::string& port, const ProtocolType& protocol) : SocketPrototype(address, port, SocketType::SERVER, protocol)
	{
		int retValue = 0;
		retValue = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (retValue != 0)
		{
			m_status = WinSocketStatus::WSA_STARTUP_FAILED;
		}

		addrinfo info{ 0 };
		info.ai_family = AF_INET;
		info.ai_socktype = SOCK_STREAM;
		if (ProtocolType::TCP == m_protocol)
		{
			info.ai_protocol = IPPROTO_TCP;
		}
		if (ProtocolType::UDP == m_protocol)
		{
			info.ai_protocol = IPPROTO_UDP;
		}
		info.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		retValue = getaddrinfo(m_address.c_str(), m_port.c_str(), &info, &m_serverInfo);
		if (retValue != 0) 
		{
			m_status = WinSocketStatus::GET_ADDRESS_ERROR;
			//printf("getaddrinfo failed with error: %d\n", retValue);
			WSACleanup();
		}

		//printf("Socket Server... ");
		m_server = socket(m_serverInfo->ai_family, m_serverInfo->ai_socktype, m_serverInfo->ai_protocol);
		if (m_server == INVALID_SOCKET)
		{
			//printf("socket failed with error: %ld\n", WSAGetLastError());
			m_status = WinSocketStatus::CONNECTION_FAILED;
			freeaddrinfo(m_serverInfo);
			m_server = NULL;
			m_serverInfo = NULL;
			WSACleanup();
		}
	}

	WinServer::~WinServer()
	{
		if (NULL != m_serverInfo)
		{
			freeaddrinfo(m_serverInfo);
		}
		if (NULL != m_server)
		{
			closesocket(m_server);
		}
		if (NULL != m_client)
		{
			closesocket(m_client);
		}
		if (m_comThread.joinable())
		{
			m_comThread.join();
		}
		WSACleanup();
		shutdownConnection();
	}

	void WinServer::tryConnect(const size_t retry)
	{
		//printf("Server connecting %d... ", m_server);

		int retValue = SOCKET_ERROR;

		// Setup the TCP listening socket
		retValue = bind(m_server, m_serverInfo->ai_addr, (int)m_serverInfo->ai_addrlen);
		if (retValue == SOCKET_ERROR) 
		{
			//printf(" ...bind failed with error: %d\n", WSAGetLastError());
			m_status = WinSocketStatus::ERROR_BINDING;
			closesocket(m_server);
			WSACleanup();
		}

		freeaddrinfo(m_serverInfo);
		m_serverInfo = NULL;

		retValue = listen(m_server, SOMAXCONN);
		if (retValue == SOCKET_ERROR) 
		{
			//printf(" ...listen failed with error: %d\n", WSAGetLastError());
			m_status = WinSocketStatus::ERROR_LISTEN;
			closesocket(m_server);
			WSACleanup();
		}		

		if (retValue != SOCKET_ERROR)
		{
			//printf(" ...connection ok!\n");
			m_comThread = std::thread(&WinServer::WaitForOther, this);
		}
	}

	void WinServer::shutdownConnection()
	{
		//shutdown connection
		int retValue = shutdown(m_client, SD_SEND);
		if (retValue == SOCKET_ERROR) {
			//printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(m_client);
			WSACleanup();
		}
	}

	void WinServer::WaitForOther()
	{
		//printf("Waiting for client... ");
		m_status = WinSocketStatus::WAITING_FOR_CLIENTS;
		// Accept a client socket
		sockaddr_storage clientAddress;
		socklen_t clientLen = sizeof(clientAddress);
		m_client = accept(m_server, (sockaddr*) &clientAddress, &clientLen);
		if (m_client == INVALID_SOCKET)
		{
			m_status = WinSocketStatus::ACCEPT_CLIENT_ERROR;
			//printf(" ...accept failed with error: %d\n", WSAGetLastError());
		}
		else
		{
			char clientBuffer[100];
			getnameinfo((sockaddr*)&clientAddress, clientLen, clientBuffer, sizeof(clientBuffer), 0, 0, NI_NUMERICHOST);
			//printf(" ...client detected: %s\n", clientBuffer);
		}

		// no need more server socket
		closesocket(m_server); 
		m_server = NULL;

		receiveLoop();
	}
	
	void WinServer::receiveLoop()
	{
		char recvbuf[BUFLEN];
		int recvbuflen = BUFLEN;
		int retValue{ 0 };
		int sendValue{ 0 };
		do {
			//printf("ServerRecv %lld\n", m_client);
			memset(recvbuf, 0, BUFLEN);
			retValue = recv(m_client, recvbuf, recvbuflen, 0);
 			if (retValue > 0) 
			{
				//printf("Bytes received: %d\n", retValue);
				m_msg.clear();
				m_msg = std::string(recvbuf);
				notify(Event(SocketEvent::RECEIVED_MESSAGE));
			}
			else if (retValue == 0)
			{
				m_status = WinSocketStatus::CONNECTION_CLOSED;
				//printf("Connection closing...\n");
			}
			else 
			{
				m_status = WinSocketStatus::CONNECTION_CLOSED;
				//printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(m_client);
				m_client = NULL;
				WSACleanup();
			}
		} while (retValue > 0);

		shutdownConnection();

		closesocket(m_client);
		m_client = NULL;
		WSACleanup();		
	}	
	
	void WinServer::sendMessage(const std::string& msg)
	{
		int retValue{ 0 };
		retValue = send(m_client, msg.c_str(), (int)strlen(msg.c_str()), 0);
		if (retValue == SOCKET_ERROR) 
		{
			m_status = WinSocketStatus::SEND_ERROR; // use WSAGetLastError() for more specific description
			//printf("send failed with error: %d\n", WSAGetLastError());               
			closesocket(m_client);
			WSACleanup();
		}
	}

	void WinServer::getMessage(std::string& message)
	{
		message = m_msg;
	}

	// Client Socket
	WinClient::WinClient(const std::string& address, const std::string& port, const ProtocolType& protocol) : SocketPrototype(address, port, SocketType::CLIENT, protocol)
	{
		int retValue = 0;
		retValue = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (retValue != 0)
		{
			m_status = WinSocketStatus::WSA_STARTUP_FAILED;
		}

		addrinfo info{ 0 };
		info.ai_family = AF_INET;
		info.ai_socktype = SOCK_STREAM;
		if (ProtocolType::TCP == m_protocol)
		{
			info.ai_protocol = IPPROTO_TCP;
		}
		if (ProtocolType::UDP == m_protocol)
		{
			info.ai_protocol = IPPROTO_UDP;
		}

		// Resolve the server address and port
		retValue = getaddrinfo(m_address.c_str(), m_port.c_str(), &info, &m_serverInfo);
		if (retValue != 0)
		{
			m_status = WinSocketStatus::GET_ADDRESS_ERROR;
			WSACleanup();
		}

		// Create a SOCKET for connecting to server
		//printf("Socket Client... ");
		m_client = socket(m_serverInfo->ai_family, m_serverInfo->ai_socktype, m_serverInfo->ai_protocol);
		if (m_client == INVALID_SOCKET)
		{
			//printf("socket failed with error: %ld\n", WSAGetLastError());
			m_status = WinSocketStatus::CONNECTION_FAILED;
			closesocket(m_client);
			m_client = NULL;
			WSACleanup();
		}
		else
		{
			//printf(" ...%lld Created correctly!\n", m_client);
		}	
	}

	WinClient::~WinClient()
	{
		if (NULL != m_serverInfo)
		{
			freeaddrinfo(m_serverInfo);
		}
		if (NULL != m_server)
		{
			closesocket(m_server);
		}
		if (NULL != m_client)
		{
			closesocket(m_client);
		}
		if (m_comThread.joinable())
		{
			m_comThread.join();
		}
		WSACleanup();
		shutdownConnection();
	}

	void WinClient::tryConnect(const size_t retry)
	{
		int retValue = SOCKET_ERROR;
		// Connect to server.
		//printf("Client Connecting %ld...", m_client);
		retValue = connect(m_client, m_serverInfo->ai_addr, (int)m_serverInfo->ai_addrlen);
		if (retValue == SOCKET_ERROR)
		{
			//printf(" ...connect failed with error: %d\n", WSAGetLastError());
			m_status = WinSocketStatus::INVALID;
			closesocket(m_client);
			m_client = INVALID_SOCKET;
		}
		else
		{
			//printf(" ..Connect ok!\n");
		}			

		freeaddrinfo(m_serverInfo);

		if (INVALID_SOCKET == m_client)
		{
			//printf("Unable to connect to server!\n");
			WSACleanup();
		}
		else if (SOCKET_ERROR != retValue)
		{
			m_comThread = std::thread(&WinClient::WaitForOther, this);
		}
	}
	
	void WinClient::shutdownConnection()
	{
		//shutdown connection
		int retValue = shutdown(m_client, SD_SEND);
		if (retValue == SOCKET_ERROR) 
		{
			//printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(m_client);
			m_client = NULL;
			WSACleanup();
		}
	}

	void WinClient::WaitForOther()
	{
		//printf("WaitForServer\n");
		receiveLoop();
	}

	void WinClient::receiveLoop()
	{
		char recvbuf[BUFLEN];
		int recvbuflen = BUFLEN;
		int retValue{ 0 };
		int sendValue{ 0 };
		do {
			//printf("ClientRecv %lld\n", m_client);
			memset(recvbuf, 0, BUFLEN);
			retValue = recv(m_client, recvbuf, recvbuflen, 0);
			if (retValue > 0)
			{
				//printf("Bytes received: %d\n", retValue);
				m_msg = std::string(recvbuf);
				notify(Event(SocketEvent::RECEIVED_MESSAGE));
			}
			else if (retValue == 0)
			{
				m_status = WinSocketStatus::CONNECTION_CLOSED;
				//printf("Connection closing...\n");
			}
			else
			{
				m_status = WinSocketStatus::CONNECTION_CLOSED;
				//printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(m_client);
				m_client = NULL;
				WSACleanup();
			}
		} while (retValue > 0);

		closesocket(m_client);
		m_client = NULL;
		WSACleanup();
	}

	void WinClient::sendMessage(const std::string& msg)
	{
		int retValue{ 0 };
		retValue = send(m_client, msg.c_str(), (int)strlen(msg.c_str()), 0);
		if (retValue == SOCKET_ERROR)
		{
			m_status = WinSocketStatus::SEND_ERROR; // use WSAGetLastError() for more specific description
			closesocket(m_client);
			m_client = NULL;
			WSACleanup();
		}
	}

	void WinClient::getMessage(std::string& message)
	{
		message = m_msg;
	}

}