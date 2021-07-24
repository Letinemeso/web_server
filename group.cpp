#include "group.h"



group::group(const std::string& _name, const std::string& _password) noexcept
	:name(_name), password(_password)
{

}



bool group::connect_user(user* _user) noexcept
{
	for (int i = 0; i < 5; ++i)
	{
		if (active_user[i] == nullptr)
		{
			active_user[i] = _user;

			return true;
		}
	}

	std::cout << "user with ip adress "
		<< (*_user)
		<< " couldn't connect because group" << name << " is full\n\n";

	return false;
}

user* group::disconnect_user(int _index)
{
	if (_index > 4)
	{
		throw _index;
	}

	std::cout << "user " << *active_user[_index] <<
		" has left group " << name << "\n\n";
	
	user* temp = active_user[_index];
	active_user[_index] = nullptr;
	return temp;
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

int group::get_size() const noexcept
{
	return size;
}

const user* const group::get_user(unsigned int _index) const
{
	if (_index >= size)
	{
		throw _index;
	}

	return active_user[_index];
}