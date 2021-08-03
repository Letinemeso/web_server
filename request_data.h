#ifndef __REQUEST_DATA
#define __REQUEST_DATA

#include <string>

#define NOT_A_COMMAND ""
#define NEW_GROUP "new"
#define CONNECT_TO_GROUP "connect"
#define GET_LIST "list"
#define DISCONNECT "disconnect"
#define MESSAGE "message"

class request_data
{
private:
	std::string command; 
	std::string name, password;
	std::string message;
	int group_size = 0;

private:
	void parse_new(const std::string& _request) noexcept;
	void parse_connect(const std::string& _request) noexcept;
	void parse_message(const std::string& _request) noexcept;

public:
	request_data() = delete;
	request_data(request_data&& _other) = delete;
	request_data(request_data const& _other) = delete;

	request_data(const std::string& _request) noexcept;

	const std::string& get_command() const noexcept;
	const std::string& get_name() const noexcept;
	const std::string& get_password() const noexcept;
	const int get_group_size() const noexcept;
	const std::string& get_message() const noexcept;
};

#endif