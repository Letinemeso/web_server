#ifndef __USER_MANAGER
#define __USER_MANAGER

#include "group.h"
#include "user.h"
#include "list.h"
#include "server_socket.h"
#include "request_data.h"
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

class user_manager
{
private:
	list<group> groups;
	list<user> non_auth_users;

	server_socket& socket_obj;

public:
	user_manager(server_socket& _socket);

};

#endif