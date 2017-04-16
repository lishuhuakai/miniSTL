#pragma once

namespace YSTL 
{
	//
	// CheckOrder �����ж�һ��T������Ƿ�ֲ�����
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
	// Increase ʹT������ֵ��1
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
	// Decrease ʹT������ֵ��1
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
	// Double ʹ��T������ֵ����
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