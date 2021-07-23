#ifndef __SOCKET_SETUP
#define __SOCKET_SETUP

#include <WS2tcpip.h>
#include <iostream>
#include "user.h"

class socket_exception
{
private:
	const char* error;

public:
	socket_exception(const char* _error);

	~socket_exception();

	const char* what();
};



class server_socket
{
	SOCKET socket_obj = INVALID_SOCKET;

public:
	server_socket();

	~server_socket();

	SOCKET const& get() const noexcept;

	user accept_connection() const noexcept;
};

#endif