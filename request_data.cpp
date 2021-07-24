#include "request_data.h"

#define CREATE_NEW_GROUP "new"
#define CONNECT_TO_GROUP "connect"

void request_data::parse_new(const std::string& _request) noexcept	//fills "name" and "password" fields
{
	//example: /new&{10}letinemeso&{8}password		
	//{number} - symbol encoded with number equal to next word length

	int offset = 1;
	int size;

	while (_request[offset] != '&')
	{
		++offset;
	}
	++offset;

	size = _request[offset];
	++offset;

	name.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		name += _request[offset + i];
	}
	
	offset += size + 1;
	size = _request[offset];
	++offset;

	password.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		password += _request[offset + i];
	}
}

void request_data::parse_connect(const std::string& _request) noexcept	//fills "name" and "password" fields
{
	//example: /connect&{10}letinemeso&{8}password

	int offset = 1;
	int size;

	while (_request[offset] != '&')
	{
		++offset;
	}
	++offset;

	size = _request[offset];
	++offset;

	name.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		name += _request[offset + i];
	}

	offset += size + 1;
	size = _request[offset];
	++offset;

	password.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		password += _request[offset + i];
	}

}



request_data::request_data(const std::string& _request) noexcept
{
	if (_request[0] != '/')
	{
		command = NOT_A_COMMAND;
		return;
	}

	int offset = 1, size = 0;
	
	while (offset + size < _request.size())
	{
		if (_request[offset + size] != '&')
		{
			++size;
		}
		else
		{
			break;
		}
	}

	command.reserve(size);

	for (int i = offset; i < offset + size; ++i)
	{
		command += _request[i];
	}

	if (command == "new")
	{
		parse_new(_request);
	}
	else if (command == "connect")
	{
		parse_connect(_request);
	}
	else if (command == "list")
	{
		//nothing more to do here
	}
	else if (command == "disconnect")
	{
		//nothing more to do here
	}

}



const std::string& request_data::get_command() const noexcept
{
	return command;
}

const std::string& request_data::get_name() const noexcept
{
	return name;
}

const std::string& request_data::get_password() const noexcept
{
	return password;
}

const int request_data::get_group_size() const noexcept
{
	return group_size;
}