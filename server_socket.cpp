#include "server_socket.h"

//socket exception class methods

socket_exception::socket_exception(const char* _error)
	: error(_error)
{ }

socket_exception::~socket_exception()
{
	delete[] error;
}

const char* socket_exception::what()
{
	return error;
}



//server socket class methods

server_socket::server_socket()
{
	int error;

	addrinfo* result = nullptr;
	addrinfo hints;

	hints.ai_addrlen = 0;
	hints.ai_canonname = nullptr;
	hints.ai_addr = nullptr;
	hints.ai_next = nullptr;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	error = getaddrinfo(0, "27015", &hints, &result);
	if (error != 0)
	{
		throw socket_exception("getaddrinfo() error");
	}

	socket_obj = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (socket_obj == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		throw socket_exception("error at socket()");
	}

	error = bind(socket_obj, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	if (error != 0)
	{
		closesocket(socket_obj);
		throw socket_exception("bind() error");
	}

	error = listen(socket_obj, SOMAXCONN);

	if (error == SOCKET_ERROR)
	{
		closesocket(socket_obj);
		throw socket_exception("listen() error");
	}
}

server_socket::~server_socket()
{
	closesocket(socket_obj);
}

SOCKET const& server_socket::get() const noexcept
{
	return socket_obj;
}

user server_socket::accept_connection() const noexcept
{
	sockaddr_in adress;
	int size = sizeof(adress);

	SOCKET client_socket = accept(socket_obj, (sockaddr*)&adress, &size);

	return user(client_socket, adress);
}