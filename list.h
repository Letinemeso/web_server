#ifndef __LIST
#define __LIST

#include <utility>
#include "list_exception.h"

template <typename T>
class list
{
	class node
	{
	public:
		T* data;
		node* next = nullptr;
		node* prev = nullptr;

	public:
		node() = delete;

		node(const T& _data);
		node(T&& _data);

		~node();
	};

	node* first = nullptr;
	node* last = nullptr;

	unsigned int count = 0;

public:
	~list() noexcept;

	void add_to_end(T&& _data) noexcept;

	void add_to_end(T const& _data) noexcept;

	void add_to_beginning(T&& _data) noexcept;

	void add_to_beginning(T const& _data) noexcept;

	void remove(unsigned int _index);



	T& operator[](unsigned int _index) const;

	int size() const noexcept;

};

//node

template<typename T>
list<T>::node::node(const T& _data)
{
	data = new T(_data);
}

template<typename T>
list<T>::node::node(T&& _data)
{
	data = new T(std::move(_data));
}

template<typename T>
list<T>::node::~node()
{
	delete data;
}



//list

template<typename T>
list<T>::~list() noexcept
{
	node* temp;
	while (first != nullptr)
	{
		temp = first->next;
		delete first;
		first = temp;
	}
	last = nullptr;
}

template<typename T>
void list<T>::add_to_end(T&& _data) noexcept
{
	if (first == nullptr)
	{
		first = new node(std::move(_data));
		last = first;
	}
	else
	{
		last->next = new node(std::move(_data));
		last->next->prev = last;
		last = last->next;
	}

	++count;
}

template<typename T>
void list<T>::add_to_end(T const& _data) noexcept
{
	if (first == nullptr)
	{
		first = new node(_data);
		last = first;
	}
	else
	{
		last->next = new node(_data);
		last->next->prev = last;
		last = last->next;
	}

	++count;
}

template<typename T>
void list<T>::add_to_beginning(T&& _data) noexcept
{
	node* temp = first;
	first = new node(std::move(_data));
	first->next = temp;
	first->next->prev = first;

	if (last == nullptr)
	{
		last = first;
	}

	++count;
}

template<typename T>
void list<T>::add_to_beginning(T const& _data) noexcept
{
	node* temp = first;
	first = new node(_data);
	first->next = temp;
	first->next->prev = first;

	if (last == nullptr)
	{
		last = first;
	}

	++count;
}

template<typename T>
void list<T>::remove(unsigned int _index)
{
	if (count == 0)
	{
		throw list_exception("list is empty");
	}
	else if (_index >= count)
	{
		throw list_exception("out of list bounds");
	}
	else if (_index == 0)
	{
		node* temp = first->next;
		delete first;
		first = temp;
	}
	else if (_index == count - 1)
	{
		node* temp = last->prev;
		delete last;
		last = temp;
		last->next = nullptr;
	}
	else
	{
		node* temp = first;
		for (int i = 0; i < _index; ++i)
		{
			temp = temp->next;
		}

		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
	}

	--count;
}



template<typename T>
T& list<T>::operator[](unsigned int _index) const
{
	if (_index >= count)
	{
		throw list_exception("out of list bounds");
	}

	node* searched_node = first;
	for (int i = 0; i < _index; ++i)
	{
		searched_node = searched_node->next;
	}

	return *(searched_node->data);
}

template<typename T>
int list<T>::size() const noexcept
{
	return count;
}


#endif