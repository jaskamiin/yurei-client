#pragma once

#include <memory>
#include "yurei_utils.h"


/*Includes for [non] Windows*/
	#ifdef _WIN32
		#ifndef _WIN32_WINNT
			#define _WIN32_WINNT 0x0501  /* Windows XP. */
		#endif
		#include <winsock2.h>
		#include <Ws2tcpip.h>
		#pragma comment(lib,"ws2_32.lib")
	#else
		#include <sys/socket.h>
		#include <arpa/inet.h>
		#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
		#include <unistd.h> /* Needed for close() */
	#endif

class Client
{
private:
	int sock_init();
	int sock_quit();
	int sock_close(SOCKET);

	SOCKET* sockets;

	class TCPSocket
	{
	private:
		void openSocket(addrinfo*);
		void setInfo(std::string, int);
		void setInfo(int);

		addrinfo	info;

		int			sock			= -1;
		bool		sockOpen		= false;
		bool		sockCreated		= false;
		bool		sockBound		= false;
		bool		sockConnected	= false;

	public:
		TCPSocket();
		TCPSocket(int, int);
		TCPSocket(int, addrinfo, bool, bool);
		virtual ~TCPSocket();

		TCPSocket(const TCPSocket& socket) = delete;
		TCPSocket &operator=(const TCPSocket& socket) = delete;

		void bind(int);
		void connect(std::string, int);
		void listen(int);
		std::shared_ptr<TCPSocket> accept();
		void send(const char*, unsigned, int);
		bool receive(char*, int, int);
		void close();
	};

public:
	Client();
	~Client();

	int start();

};