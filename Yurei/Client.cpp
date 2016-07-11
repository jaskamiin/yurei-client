#include "Client.h"
#include "Client.h"


/*
*
*	Nested TCPSocket implementation	
*	TODO : fix all `throw` clauses
*/

Client::TCPSocket::TCPSocket()
{
	memset(&info, 0, sizeof info);
	info.ai_family		= AF_UNSPEC;
	info.ai_socktype	= SOCK_STREAM;
}


Client::TCPSocket::TCPSocket(int family, int flags)
{
	TCPSocket();
	info.ai_family	= family;
	info.ai_flags	= flags;

	if (family == AF_UNSPEC) return;

	sock = socket(info.ai_family, info.ai_socktype, 0);

	if (sock == -1)	throw 0;

	sockCreated = true;
}

Client::TCPSocket::TCPSocket(int sock, addrinfo info, bool bound, bool connected)
{
	this->sock		= sock;
	this->info		= info;
	sockBound		= bound;
	sockConnected	= connected;
}

Client::TCPSocket::~TCPSocket()
{
	if (sockOpen) this->close();
}

void Client::TCPSocket::bind(int port)
{
	if (sockBound && sockConnected) throw 0;

	setInfo(port);

	addrinfo* result;

	for (result = &info; result != NULL; result = info.ai_next)
	{
		if (!sockCreated)
		{
			try {
				openSocket(result);
			} catch (int e) {
				continue;
			}
		}

		sockBound = ::bind(sock, result->ai_addr, result->ai_addrlen) == 0 ? true : false;
		if (sockBound) return;
	}

	throw 0;
}


void Client::TCPSocket::connect(std::string addr, int port)
{
	if (sockConnected) throw 0;

	setInfo(addr, htons(port));

	addrinfo* result;

	for (result = &info; result != NULL; result = info.ai_next)
	{
		if (!sockCreated)
		{
			try {
				openSocket(result);
			} catch (int e) {
				continue;
			}
		}

		sockConnected = ::connect(sock, result->ai_addr, result->ai_addrlen) == 0 ? true : false;
		if (sockConnected) return;
	}
	
	throw 0;
}


void Client::TCPSocket::listen(int max)
{
	bool is_listening = ::listen(sock, max) == 0 ? true : false;
}


std::shared_ptr<Client::TCPSocket> Client::TCPSocket::accept()
{
	union
	{
		sockaddr addr;
		sockaddr_in in;
		sockaddr_in6 in6;
		sockaddr_storage s;
	} addr;

	socklen_t addr_size = sizeof(sockaddr_storage);

	int newSock = ::accept(sock, (sockaddr*)&addr.s, &addr_size);
	if (newSock) throw 0;

	addrinfo info;
	memset(&info, 0, sizeof info);

	if (addr.s.ss_family == AF_INET)
		info.ai_family = AF_INET;
	else 
		info.ai_family = AF_INET6;
	
	info.ai_addr = new sockaddr(addr.addr);

	return std::shared_ptr<Client::TCPSocket>(new Client::TCPSocket(newSock, info, true, false));

}




/*
*
*	Main Client implementation
*
*/

Client::Client()
{
}


Client::~Client()
{
}


int Client::sock_init()
{
	#ifdef _WIN32
		WSADATA wsadata;
		return WSAStartup(MAKEWORD(1,1), &wsadata);
	#else
		return 0;
	#endif
}


int Client::sock_quit()
{
	#ifdef _WIN32
		return WSACleanup();
	#else
		return 0;
	#endif
}

int Client::sock_close(SOCKET sock)
{
	int status = 0;

	#ifdef _WIN32
		status = shutdown(sock, SD_BOTH);
		if (status == 0)
			status = closesocket(sock);
	#else
		status = shutdown(sock, SHUT_RDWR);
		if (status == 0)
			status = close(sock);
	#endif

	return status;
}

int Client::start()
{
	try 
	{
		if (this->sock_init() != 0)
			throw CLIENT_BAD_INIT;
	} catch (EXCEPTION e) {

	}
}