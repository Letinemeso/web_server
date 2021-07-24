#include "user_manager.h"

//mutexes for preventing data corruption in lists

std::mutex group_mutex;

//functions for threads

void listen_to_user(user_manager* _um, user* _user)
{
	std::string message;
	group* group_with_current_user = nullptr;

	do
	{
		message = _user->get_message();
		request_data request(message);

		if (group_with_current_user == nullptr)	//that means user
		{										//doesnt belong to a group
			if (request.get_command() == NEW_GROUP)
			{
				group_with_current_user = _um->create_group(request.get_name(), request.get_password());
				if (group_with_current_user != nullptr)
				{
					_user->send_message("//success");
					group_with_current_user->connect_user(_user);
				}
				else
				{
					_user->send_message("//fail");
				}
			}
			else if (request.get_command() == CONNECT_TO_GROUP)
			{
				group_with_current_user = _um->get_group(request.get_name());
				if (group_with_current_user != nullptr)
				{
					if (group_with_current_user->get_password() == request.get_password())
					{
						_user->send_message("//success");
						group_with_current_user->connect_user(_user);
					}
					else
					{
						_user->send_message("//fail");
						group_with_current_user = nullptr;
					}
				}
				else
				{
					_user->send_message("//fail");
				}
			}
			else if (request.get_command() == GET_LIST)
			{
				_user->send_message(_um->get_group_list());
			}
		}
		else
		{
			if (request.get_command() == NOT_A_COMMAND) 
			{
				for (int i = 0; i < group_with_current_user->get_size(); ++i)
				{
					if (group_with_current_user->get_user(i) != nullptr)
					{
						if (*(group_with_current_user->get_user(i)) != *_user)
						{
							group_with_current_user->get_user(i)->send_message(message);
						}
					}
				}
			}
			else if (request.get_command() == DISCONNECT)
			{
				for (int i = 0; i < group_with_current_user->get_size(); ++i)
				{
					if (*(group_with_current_user->get_user(i)) == *_user)
					{
						group_with_current_user->disconnect_user(i);
						group_with_current_user = nullptr;
						break;
					}
				}
				_user->send_message("//disconnected");
			}
		}
		
	} while (message.size() != 0);

	std::cout << "user " << *_user << " disconnected\n\n";

	if (group_with_current_user != nullptr)
	{
		for (int i = 0; i < group_with_current_user->get_size(); ++i)
		{
			if (*(group_with_current_user->get_user(i)) == *_user)
			{
				group_with_current_user->disconnect_user(i);
				group_with_current_user = nullptr;
				break;
			}
		}
	}

	delete _user;
}

void remove_empty_ft(user_manager* _um)
{
	while (true)
	{
		group_mutex.lock();
		_um->remove_empty();
		group_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void listen_for_connections(user_manager* _um)
{
	while (true)
	{
		user* connected_user = new user(_um->get_socket().accept_connection());

		if (connected_user->is_invalid())
		{
			delete connected_user;
		}
		else
		{
			std::cout << "user " << *connected_user << " connected to the server\n\n";

			std::thread listening_thread(listen_to_user, _um, connected_user);
			listening_thread.detach();
		}
	}
}



user_manager::user_manager(server_socket& _socket)
	:socket_obj(_socket)
{
	std::thread remove_empty_thread(remove_empty_ft, this);
	remove_empty_thread.detach();

	std::thread listen_for_connections_thread(listen_for_connections, this);
	listen_for_connections_thread.detach();
}



void user_manager::remove_empty() noexcept
{
	for (int i = 0; i < groups.size(); ++i)
	{
		if (groups[i].is_empty())
		{
			groups.remove(i);
		}
	}
}



group* user_manager::get_group(const std::string& _name) const noexcept
{
	group* return_value = nullptr;

	group_mutex.lock();
	for (int i = 0; i < groups.size(); ++i)
	{
		if (groups[i].get_name() == _name)
		{
			return_value = &(groups[i]);
			break;
		}
	}
	group_mutex.unlock();

	return return_value;
}

group* user_manager::create_group(const std::string& _name, const std::string& _password) noexcept
{
	if (get_group(_name) != nullptr)
	{
		return nullptr;
	}

	group_mutex.lock();
	groups.add_to_end(group(_name, _password));
	group* return_value = &(groups[groups.size() - 1]);
	group_mutex.unlock();
	
	return return_value;
}

std::string user_manager::get_group_list() const noexcept
{
	std::string return_value("//list");

	group_mutex.lock();
	for (int i = 0; i < groups.size(); ++i)
	{
		return_value += '&';
		return_value += (char)(groups[i].get_name().size());
		return_value += groups[i].get_name();
	}
	group_mutex.unlock();

	return return_value;
}

const server_socket& user_manager::get_socket() const noexcept
{
	return socket_obj;
}