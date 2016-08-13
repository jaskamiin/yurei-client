#pragma once

#include <vector>

/*Socket includes*/
#ifdef _WIN32
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
#endif


class Server
{
private:

	std::vector<int> to_kill;	/*List of tasks to kill*/


	void kill_all_tasks();

public:

	Server();
	~Server();

	void bind(int);
	void start_connection();
	void kill_task(int);


};