#include "Communicator.h"

Communicator::Communicator()
{
}

Communicator::~Communicator()
{
}

void Communicator::WSAInitializer()
{
	WSADATA wsa_data = {};
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		//FAILED
	}
}

void Communicator::WSADestructor()
{
	WSACleanup();
}

/*************************************************************** UDP AREA ****************************************************************/
SOCKET Communicator::UDP::createSocket()
{
	Communicator::WSAInitializer();
	SOCKET udp_soc = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_soc == INVALID_SOCKET)
	{
		//FAILED
	}
	return udp_soc;
}

SOCKET Communicator::UDP::destroySocket(SOCKET soc)
{
	Communicator::WSADestructor();
	closesocket(soc);
	return INVALID_SOCKET;
}

void Communicator::UDP::Send(SOCKET udp_soc, int port, const void *info, int len)
{
	if (udp_soc != INVALID_SOCKET)
	{
		sockaddr_in to;
		to.sin_family = AF_INET;
		InetPton(AF_INET, CC_IP, &(to.sin_addr));
		to.sin_port = htons(port);
		int h = sendto(udp_soc, (const char*)info, len, 0, (sockaddr *)&to, sizeof(to));
		if (h == -1)
		{
			std::cout << GetLastError() << std::endl;
		}
	}
}

/*************************************************************** TCP AREA ****************************************************************/
SOCKET Communicator::TCP::createSocket()
{
	Communicator::WSAInitializer();
	SOCKET tcp_soc = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (tcp_soc == INVALID_SOCKET)
	{
		//FAILED
	}
	return tcp_soc;
}

void Communicator::TCP::bindAndListen(SOCKET tcp_soc, const int listen_port)
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(listen_port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = 0;

	if (bind(tcp_soc, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		// FAILED
	}

	if (listen(tcp_soc, SOMAXCONN) == SOCKET_ERROR)
	{
		// FAILED
	}
}

SOCKET Communicator::TCP::acceptClient(SOCKET server_soc)
{
	SOCKET client_socket = accept(server_soc, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		return INVALID_SOCKET;// FAILED
	}
	return client_socket;
}

SOCKET Communicator::TCP::connect(SOCKET tcp_soc, char server_ip[16], int server_port)
{
	sockaddr_in to;
	to.sin_family = AF_INET;
	InetPton(AF_INET, server_ip, &(to.sin_addr));
	to.sin_port = htons(server_port);
	SOCKET ret = ::connect(tcp_soc, (sockaddr*)&to, sizeof(to));
	if (ret == SOCKET_ERROR)
	{
		// ERROR 
	}
	return ret;
}

Communicator::Message Communicator::TCP::recvFromClient(SOCKET client_soc)
{
	char data[RECV_TCP_BUFFER_SIZE + 1] = { 0 };// needs to malloc
	int res = recv(client_soc, data, RECV_TCP_BUFFER_SIZE, 0/*flags*/);

	if (res == INVALID_SOCKET)
	{
		return "";// ERROR
	}
	data[RECV_TCP_BUFFER_SIZE] = 0;
	return Communicator::Message(data);
}

void Communicator::TCP::Send(SOCKET tcp_soc, const char * info, int len)
{
	if (tcp_soc != INVALID_SOCKET)
	{
		int h = send(tcp_soc, info, len, 0);
		if (h == -1)
		{
			std::cout << GetLastError() << std::endl;
		}
	}
}

SOCKET Communicator::TCP::destroySocket(SOCKET soc)
{
	Communicator::WSADestructor();
	closesocket(soc);
	return INVALID_SOCKET;
}

/********************************************************** MESSAGE AREA ****************************************************************/
Communicator::Message::Message(char * data)
{
	this->data = data;
}

Communicator::Message::~Message()
{
}

std::string Communicator::Message::getData()
{
	return this->data;
}
