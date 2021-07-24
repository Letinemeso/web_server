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

	int size = 5;
	user* active_user[5] = { nullptr };

public:
	group(const std::string& _name, const std::string& _password) noexcept;
	group() = delete;
	
	bool connect_user(user* _user) noexcept;
	user* disconnect_user(int _index);

	bool is_empty() const noexcept;
	const std::string& get_name() const noexcept;
	const std::string& get_password() const noexcept;
	int get_size() const noexcept;
	const user* const get_user(unsigned int _index) const;
};

#endif