#pragma once

#include <cstddef>
#include <cassert>

template<typename T>
class List
{
private:
	struct Node
	{
		Node(const T& data);

		T data;

		Node* next;
		Node* prev;
	};

public:
	template<typename E>
	class Iterator
	{
	private:
		Iterator(const List<T>& owner, Node* node);

	public:
		Iterator operator++(int);
		Iterator& operator++();
		Iterator operator--(int);
		Iterator& operator--();

		E& operator*() const;
		E* operator->() const;

		bool operator==(const Iterator& second);
		bool operator!=(const Iterator& second);

	private:
		const List<T>& mOwner;
		Node* mNode;

		friend List<T>;
	};

	using value_type = T;

	using MutableIterator = Iterator<T>;
	using ConstIterator = Iterator<const T>;

	List();
	List(const List& second);
	~List();

	List& operator=(const List& second);

	void insert(MutableIterator position, const T& value);
	void push_front(const T& value);
	void push_back(const T& value);

	void erase(MutableIterator position);
	void pop_front();
	void pop_back();

	void clear();

	MutableIterator begin();
	ConstIterator begin() const;
	ConstIterator cbegin() const;

	MutableIterator end();
	ConstIterator end() const;
	ConstIterator cend() const;

	std::size_t size() const;
	bool empty() const;

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;

private:
	void insert_first(const T& value);
	void insert_begin(const T& value);
	void insert_end(const T& value);
	void insert_before_position(const T& value, Node* position);

	void erase_last();
	void erase_begin();
	void erase_end();
	void erase_at_position(Node* node);

	Node* mBegin;
	Node* mEnd;

	std::size_t mSize;
};

template<typename T>
List<T>::Node::Node(const T& data) :
	data(data),
	next(nullptr),
	prev(nullptr)
{ }

template<typename T>
template<typename E>
List<T>::Iterator<E>::Iterator(const List<T>& owner, List<T>::Node* node) :
	mOwner(owner),
	mNode(node)
{ }

template<typename T>
template<typename E>
typename List<T>::Iterator<E> List<T>::Iterator<E>::operator++(int)
{
	Iterator copy = *this;
	operator++();
	return copy;
}

template<typename T>
template<typename E>
typename List<T>::Iterator<E>& List<T>::Iterator<E>::operator++()
{
	mNode =
		mNode
		? mNode->next
		: mOwner.mBegin;
	return *this;
}

template<typename T>
template<typename E>
typename List<T>::Iterator<E> List<T>::Iterator<E>::operator--(int)
{
	Iterator copy = *this;
	operator--();
	return copy;
}

template<typename T>
template<typename E>
typename List<T>::Iterator<E>& List<T>::Iterator<E>::operator--()
{
	mNode =
		mNode
		? mNode->prev
		: mOwner.mEnd;
	return *this;
}

template<typename T>
template<typename E>
E& List<T>::Iterator<E>::operator*() const
{
	return mNode->data;
}

template<typename T>
template<typename E>
E* List<T>::Iterator<E>::operator->() const
{
	return &(mNode->data);
}

template<typename T>
template<typename E>
bool List<T>::Iterator<E>::operator==(const List<T>::Iterator<E>& second)
{
	return mNode == second.mNode;
}

template<typename T>
template<typename E>
bool List<T>::Iterator<E>::operator!=(const List<T>::Iterator<E>& second)
{
	return mNode != second.mNode;
}

template<typename T>
List<T>::List() :
	mBegin(nullptr),
	mEnd(nullptr),
	mSize(0)
{ }

template<typename T>
List<T>::List(const List<T>& second) :
	List()
{
	for (const auto& element : second)
		push_back(element);
}

template<typename T>
List<T>::~List()
{
	clear();
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& second)
{
	clear();

	for (const auto& element : second)
		push_back(element);

	return *this;
}

template<typename T>
void List<T>::insert(List<T>::MutableIterator position, const T& value)
{
	assert(&position.mOwner == this);

	if (mSize == 0)
		insert_first(value);
	else if (position.mNode == mBegin)
		insert_begin(value);
	else if (position.mNode == nullptr)
		insert_end(value);
	else
		insert_before_position(value, position.mNode);
}

template<typename T>
void List<T>::push_front(const T& value)
{
	if (mSize == 0)
		insert_first(value);
	else
		insert_begin(value);
}

template<typename T>
void List<T>::push_back(const T& value)
{
	if (mSize == 0)
		insert_first(value);
	else
		insert_end(value);
}

template<typename T>
void List<T>::erase(List<T>::MutableIterator position)
{
	assert(&position.mOwner == this);

	if (mSize == 1)
		erase_last();
	else if (position.mNode == mBegin)
		erase_begin();
	else if (position.mNode == mEnd)
		erase_end();
	else
		erase_at_position(position.mNode);
}

template<typename T>
void List<T>::pop_front()
{
	if (mSize == 1)
		erase_last();
	else
		erase_begin();
}

template<typename T>
void List<T>::pop_back()
{
	if (mSize == 1)
		erase_last();
	else
		erase_end();
}

template<typename T>
void List<T>::clear()
{
	while (!empty())
		pop_front();
}

template<typename T>
typename List<T>::MutableIterator List<T>::begin()
{
	return List<T>::MutableIterator(*this, mBegin);
}

template<typename T>
typename List<T>::ConstIterator List<T>::begin() const
{
	return typename List<T>::ConstIterator(*this, mBegin);
}

template<typename T>
typename List<T>::ConstIterator List<T>::cbegin() const
{
	return typename List<T>::ConstIterator(*this, mBegin);
}

template<typename T>
typename List<T>::MutableIterator List<T>::end()
{
	return List<T>::MutableIterator(*this, nullptr);
}

template<typename T>
typename List<T>::ConstIterator List<T>::end() const
{
	return typename List<T>::ConstIterator(*this, nullptr);
}

template<typename T>
typename List<T>::ConstIterator List<T>::cend() const
{
	return typename List<T>::ConstIterator(*this, nullptr);
}

template<typename T>
std::size_t List<T>::size() const
{
	return mSize;
}

template<typename T>
bool List<T>::empty() const
{
	return mSize == 0;
}

template<typename T>
T& List<T>::front()
{
	assert(mSize > 0);

	return mBegin->data;
}

template<typename T>
const T& List<T>::front() const
{
	assert(mSize > 0);

	return mBegin->data;
}

template<typename T>
T& List<T>::back()
{
	assert(mSize > 0);

	return mEnd->data;
}

template<typename T>
const T& List<T>::back() const
{
	assert(mSize > 0);

	return mEnd->data;
}

template<typename T>
void List<T>::insert_first(const T& value)
{
	assert(mSize == 0);

	auto node = new List<T>::Node(value);

	mBegin = mEnd = node;

	mSize++;
}

template<typename T>
void List<T>::insert_begin(const T& value)
{
	assert(mSize > 0);
	assert(mBegin != nullptr);

	auto node = new List<T>::Node(value);

	node->next = mBegin;
	mBegin->prev = node;
	mBegin = node;

	mSize++;
}

template<typename T>
void List<T>::insert_end(const T& value)
{
	assert(mSize > 0);
	assert(mEnd != nullptr);

	auto node = new List<T>::Node(value);

	node->prev = mEnd;
	mEnd->next = node;
	mEnd = node;

	mSize++;
}

template<typename T>
void List<T>::insert_before_position(const T& value, List<T>::Node* position)
{
	assert(mSize >= 2);
	assert(position != nullptr);
	assert(position->prev != nullptr);

	auto node = new List<T>::Node(value);

	node->next = position;
	node->prev = position->prev;
	node->next->prev = node;
	node->prev->next = node;

	mSize++;
}

template<typename T>
void List<T>::erase_last()
{
	assert(mSize == 1);

	delete mBegin;

	mBegin = mEnd = nullptr;

	mSize--;
}

template<typename T>
void List<T>::erase_begin()
{
	assert(mSize > 1);
	assert(mBegin != nullptr);

	auto node = mBegin;

	mBegin = mBegin->next;
	mBegin->prev = nullptr;

	delete node;

	mSize--;
}

template<typename T>
void List<T>::erase_end()
{
	assert(mSize > 0);
	assert(mEnd != nullptr);

	auto node = mEnd;

	mEnd = mEnd->prev;
	mEnd->next = nullptr;

	delete node;

	mSize--;
}

template<typename T>
void List<T>::erase_at_position(List<T>::Node* node)
{
	assert(mSize > 2);
	assert(position != nullptr);
	assert(position != mBegin);
	assert(position != mEnd);

	node->next->prev = node->prev;
	node->pred->next = node->next;

	delete node;

	mSize--;
}