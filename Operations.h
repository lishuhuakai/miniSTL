#pragma once

namespace YSTL 
{
	//
	// CheckOrder 用于判断一个T类对象是否局部有序
	//
	template <typename T> 
	struct CheckOrder 
	{
		T pred; int& u;
		CheckOrder(int& unsorted, T& first) :
			pred(first), u(unsorted)
		{}
		virtual void operator() (T& e) 
		{
			if (pred > e) u++;
			pred = e;
		}
	};


	//
	// Increase 使T类对象的值增1
	//
	template <typename T> 
	struct Increase 
	{
		virtual void operator() (T& e) 
		{
			e++;
		}
	};

	//
	// Decrease 使T类对象的值减1
	//
	template <typename T>
	struct Decrease 
	{
		virtual void operator() (T& e) 
		{
			e--;
		}
	};

	//
	// Double 使得T类对象的值翻倍
	//
	template <typename T>
	struct Double 
	{
		virtual void operator() (T& e) 
		{
			e *= 2;
		}
	};
}