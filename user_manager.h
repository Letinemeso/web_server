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

	server_socket& socket_obj;

public:
	user_manager(server_socket& _socket);

	void remove_empty() noexcept;

	unsigned int get_groups_count() const noexcept;
	group* get_group(const std::string& _name) const noexcept;
	group* create_group(const std::string& _name, const std::string& _password) noexcept;

	std::string get_group_list() const noexcept;
	const server_socket& get_socket() const noexcept;
};

#endif