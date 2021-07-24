#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "server_socket.h"
#include "user_manager.h"


int main()
{
	WSADATA wdata;

	int error = WSAStartup(MAKEWORD(2, 2), &wdata);
	if (error != 0)
	{
		std::cout << "setup error: " << error << "\n\n";
		WSACleanup();
		return 1;
	}

	server_socket* ss;
	try
	{
		ss = new server_socket;
	}
	catch (socket_exception& e)
	{
		std::cout << e.what() << "\n\n";
		return -1;
	}

	user_manager um(*ss);

	std::string input;

	while (true)
	{
		std::cin >> input;

		if (input == "/stop")
		{
			break;
		}
	}	

	WSACleanup();

	return 0;
}