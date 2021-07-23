#ifndef __GROUP
#define __GROUP

#include "request_data.h"
#include "user.h"
#include <WS2tcpip.h>
#include <memory>
#include <thread>
#include <string>

class group
{
private:
	std::string name;
	std::string password;

	user* active_user[5] = { nullptr };
	bool still_connected[5] = { false };

public:
	group(const std::string& _name, const std::string& _password) noexcept;
	group() = delete;
	
	void connect_user(SOCKET const& _client_socket, sockaddr_in const& _socket_data) noexcept;
	void connect_user(user& _user) noexcept;
	void disconnect_user(int _index);
	friend void listen_to_client(group* _group, int _index);

	bool is_empty() const noexcept;
	const std::string& get_name() const noexcept;
	const std::string& get_password() const noexcept;
};

#endif