#include "list_exception.h"

list_exception::list_exception(const char* _message) noexcept : std::exception(_message)
{

}

list_exception::list_exception(const list_exception& _e) noexcept : std::exception(_e)
{

}

list_exception::list_exception(list_exception&& _e) noexcept : std::exception(_e)
{

}