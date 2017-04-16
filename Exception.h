#pragma once
#include <sstream>
#include <string>
using namespace std;

namespace YSTL 
{

	class BaseException 
	{
	public:
		BaseException (const std::string& str = "") :
			msg_(str)
		{}

		std::string what() const 
		{
			return msg_;
		}
	//
	// protected修饰符允许子类访问msg_属性.
	//
	protected:
		std::string msg_;
	};

	//
	// MemoryAllocationError 内存分配错误.
	//
	class MemoryAllocationError : public BaseException 
	{
	public:
		MemoryAllocationError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// UnderflowError, 下溢,当试图访问空的队列,pop空的栈等时候,会发生这种错误.
	//
	class UnderflowError : public BaseException 
	{
	public:
		UnderflowError(const string& msg = ""):
			BaseException(msg)
		{}
	};

	//
	// OverflowError, 上溢
	//
	class OverflowError : public BaseException 
	{
	public:
		OverflowError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// ReferenceError 解引用出错.
	//
	class ReferenceError : public BaseException
	{
	public:
		ReferenceError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// IndexRangeError 索引越界
	//
	class IndexRangeError : public BaseException 
	{
	public:
		IndexRangeError(const string& msg, int i, int size) :
			BaseException()
		{
			ostringstream indexErr;
			indexErr << msg << " index" << i << " size = " << size << ends;
			msg_ = indexErr.str();
		}
	};

}
