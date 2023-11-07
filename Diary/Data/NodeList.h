#pragma once
#include "Node.h"

template<typename T>
class NodeList
{
public:
	Node<T>* Head;
	Node<T>* Tail;

	Node<T>* Current;

	Node<T>* Next();
	Node<T>* Previous();

	Node<T>* Pop();
	Node<T>* Shift();

	Node<T>* ElementAt(int index);

	int Length();

	void Insert(Node<T>* value, int index);
	void Append(Node<T>* value);

	void Remove(Node<T>* value, bool free = true);
	void RemoveAt(int index);
};

template<typename T>
inline Node<T>* NodeList<T>::Next()
{
	if (Current == nullptr)
		Current = Head;
	else
		Current = Current->Next;

	return Current;
}
template<typename T>
inline Node<T>* NodeList<T>::Previous()
{
	if (Current == nullptr)
		Current = Head;
	else
		Current = Current->Previous;

	return Current;
}

template<typename T>
inline void NodeList<T>::Insert(Node<T>* value, int index)
{
	if (Head = nullptr)
	{
		Head = value;
		Tail = value;
		Current = value;
		return;
	}

	Node<T>* iteratedNode = Head;
	for (int i = 0; i < index; i++)
		iteratedNode = iteratedNode->Next;

	if (iteratedNode == Head) {
		Head->Previous = value;
		value->Next = Head;

		Head = value;
	}
	else if (iteratedNode == Tail) {
		Tail->Next = value;
		value->Previous = Tail;

		Tail = value;
	}
	else
	{
		Node<T>* insertBefore = iteratedNode->Next;
		insertBefore->Previous = value;
		value->Next = insertBefore;

		iteratedNode->Next = value;
		value->Previous = iteratedNode;
	}
}
template<typename T>
inline void NodeList<T>::Append(Node<T>* value)
{
	if (Head == nullptr)
	{
		Head = value;
		Tail = value;
		value->Next = nullptr;
		value->Previous = nullptr;
		Current = Head;
		return;
	}

	Tail->Next = value;
	value->Previous = Tail;

	Tail = value;
	value->Next = nullptr;
}

template<typename T>
inline Node<T>* NodeList<T>::Pop()
{
	Node<T>* value = Tail;

	Tail = Tail->Previous;
	return value;
}
template<typename T>
inline Node<T>* NodeList<T>::Shift()
{
	Node<T>* value = Head;

	Head = Head->Next;
	return value;
}

template<typename T>
inline Node<T>* NodeList<T>::ElementAt(int index)
{
	if (Head == nullptr)
		return nullptr;

	// TODO: Optimalize going through -> we can go from the end
	//int difference = index - Length();

	Node<T>* record = Head;
	for (int i = 0; i < index; i++)
		record = record->Next;

	return record;
}

template<typename T>
inline int NodeList<T>::Length()
{
	if (Head == nullptr)
		return 0;

	int length = 1;
	Node<T>* iteratedNode = Head;
	while (iteratedNode->Next != nullptr)
	{
		iteratedNode = iteratedNode->Next;
		length++;
	}

	return length;
}

template<typename T>
inline void NodeList<T>::Remove(Node<T>* value, bool free)
{
	if (Head == nullptr)
		return;

	Node<T>* iteratedNode = Head;
	while (iteratedNode != nullptr)
	{
		if (value == iteratedNode)
		{
			Node<T>* insertBefore = iteratedNode->Next;
			insertBefore->Previous = iteratedNode->Previous;

			iteratedNode->Previous->Next = insertBefore;

			if (free)
				delete iteratedNode;
		}
	}
}
template<typename T>
inline void NodeList<T>::RemoveAt(int index)
{
	if (Head == nullptr)
		return;

	Node<T>* iteratedNode = Head;
	for (int i = 0; i < index; i++)
		iteratedNode = iteratedNode->Next;

	if (Head == Tail)
	{
		Head = nullptr;
		Tail = nullptr;
		Current = nullptr;
	}
	else if (iteratedNode->Next != nullptr)
	{
		Node<T>* insertBefore = iteratedNode->Next;
		insertBefore->Previous = iteratedNode->Previous;

		if (iteratedNode->Previous != nullptr)
			iteratedNode->Previous->Next = insertBefore;

		if (insertBefore->Next == nullptr)
			Tail = insertBefore;
		
		if (insertBefore->Previous == nullptr)
			Head = insertBefore;
	}
	else if (iteratedNode->Previous != nullptr)
	{
		iteratedNode->Previous->Next = nullptr;
		Tail = iteratedNode->Previous;
	}



	delete iteratedNode;
}
