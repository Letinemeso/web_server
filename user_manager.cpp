#include "user_manager.h"

//mutexes for preventing data corruption in lists

std::mutex group_mutex;
std::mutex user_mutex;



group* get_group(list<group>* _groups, const request_data& _data)
{
	group_mutex.lock();
	for (int i = 0; i < _groups->size(); ++i)
	{
		if ((*_groups)[i].get_name() == _data.get_name())
		{
			group_mutex.unlock();
			return &((*_groups)[i]);
		}
	}
	group_mutex.unlock();

	return nullptr;
}

group* create_group(list<group>* _groups, const request_data& _data)
{
	group* check = get_group(_groups, _data);

	if (check == nullptr)
	{
		group_mutex.lock();
		_groups->add_to_end(group(_data.get_name(), _data.get_password()));
		group_mutex.unlock();
		return &((*_groups)[_groups->size() - 1]);
	}
	
	return nullptr;
}

std::string create_group_list_message(const list<group> const* _groups)
{
	std::string message("//list");
	for (int i = 0; i < _groups->size(); ++i)
	{
		message += '&';
		message += (char)(*_groups)[i].get_name().size();
		message += (*_groups)[i].get_name();
	}

	return message;
}



//functions for threads

void listen_to_non_auth_user(list<group>* _groups, list<user>* _users, user* _user)
{
	std::string message;

	do
	{
		message = _user->get_message();

		if (message[0] == '/')
		{
			request_data data(message);

			if (data.get_command() == NEW_GROUP)
			{
				group* gr = create_group(_groups, message);

				if (gr == nullptr)
				{
					std::string temp_message("//fail");
					_user->send_message(temp_message);
				}
				else
				{
					std::cout << "user " << *_user << " joined group " << gr->get_name() << "\n\n";
					std::string temp_message("//success");
					_user->send_message(temp_message);
					gr->connect_user(*_user);
					break;
				}				
			}
			else if (data.get_command() == CONNECT_TO_GROUP)
			{
				group* gr = get_group(_groups, message);

				if (gr == nullptr)
				{
					std::string temp_message("//fail");
					_user->send_message(temp_message);
				}
				else
				{
					if (gr->get_password() == data.get_password())
					{
						std::cout << "user " << *_user << " joined group " << gr->get_name() << "\n\n";
						std::string temp_message("//success");
						_user->send_message(temp_message);
						gr->connect_user(*_user);
						break;
					}
					else
					{
						std::string temp_message("//fail");
						_user->send_message(temp_message);
					}
				}
			}
			else if (data.get_command() == GET_LIST)
			{
				group_mutex.lock();
				std::string temp_message = create_group_list_message(_groups);
				_user->send_message(temp_message);
				group_mutex.unlock();
			}
			else if (data.get_command() == DISCONNECT)
			{
				std::string temp_message("//disconnected");
				_user->send_message(temp_message);

				std::cout << "not authorized user " << *_user << " disconnected\n\n";

				user_mutex.lock();
				for (int i = 0; i < _users->size(); ++i)
				{
					if ((*_users)[i] == (*_user))
					{
						_users->remove(i);
					}
				}
				user_mutex.unlock();

				break;
			}
		}
	} while (message.size() != 0);

	//deleting user if they disconnected 
	if (message.size() == 0)
	{
		std::cout << "error occured at " << *_user << "\n\n";

		user_mutex.lock();
		for (int i = 0; i < _users->size(); ++i)
		{
			if ((*_users)[i] == (*_user))
			{
				_users->remove(i);
			}
		}
		user_mutex.unlock();
	}
}

void check_for_emptiness(list<group>* _groups, list<user>* _users)
{
	while (true)
	{
		group_mutex.lock();
		for (int i = 0; i < _groups->size(); ++i)
		{
			if ((*_groups)[i].is_empty() == true)
			{
				_groups->remove(i);
				--i;
			}
		}
		group_mutex.unlock();
		
		user_mutex.lock();
		for (int i = 0; i < _users->size(); ++i)
		{
			if ((*_users)[i].is_invalid() == true)
			{
				_users->remove(i);
				--i;
			}
		}
		user_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void listen_for_connections(server_socket* _socket, list<group>* _groups, list<user>* _users)
{
	while (true)
	{
		user connected_user = _socket->accept_connection();

		if (connected_user.is_invalid() == false)
		{
			std::cout << "user " << connected_user << " connected to the server\n\n";

			user_mutex.lock();
			_users->add_to_end(connected_user);
			std::thread listen_to_non_auth_user_thread(listen_to_non_auth_user, _groups, _users, &((*_users)[_users->size() - 1]));
			user_mutex.unlock();
			listen_to_non_auth_user_thread.detach();
		}
	}
}



user_manager::user_manager(server_socket& _socket)
	:socket_obj(_socket)
{
	std::thread check_for_emptiness_thread(check_for_emptiness, &groups, &non_auth_users);
	check_for_emptiness_thread.detach();

	std::thread listen_for_connections_thread(listen_for_connections, &socket_obj, &groups, &non_auth_users);
	listen_for_connections_thread.detach();
}