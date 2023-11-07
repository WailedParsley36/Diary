#pragma once

template<typename T>
class Node
{
public:
	T* Value;

	Node* Next;
	Node* Previous;

	~Node<T>();
};

template<typename T>
inline Node<T>::~Node<T>() {
	delete Value;
}