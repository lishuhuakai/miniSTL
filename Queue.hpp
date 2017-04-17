#pragma once
#include "List.hpp"

namespace YSTL
{
	//
	// Queue 一个简易的队列实现.
	//
	template <typename T>
	class Queue
	{
	public:
		Queue() {}
		void push(const T& item);
		void pop();
		T& front();
		const T& front() const;
		
		int size() const
		{
			return qlist_.size();
		}

		bool empty() const
		{
			return qlist_.empty();
		}
	private:
		List<T> qlist_;
	};

	template <typename T>
	void Queue<T>::push(const T& item)
	{
		qlist_.pushBack(item);
	}

	template <typename T>
	void Queue<T>::pop()
	{
		if (qlist_.size() == 0)
			throw UnderflowError("Queue pop(): empty queue");
		qlist_.popFront();
	}

	template <typename T>
	T& Queue<T>::front()
	{
		if (qlist_.size() == 0)
			throw UnderflowError("Queue front(): empty queue");
		return qlist_.front();
	}

	template <typename T>
	const T& Queue<T>::front() const
	{
		if (qlist_.size() == 0)
			throw UnderflowError("Queue front(): empty queue");
		return qlist_.front();
	}

}
