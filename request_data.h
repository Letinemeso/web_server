#ifndef __REQUEST_DATA
#define __REQUEST_DATA

#include <string>

#define NEW_GROUP "new"
#define CONNECT_TO_GROUP "connect"
#define GET_LIST "list"
#define DISCONNECT "disconnect"

class request_data
{
private:
	std::string command, name, password;
	int group_size;

private:
	void parse_new(const std::string& _request) noexcept;
	void parse_connect(const std::string& _request) noexcept;

public:
	request_data() = delete;
	request_data(request_data&& _other) = delete;
	request_data(request_data const& _other) = delete;

	request_data(const std::string& _request) noexcept;

	const std::string& get_command() const noexcept;
	const std::string& get_name() const noexcept;
	const std::string& get_password() const noexcept;
	const int get_group_size() const noexcept;
};

#endif