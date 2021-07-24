#ifndef __USER
#define __USER

#include <WS2tcpip.h>
#include <memory>
#include <iostream>
#include <ostream>
#include <string>


#ifndef MAX_MESSAGE_LENGTH 
#define MAX_MESSAGE_LENGTH 8192
#endif


class user
{
private:
	SOCKET client_socket;
	char ip_addr[4];

public:
	user(SOCKET const& _client_socket, sockaddr_in const& _socket_data) noexcept;
	user(user&& _user) noexcept;
	~user() noexcept;

	user(user const& _user) = delete;

	bool operator==(const user& _user) const noexcept;
	bool operator!=(const user& _user) const noexcept;
	friend std::ostream& operator<<(std::ostream& _stream, user const& _user);

	bool is_invalid() const noexcept;
	std::string get_message() const noexcept;
	void send_message(const std::string& _message) const noexcept;
	void send_message(const char* _message) const noexcept;
};

#endif