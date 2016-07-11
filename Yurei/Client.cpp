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


void Client::TCPSocket::send(const char* data, unsigned len, int flags)
{
	const char* buffer = data;
	int status = 0;
	int total_sent = 0;
	int left_to_send = len;
	
	while (total_sent < len)
	{
		status = ::send(sock, buffer + total_sent, left_to_send, flags);

		if (status == -1) throw 0;

		total_sent += status;
		left_to_send -= status;
	}
}

bool Client::TCPSocket::receive(char* msg, int len, int flags)
{
	int status = ::recv(sock, msg, len, flags);

	if (status == -1) throw 0;
	
	return status == 0 ? false : true;
}


void Client::TCPSocket::close()
{
	if (sockOpen)
	{
		int toClose;
		#ifdef _WIN32
			toClose = ::closesocket(sock);
			if (toClose == -1) throw 0;
			sockOpen = false;
		#else
			toClose = ::close(sock);
			if (toClose == -1) throw 0;
			sockOpen = false;
		#endif

	}
}


void Client::TCPSocket::setInfo(int port)
{
	setInfo("null", port);
}


void Client::TCPSocket::setInfo(std::string addr, int port)
{
	const char* charAddr = (addr == "null") ? NULL : addr.c_str();
	
	addrinfo hints = info;
	addrinfo* pInfo = &info;

	int status = getaddrinfo(charAddr, std::to_string(port).c_str(), &hints, &pInfo);
	if (status != 0) throw 0;
}


void Client::TCPSocket::openSocket(addrinfo* info)
{
	sock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (sock == -1) throw 0;
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