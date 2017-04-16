#pragma once
#include <assert.h>
#include "Exception.h"

//
// Yihulee's Standard Template Library.
//

namespace YSTL {
	typedef int Rank;
#define DEFAULT_CAPACITY 3

	template <typename T> class Vector {
	protected:
		Rank size_;			// 用于记录大小
		int capacity_;
		T* elem_;
		void copyFrom(T const* A, Rank lo, Rank hi);
		void expand();
		void shrink();
		bool bubble(Rank lo, Rank hi);
		void bubbleSort(Rank lo, Rank hi);
		Rank max(Rank lo, Rank hi);
		void selectionSort(Rank lo, Rank hi);
		void mergeSort(Rank lo, Rank hi);
		void merge(Rank lo, Rank mi, Rank hi);
		Rank partition(Rank lo, Rank hi);
		void quickSort(Rank lo, Rank hi);
		void heapSort(Rank lo, Rank hi);
	public:
		// sort
		void selectionSort() 
		{
			selectionSort(0, size_);
		}

		void mergeSort() 
		{
			mergeSort(0, size_);
		}

		void quickSort()
		{
			quickSort(0, size_);
		}

		void heapSort()
		{
			heapSort(0, size_);
		}

		void bubbleSort()
		{
			bubbleSort(0, size_);
		}
	public:
		Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) { // 容量为c, 规模为s,所有元素初始化为v
			elem_ = new T[capacity_ = c];
			for (size_ = 0; size_ < s; elem_[size_++] = v);
		}

		Vector(T const* A, Rank n) { // 数组整体复制
			copyFrom(A, 0, n);
		}

		Vector(T const* A, Rank lo, Rank hi) {
			copyFrom(A, lo, hi);
		}

		Vector(Vector<T> const& V) {
			copyFrom(V.elem_, 0, V.size_);
		}

		Vector(Vector<T> const& V, Rank lo, Rank hi) {
			copyFrom(V.elem_, lo, hi);
		}

		// 析构函数
		~Vector() { delete[]elem_; }

		// 只读访问接口
		Rank size() { return size_; }
		bool empty() { return !size_; }
		int disordered() const;
		Rank find(T const& e) const { return find(e, 0, size_); }
		Rank find(T const& e, Rank lo, Rank hi) const;  // 无序向量区间查找
		Rank search(T const& e, Rank lo, Rank hi) const; // 有序向量区间查找

		// 可写访问接口
		T& operator[] (Rank r) const; 
		Vector<T>& operator=(Vector<T> const&);
		T erase(Rank r);
		int erase(Rank lo, Rank hi);
		Rank insert(Rank r, T const& e);
		Rank insert(T const& e) {
			return insert(size_, e); // 在尾部插入
		}
		void unsort(Rank lo, Rank hi);

		// 整体置乱
		void unsort() {
			unsort(0, size_);
		}
		int deduplicate(); // 无序去重
		int uniquify(); // 有序去重

		// 遍历
		void traverse(void (*visit)(T&));
		template <typename VST> void traverse(VST&); // 遍历,使用全局对象,全局修改性

		T& back(); // 获取最后一个元素
		const T& back() const; // const 版本的back函数

		bool empty() const;
		int capacity() const;
	};

	// back 获取最后一个元素,如果元素不存在,会抛出异常.
	template <typename T>
	T& Vector<T>::back() 
	{
		if (size_ == 0)
			throw UnderflowError("Vector back(): vector empty");
		return elem_[size_ - 1];
	}

	template <typename T>
	const T& Vector<T>::back() const 
	{
		if (size_ == 0)
			throw UnderflowError("Vector back(): vector empty");
		return elem_[size_ - 1];
	}

	template <typename T>
	bool Vector<T>::empty() const 
	{
		return size_ == 0;
	}

	// capacity 返回容量.
	template <typename T>
	int Vector<T>::capacity() const 
	{
		return capacity_;
	}

	// copyFrom 进行拷贝.T const* A 表示通过A不能对传入的T*进行修改.
	template<typename T>
	void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) 
	{
		elem_ = new T[capacity_ = 2 * (hi - lo)];
		if (elem_ == NULL)
			throw MemoryAllocationError("Vector copyFrom(): memory allocation failure");
		size_ = 0;
		while (lo < hi)
			elem_[size++] = A[lo++];
	}

	// operator= 赋值函数.
	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector<T> const& V) 
	{
		if (elem_) delete []elem_;
		copyFrom(V.elem_, 0, V.size());
		return *this;
	}

	template <typename T>
	void Vector<T>::expand() 
	{
		if (size_ < capacity_) return;
		if (capacity_ < DEFAULT_CAPACITY) capacity_ = DEFAULT_CAPACITY;
		T* oldElem = elem_; elem_ = new T[capacity_ << 1];  // 容量加倍
		for (int i = 0; i < size_; i++)
			elem_[i] = oldElem[i];
		delete[]oldElem;
	}

	template <typename T>
	void Vector<T>::shrink() 
	{
		if (capacity_ < DEFAULT_CAPACITY << 1) return;
		if (size_ << 2 > capacity_) return;  // 空间利用率在25%以下的话,要缩减空间
		T* oldElem = elem_; elem_ = new T[capacity_ >>= 1];  // 容量缩减一半
		for (int i = 0; i < size_; i++) elem_[i] = oldElem[i];
		delete[]oldElem;
	}

	template <typename T>
	T& Vector<T>::operator[] (Rank r) const 
	{
		if (r < 0 || r >= size_)
			throw IndexRangeError("Vector: index range error", r, size_);
		return elem_[r];
	}

	// permute 随机置乱向量,使个元素等概率出现在各个位置.
	template <typename T>
	void permute(Vector<T>& v) 
	{
		for (int i = V.size(); i > 0; i--)
			swap(V[i - 1], V[rand() % i]);
	}

	template <typename T>
	void Vector<T>::unsort(Rank lo, Rank hi) 
	{
		T* V = elem_ + lo;
		for (Rank i = hi - lo; i > 0; i--)
			swap(V[i - 1], V[rand() % i]);
	}

	template <typename T>
	Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const 
	{
		while ((lo < hi--) && (e != elem_[hi]));
		return hi; // 若hi < lo,则意味着失败,否则hi即命中元素的秩
	}

	template <typename T>
	Rank Vector<T>::insert(Rank r, T const& e) 
	{
		expand();  // 如果有必要,扩容
		for (int i = size_; i > r; i--) elem_[i] = elem_[i - 1];
		elem_[r] = e; size_++;
		return r;
	}

	template <typename T>
	int Vector<T>::erase(Rank lo, Rank hi) 
	{
		if (lo == hi) return 0;
		while (hi < size_) elem_[lo++] = elem_[hi++];
		size_ = lo;
		shrink(); // 如果有必要的话,缩减容量
		return hi - lo;
	}

	template <typename T>
	T Vector<T>::erase(Rank r) 
	{
		T e = elem_[r]; // 备份被删除的元素
		erase(r, r + 1);
		return e;
	}

	// deduplicate 用于去重.
	template <typename T>
	int Vector<T>::deduplicate() 
	{
		int oldSize = size_;
		Rank i = 1;
		while (i < size_)
			(find(elem_[i], 0, i) < 0) ? i++ : erase(i);
		return oldSize - size_; // 返回被删除元素总数
	}

	// traverse函数用于遍历整个vector,对每一个元素进行操作,非常类似与map函数
	template <typename T>
	void Vector<T>::traverse(void (*visit)(T&)) 
	{
		for (int i = 0; i < size_; i++)
			visit(elem_[i]);
	}

	// traverse 结合仿函数,足够强大.
	template <typename T> template<typename VST>
	void Vector<T>::traverse(VST& visit) 
	{
		for (int i = 0; i < size_; i++)
			visit(elem_[i]);
	}


	template <typename T> 
	void increase(Vector<T>& V) 
	{
		V.traverse(Increase<T>());
	}

	//
	// disordered 返回向量中逆序相邻元素对的总数.
	//
	template <typename T>
	int Vector<T>::disordered() const 
	{
		int n = 0; // 计数器
		for (int i = 1; i < size_; i++) 
			if (elem_[i - 1] > elem_[i]) n++; // 逆序计数
		return n;
	}

	//
	// uniquify 移除重复的元素,调用这个函数需要保证vector有序.
	//
	template <typename T>
	int Vector<T>::uniquify() 
	{
		Rank i = 0, j = 0;
		while (++j < size_)
			if (elem_[i] != elem_[j])
				elem_[++i] = elem_[j];
		size_ = ++i; shrink(); // 直接截除尾部的多余元素
		return j - i;
	}

	template<typename T>
	Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const 
	{
		return binSearch(elem_, e, lo, hi);
	}

	// binSearch 二分查找,在[lo, hi)内查找元素e
	template <typename T>
	static Rank binSarch(T* A, T const& e, Rank lo, Rank hi) 
	{
		while (lo < hi) 
		{
			Rank mi = (lo + hi) >> 1;
			if (e < A[mi]) hi = mi; // 深入前半段[lo, hi)继续查找
			else if (e > A[mi]) lo = mi + 1; // 深入后半段(mi, hi)继续查找
			else return mi;
		}
		return -1; // 查找失败
	}

	// bubbleSort 冒泡排序.
	template <typename T>
	void Vector<T>::bubbleSort(Rank lo, Rank hi) 
	{
		assert((0 <= lo) && (lo < hi) && (hi <= size_));
		while (!bubble(lo, hi--));
	}

	//
	// bubble 执行一趟冒泡排序.
	//
	template <typename T>
	bool Vector<T>::bubble(Rank lo, Rank hi) 
	{
		bool sorted = true;
		while (++lo < hi)
			if (elem_[lo - 1] > elem_[lo]) 
			{
				sorted = false;
				swap(elem_[lo - 1], elem_[lo]);
			}
		return sorted;
	}

	template <typename T>
	void Vector<T>::mergeSort(Rank lo, Rank hi) 
	{
		if (hi - lo < 2) return; // 单个元素自然有序
		int mi = (lo + hi) / 2;
		mergeSort(lo, mi); mergeSort(mi, hi);
		merge(lo, mi, hi);
	}

	template <typename T>
	void Vector<T>::merge(Rank lo, Rank mi, Rank hi) 
	{
		T* A = elem_ + lo; 
		int lb = mi - lo; T* B = new T[lb]; // 前子向量B[0, lb) = elem_[lo, mi)
		for (Rank i = 0; i < lb; B[i] = A[i++]);
		int lc = hi - mi; T* C = elem_ + mi; // 后子向量C[0, lc) = elem_[mi, hi)
		// 这里的归并,无非就是选择两个中较小的
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) 
		{
			if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
				A[i++] = B[j++];
			if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
				A[i++] = C[k++];
		}
		delete[]B; // 释放临时空间B
	}
}
