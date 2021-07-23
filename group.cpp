#include "group.h"

void listen_to_client(group* _group, int _index)
{
	std::string message;

	do
	{
		message = _group->active_user[_index]->get_message();

		if (message.size() != 0)
		{
			for (int i = 0; i < _index; ++i)
			{
				if (_group->active_user[i] != nullptr)
				{
					_group->active_user[i]->send_message(message);
				}
			}
			for (int i = _index + 1; i < 5; ++i)
			{
				if (_group->active_user[i] != nullptr)
				{
					_group->active_user[i]->send_message(message);
				}
			}
		}

	} while (message.size() != 0);

	_group->disconnect_user(_index);
}



group::group(const std::string& _name, const std::string& _password) noexcept
{
	name = _name;
	password = _password;
}



void group::connect_user(SOCKET const& _client_socket, sockaddr_in const& _socket_data) noexcept
{
	for (int i = 0; i < 5; ++i)
	{
		if (active_user[i] == nullptr)
		{
			still_connected[i] = true;
			active_user[i] = new user(_client_socket, _socket_data);
			
			std::thread start_listening(&listen_to_client, this, i);
			start_listening.detach();

			return;
		}
	}

	std::cout << "user with ip adress "
		<< (int)_socket_data.sin_addr.S_un.S_un_b.s_b1 << '.'
		<< (int)_socket_data.sin_addr.S_un.S_un_b.s_b2 << '.'
		<< (int)_socket_data.sin_addr.S_un.S_un_b.s_b3 << '.'
		<< (int)_socket_data.sin_addr.S_un.S_un_b.s_b4
		<< " couldn't connect because group" << name << " is full\n\n";
}

void group::connect_user(user& _user) noexcept
{
	for (int i = 0; i < 5; ++i)
	{
		if (active_user[i] == nullptr)
		{
			still_connected[i] = true;
			active_user[i] = new user(std::move(_user));

			std::thread start_listening(&listen_to_client, this, i);
			start_listening.detach();

			return;
		}
	}

	std::cout << "user with ip adress "
		<< _user
		<< " couldn't connect because group" << name << " is full\n\n";
}

void group::disconnect_user(int _index)
{
	if (_index > 4)
	{
		throw _index;
		return;
	}

	std::cout << "user " << *active_user[_index] << " disconnected\n\n";

	still_connected[_index] = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	delete active_user[_index];
	active_user[_index] = nullptr;
}



bool group::is_empty() const noexcept
{
	for (int i = 0; i < 5; ++i)
	{
		if (active_user[i] != nullptr)
		{
			return false;
		}
	}
	return true;
}

const std::string& group::get_name() const noexcept
{
	return name;
}

const std::string& group::get_password() const noexcept
{
	return password;
}