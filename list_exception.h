#ifndef __LIST_EXCEPTION
#define __LIST_EXCEPTION

#include <exception>

class list_exception : std::exception
{
public:
	list_exception() noexcept = delete;

	list_exception(const list_exception& _e) noexcept;
	list_exception(list_exception&& _e) noexcept;

	list_exception(const char* _message) noexcept;
};

#endif