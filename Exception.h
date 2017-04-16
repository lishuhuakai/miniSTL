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
	// protected���η������������msg_����.
	//
	protected:
		std::string msg_;
	};

	//
	// MemoryAllocationError �ڴ�������.
	//
	class MemoryAllocationError : public BaseException 
	{
	public:
		MemoryAllocationError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// UnderflowError, ����,����ͼ���ʿյĶ���,pop�յ�ջ��ʱ��,�ᷢ�����ִ���.
	//
	class UnderflowError : public BaseException 
	{
	public:
		UnderflowError(const string& msg = ""):
			BaseException(msg)
		{}
	};

	//
	// OverflowError, ����
	//
	class OverflowError : public BaseException 
	{
	public:
		OverflowError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// ReferenceError �����ó���.
	//
	class ReferenceError : public BaseException
	{
	public:
		ReferenceError(const string& msg = "") :
			BaseException(msg)
		{}
	};

	//
	// IndexRangeError ����Խ��
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
