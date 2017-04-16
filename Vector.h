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
		Rank size_;			// ���ڼ�¼��С
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
		Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) { // ����Ϊc, ��ģΪs,����Ԫ�س�ʼ��Ϊv
			elem_ = new T[capacity_ = c];
			for (size_ = 0; size_ < s; elem_[size_++] = v);
		}

		Vector(T const* A, Rank n) { // �������帴��
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

		// ��������
		~Vector() { delete[]elem_; }

		// ֻ�����ʽӿ�
		Rank size() { return size_; }
		bool empty() { return !size_; }
		int disordered() const;
		Rank find(T const& e) const { return find(e, 0, size_); }
		Rank find(T const& e, Rank lo, Rank hi) const;  // ���������������
		Rank search(T const& e, Rank lo, Rank hi) const; // ���������������

		// ��д���ʽӿ�
		T& operator[] (Rank r) const; 
		Vector<T>& operator=(Vector<T> const&);
		T erase(Rank r);
		int erase(Rank lo, Rank hi);
		Rank insert(Rank r, T const& e);
		Rank insert(T const& e) {
			return insert(size_, e); // ��β������
		}
		void unsort(Rank lo, Rank hi);

		// ��������
		void unsort() {
			unsort(0, size_);
		}
		int deduplicate(); // ����ȥ��
		int uniquify(); // ����ȥ��

		// ����
		void traverse(void (*visit)(T&));
		template <typename VST> void traverse(VST&); // ����,ʹ��ȫ�ֶ���,ȫ���޸���

		T& back(); // ��ȡ���һ��Ԫ��
		const T& back() const; // const �汾��back����

		bool empty() const;
		int capacity() const;
	};

	// back ��ȡ���һ��Ԫ��,���Ԫ�ز�����,���׳��쳣.
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

	// capacity ��������.
	template <typename T>
	int Vector<T>::capacity() const 
	{
		return capacity_;
	}

	// copyFrom ���п���.T const* A ��ʾͨ��A���ܶԴ����T*�����޸�.
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

	// operator= ��ֵ����.
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
		T* oldElem = elem_; elem_ = new T[capacity_ << 1];  // �����ӱ�
		for (int i = 0; i < size_; i++)
			elem_[i] = oldElem[i];
		delete[]oldElem;
	}

	template <typename T>
	void Vector<T>::shrink() 
	{
		if (capacity_ < DEFAULT_CAPACITY << 1) return;
		if (size_ << 2 > capacity_) return;  // �ռ���������25%���µĻ�,Ҫ�����ռ�
		T* oldElem = elem_; elem_ = new T[capacity_ >>= 1];  // ��������һ��
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

	// permute �����������,ʹ��Ԫ�صȸ��ʳ����ڸ���λ��.
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
		return hi; // ��hi < lo,����ζ��ʧ��,����hi������Ԫ�ص���
	}

	template <typename T>
	Rank Vector<T>::insert(Rank r, T const& e) 
	{
		expand();  // ����б�Ҫ,����
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
		shrink(); // ����б�Ҫ�Ļ�,��������
		return hi - lo;
	}

	template <typename T>
	T Vector<T>::erase(Rank r) 
	{
		T e = elem_[r]; // ���ݱ�ɾ����Ԫ��
		erase(r, r + 1);
		return e;
	}

	// deduplicate ����ȥ��.
	template <typename T>
	int Vector<T>::deduplicate() 
	{
		int oldSize = size_;
		Rank i = 1;
		while (i < size_)
			(find(elem_[i], 0, i) < 0) ? i++ : erase(i);
		return oldSize - size_; // ���ر�ɾ��Ԫ������
	}

	// traverse�������ڱ�������vector,��ÿһ��Ԫ�ؽ��в���,�ǳ�������map����
	template <typename T>
	void Vector<T>::traverse(void (*visit)(T&)) 
	{
		for (int i = 0; i < size_; i++)
			visit(elem_[i]);
	}

	// traverse ��Ϸº���,�㹻ǿ��.
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
	// disordered ������������������Ԫ�ضԵ�����.
	//
	template <typename T>
	int Vector<T>::disordered() const 
	{
		int n = 0; // ������
		for (int i = 1; i < size_; i++) 
			if (elem_[i - 1] > elem_[i]) n++; // �������
		return n;
	}

	//
	// uniquify �Ƴ��ظ���Ԫ��,�������������Ҫ��֤vector����.
	//
	template <typename T>
	int Vector<T>::uniquify() 
	{
		Rank i = 0, j = 0;
		while (++j < size_)
			if (elem_[i] != elem_[j])
				elem_[++i] = elem_[j];
		size_ = ++i; shrink(); // ֱ�ӽس�β���Ķ���Ԫ��
		return j - i;
	}

	template<typename T>
	Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const 
	{
		return binSearch(elem_, e, lo, hi);
	}

	// binSearch ���ֲ���,��[lo, hi)�ڲ���Ԫ��e
	template <typename T>
	static Rank binSarch(T* A, T const& e, Rank lo, Rank hi) 
	{
		while (lo < hi) 
		{
			Rank mi = (lo + hi) >> 1;
			if (e < A[mi]) hi = mi; // ����ǰ���[lo, hi)��������
			else if (e > A[mi]) lo = mi + 1; // �������(mi, hi)��������
			else return mi;
		}
		return -1; // ����ʧ��
	}

	// bubbleSort ð������.
	template <typename T>
	void Vector<T>::bubbleSort(Rank lo, Rank hi) 
	{
		assert((0 <= lo) && (lo < hi) && (hi <= size_));
		while (!bubble(lo, hi--));
	}

	//
	// bubble ִ��һ��ð������.
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
		if (hi - lo < 2) return; // ����Ԫ����Ȼ����
		int mi = (lo + hi) / 2;
		mergeSort(lo, mi); mergeSort(mi, hi);
		merge(lo, mi, hi);
	}

	template <typename T>
	void Vector<T>::merge(Rank lo, Rank mi, Rank hi) 
	{
		T* A = elem_ + lo; 
		int lb = mi - lo; T* B = new T[lb]; // ǰ������B[0, lb) = elem_[lo, mi)
		for (Rank i = 0; i < lb; B[i] = A[i++]);
		int lc = hi - mi; T* C = elem_ + mi; // ��������C[0, lc) = elem_[mi, hi)
		// ����Ĺ鲢,�޷Ǿ���ѡ�������н�С��
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) 
		{
			if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
				A[i++] = B[j++];
			if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
				A[i++] = C[k++];
		}
		delete[]B; // �ͷ���ʱ�ռ�B
	}
}
