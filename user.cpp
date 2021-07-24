#include "user.h"

user::user(SOCKET const& _client_socket, sockaddr_in const& _socket_data) noexcept
{
	client_socket = _client_socket;

	ip_addr[0] = _socket_data.sin_addr.S_un.S_un_b.s_b1;
	ip_addr[1] = _socket_data.sin_addr.S_un.S_un_b.s_b2;
	ip_addr[2] = _socket_data.sin_addr.S_un.S_un_b.s_b3;
	ip_addr[3] = _socket_data.sin_addr.S_un.S_un_b.s_b4;
}

user::user(user&& user) noexcept
{
	client_socket = user.client_socket;
	user.client_socket = INVALID_SOCKET;

	ip_addr[0] = user.ip_addr[0];
	ip_addr[1] = user.ip_addr[1];
	ip_addr[2] = user.ip_addr[2];
	ip_addr[3] = user.ip_addr[3];

	user.ip_addr[0] = 0;
	user.ip_addr[1]	= 0;
	user.ip_addr[2]	= 0;
	user.ip_addr[3]	= 0;
}

user::~user() noexcept
{
	if (client_socket != INVALID_SOCKET)
	{
		closesocket(client_socket);
	}
}



bool user::operator==(const user& _user) const noexcept
{
	for (int i = 0; i < 4; ++i)
	{
		if (ip_addr[i] != _user.ip_addr[i])
		{
			return false;
		}
	}
	return true;
}

bool user::operator!=(const user& _user) const noexcept
{
	for (int i = 0; i < 4; ++i)
	{
		if (ip_addr[i] != _user.ip_addr[i])
		{
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& _stream, user const& _user)
{
	for (int i = 0; i < 3; ++i)
	{
		_stream << (int)_user.ip_addr[0] << '.';
	}
	_stream << (int)_user.ip_addr[3];

	return _stream;
}

bool user::is_invalid() const noexcept
{
	return (client_socket == INVALID_SOCKET ? true : false);
}



std::string user::get_message() const noexcept
{
	int size;
	char buffer[MAX_MESSAGE_LENGTH + 1];

	size = recv(client_socket, buffer, MAX_MESSAGE_LENGTH, 0);

	if (size == -1)
	{
		return std::string();
	}
	else
	{
		buffer[size] = 0;

		return std::string(buffer);
	}
}

void user::send_message(const std::string& _message) const noexcept
{
	int send_result = send(client_socket, _message.c_str(), _message.size(), 0);

	if (send_result == SOCKET_ERROR)
	{
		std::cout << "error occured at ip adress " << *this << "\n\n";
	}
}

void user::send_message(const char* _message) const noexcept
{
	int send_result = send(client_socket, _message, strlen(_message), 0);

	if (send_result == SOCKET_ERROR)
	{
		std::cout << "error occured at ip adress " << *this << "\n\n";
	}
}

//void user::connect() noexcept
//{
//	connected = true;
//}
//
//void user::disconnect() noexcept
//{
//	connected = false;
//}
//
//bool user::is_connected() const noexcept
//{
//	return connected;
//}