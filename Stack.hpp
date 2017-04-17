#pragma once
#include "Vector.hpp"
#include "Exception.h"

namespace YSTL
{
	//
	// Stack 一个简易的栈实现.
	//
	template <typename T>
	class Stack
	{
	public:
		Stack() {}
		void push(const T& item);
		void pop();
		T& top();
		const T& top() const;
		bool empty() const
		{
			return stackVector_.empty();
		}
		int size() const
		{
			return stackVector_.size();
		}
	private:
		Vector<T> stackVector_;
	};

	template <typename T>
	void Stack::push(const T& item)
	{
		stackVector_.pushBack(item);
	}

	template <typename T>
	void Stack::pop()
	{
		if (empty())
			throw UnderflowError("Stack pop(): stack empty");
		stackVector_.popBack();
	}

	template <typename T>
	T& Stack<T>::top()
	{
		if (empty())
			throw UnderflowError("Stack top(): stack empty");
		return stackVector_.back();
	}

	template <typename T>
	const T& Stack<T>::top() const
	{
		if (empty())
			throw UnderflowError("Stack top(): Stack empty");
		return stackVector_.back();
	}

}
