#pragma once
#include "Exception.h"

namespace YSTL 
{
	typedef int Rank;
#define ListNodePosi(T) ListNode<T>*	// 列表节点位置
	
	//
	// ListNode 这个类用于记录实际的值,它不应该暴露给外部.
	//
	template <typename T> 
	struct ListNode 
	{
		// 成员
		T data;
		ListNodePosi(T) pred;  // 前
		ListNodePosi(T) succ;  // 后继

		// 构造函数
		ListNode() {}
		ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
			: data(e), pred(p), succ(s)
		{}

		// 操作接口
		ListNodePosi(T) insertAsPred(T const& e);	// 紧随当前节点之前插入新结点
		ListNodePosi(T) insertAsSucc(T const& e);	// 紧随当前节点之后插入新结点
	};

	// insertAsPred 前插入
	template <typename T>
	ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) 
	{
		ListNodePosi(T) x = new ListNode(e, pred, this);	// 创建新节点
		pred->succ = x;
		pred = x;
		return x;      // 返回新节点的位置
	}

	// insertAsSucc 后插入
	template <typename T>
	ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) 
	{
		ListNodePosi(T) x = new ListNode(e, this, succ);
		succ->pred = x;
		succ = x;
		return x;
	}

	//
	// List 一个双向链表.
	//
	template <typename T> class List
	{
		class iterator;
		class const_iterator;
	private:
		int size_;					// 用于记录链表的大小
		ListNodePosi(T) header_;	// 头哨兵
		ListNodePosi(T) trailer_;	// 尾哨兵
	
	protected:
		void init();	// 列表创建时的初始化
		int clear();	// 清除所有的节点
		void copyNodes(ListNodePosi(T) p, int n); // 复制列表中自位置p起的n项
		void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); // 归并
		void mergeSort(ListNodePosi(T) &p, int n);		// 对从p开始的n个节点进行归并排序
		void selectionSort(ListNodePosi(T) p, int n);	// 对从p开始的n个节点进行选择排序
		void insertionSort(ListNodePosi(T) p, int n);	// 对从p开始的n个节点进行插入排序
		void derase(ListNodePosi(T) p);

		void merge(List<T>& L) 
		{
			merge(first(), size_, L, L.first(), L.size_); // 全列表归并
		}
	public:
		// sort
		void mergeSort() 
		{
			mergeSort(header_->succ, size_);
		}

		void insertionSort()
		{
			insertionSort(header_->succ, size_);
		}

		void selectionSort()
		{
			selectionSort(header_->succ, size_);
		}

	protected:

		ListNodePosi(T) first() const 
		{ 
			return header_->succ; 
		}
		
		ListNodePosi(T) last() const 
		{ 
			return trailer_->pred; 
		}

		// valid 判断位置p是否对外合法
		bool valid(ListNodePosi(T) p) 
		{ 
			return p && (trailer_ != p) && (header_ != p);
		}

	public:
		// 构造函数,析构函数
		List() 
		{ 
			init(); 
		}

		List(List<T> const& L);			 // 整体复制链表L
		List(const iterator& it, int n); // 复制列表中自位置p起的n项
		~List();						 // 释放(包括头尾哨兵在内的)所有节点
	
	public:
		// 只读访问接口
		Rank size() const 
		{ 
			return size_; 
		}

		bool empty() const 
		{ 
			return size_ <= 0; 
		}

		T& operator[](Rank r) const;
	
	public:
		int disordered() const;		// 判断列表是否已经排序
		
		// find 无序列表查找, 在整个列表区间寻找e
		iterator find(T const& e) const 
		{ 
			return iterator(find(e, size_, trailer_));
		}

		
		// search 有序列表查找
		iterator search(T const& e) const
		{  
			return iterator(search(e, size_, trailer_));
		}
	
	protected:
		ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) it) const;
		ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; // 无序区间查找
	
	public:

		iterator selectMax(const iterator& it, int n); // 在*it以及n-1个后继中选出最大者

		// selectMax 找出整体的最大者
		iterator selectMax() 
		{ 
			return selectMax(iterator(header_->succ), size_);
		}

	public:
		// 可写访问接口
		iterator inseartAsFirst(T const& e);  // 将e当做首节点插入
		iterator inseartAsLast(T const& e);   // 将e当做末节点插入
		
		iterator insertAsSucc(const iterator& it, T const& e); // 将e当做p的后继插入
		iterator insertAsPred(const iterator& it, T const& e); // 将e当做p的前驱插入

		T erase(const iterator& p);

		void popFront();
		void popBack();
		const T& front() const;
		const T& back() const;
		T& front();
		T& back();


		int deduplicate(); // 无序去重
		int uniquify(); // 有序去重
		//void reverse(); // 前后倒置

	public:
		// 遍历
		void traverse(void(*visit)(T&));
		template<typename VST>
		void traverse(VST&);

	public:
		class iterator
		{
		public:
			friend class const_iterator;
			friend class List<T>;
			// 构造函数
			iterator() {}

			bool operator==(const iterator& rhs) const
			{
				return nodePtr_ == rhs.nodePtr_;
			}

			// inequality for iterators
			bool operator!= (const iterator& rhs) const
			{
				return nodePtr_ != rhs.nodePtr_;
			}

			// pointer dereference operator
			T& operator* ()
			{
				if (nodePtr_->succ == NULL)
					throw ReferenceError("List iterator: reference error");
				return nodePtr_->data;
			}

			// prefix increment.
			iterator& operator++ ()
			{
				nodePtr_ = nodePtr_->succ;
				return *this;
			}

			// postfix increment.
			iterator operator++(int)
			{
				iterator tmp = *this;
				nodePtr_ = nodePtr_->succ;
				return tmp;
			}

			// postfix decrement.
			iterator operator-- (int)
			{
				iterator tmp = *this;
				nodePtr_ = nodePtr_->pred;
				return tmp;
			}

			// prefix decrement.
			iterator& operator-- ()
			{
				nodePtr_ = nodePtr_->pred;
				return *this;
			}
		private:
			ListNodePosi(T) nodePtr_;
			iterator(ListNodePosi(T) p) : nodePtr_(p)
			{}
		};

		class const_iterator
		{
		public:
			friend class List<T>;
			const_iterator() {}

			const_iterator(const iterator& obj)
			{
				nodePtr_ = obj.nodePtr_;
			}

			bool operator==(const const_iterator& rhs) const
			{
				return nodePtr_ == rhs.nodePtr_;
			}

			bool operator!=(const const_iterator& rhs) const
			{
				return nodePtr_ != rhs.nodePtr_;
			}

			const T& operator* () const
			{
				if (nodePtr_->succ == NULL)
					throw ReferenceError("List iterator: reference error");
				return nodePtr_->data;
			}

			// postfix increment.
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				nodePtr_ = nodePtr_->succ;
				return tmp;
			}

			// prefix increment.
			const_iterator& operator++ ()
			{
				nodePtr_ = nodePtr_->succ;
				return *this;
			}

			// prefix decrement
			const_iterator& operator-- ()
			{
				nodePtr_ = nodePtr_->pred;
				return *this;
			}

			// postfix decrement.
			const_iterator operator-- (int)
			{
				const_iterator tmp = *this;
				nodePtr_ = nodePtr_->pred;
				return tmp;
			}

		private:
			ListNodePosi(T) nodePtr_;
			const_iterator(ListNodePosi(T) p) :
				nodePtr_(p)
			{}
		};

		iterator begin()
		{
			return iterator(header_->succ);
		}

		const_iterator begin() const
		{
			return const_iterator(header_->succ);
		}

		iterator end()
		{
			return iterator(trailer_);
		}

		const_iterator end() const
		{
			return const_iterator(trailer_);
		}
	};


	// init 初始化操作.
	template<typename T>
	void List<T>::init() 
	{
		header_ = new ListNode<T>;
		trailer_ = new ListNode<T>;
		header_->succ = trailer_; 
		header_->pred = NULL;
		trailer_->pred = header_; 
		trailer_->succ = NULL;
		size_ = 0;
	}

	//
	// 重载[]操作符,不推荐使用这个函数.
	//
	template<typename T>
	T& List<T>::operator[](Rank r) const 
	{
		if (r >= size_)
			throw IndexRangeError("List: Index Range Error", r, size_);
		ListNodePosi(T) p = header_->succ;  // 列表的起点
		while (0 < r--) 
			p = p->succ;
		return p->data; // 目标节点,返回其中所存的元素
	}

	// find
	// 仅供内部使用
	template <typename T>
	ListNodePosi(T) List<T>::find(T const&e, int n, ListNodePosi(T) p) const 
	{
		// 不断往前找
		while (0 < n--)
			if (e == (p = p->pred)->data)
				return p;
		// 没有找到的话,返回end()
		return NULL;
	}

	// 将e当做首节点插入
	template <typename T>
	typename List<T>::iterator List<T>::inseartAsFirst(T const& e) 
	{
		size_++;
		return iterator(header_->insertAsSucc(e));
	}

	// 将e当做末节点插入
	template <typename T>
	typename List<T>::iterator List<T>::inseartAsLast(T const& e) 
	{
		size_++;
		return iterator(trailer_->insertAsPred(e));
	}

	template <typename T>
	void List<T>::popFront()
	{
		if (size_ == 0)
			throw UnderflowError("List popFront Error: List is empty");
		derase(header_->succ);
	}

	template <typename T>
	void List<T>::popBack()
	{
		if (size_ == 0)
			throw UnderflowError("List popBack Error: List is empty");
		derase(trailer_->pred);
	}

	// insertSucc 将e当做p的后继插入
	template <typename T>
	typename List<T>::iterator List<T>::insertAsSucc(const iterator& it, T const& e) 
	{
		size_++;
		return iterator(it.nodePtr_->insertAsSucc(e));
	}

	// insertPred 将e当做p的前驱插入
	template <typename T>
	typename List<T>::iterator List<T>::insertAsPred(const iterator& it, T const& e) 
	{
		size_++;
		return iterator((it.nodePtr_)->insertAsPred(e));
	}

	// copyNodes是列表内部方法,复制列表中自位置p起的n项
	template <typename T>
	void List<T>::copyNodes(ListNodePosi(T) p, int n) 
	{
		init();
		// 当p不足n项时,能拷多少算多少
		// 纠正上面的错误,因为这个接口不是提供给外部的,所以不可能出现不足n项的情况
		while (n--) 
		{
			inseartAsLast(p->data);
			p = p->succ;
		}
	}

	template <typename T>
	List<T>::List(const iterator& it, int n) 
	{ 
		ListNodePosi p = *it;
		copyNodes(p, n); 
	}

	template <typename T>
	List<T>::List(List<T> const& L) 
	{
		copyNodes(L.first(), L.size_);
	}

	template <typename T>
	List<T>::~List() 
	{
		clear();
		delete header_;
		delete trailer_;
	}

	// derase函数,仅供内部使用
	template <typename T>
	void List<T>::derase(ListNodePosi(T) p)
	{
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		delete p; size_--;
	}

	// erase 删除合法节点e,返回其数值
	template <typename T>
	T List<T>::erase(const iterator& it) 
	{
		ListNodePosi(T) p = it.nodePtr_;
		T e = p->data;
		derase(p);
		return e;
	}

	// clear 清空列表
	template <typename T>
	int List<T>::clear() 
	{
		int oldsize = size_;
		while (0 < size_)
			erase(header_->succ);
		return oldsize;
	}

	// deduplicate 剔除无序列表中的重复节点
	template <typename T>
	int List<T>::deduplicate() 
	{
		if (size_ < 2) return 0;
		int oldsize = size_;
		ListNodePosi(T) p = header_; 
		Rank r = 0;
		while (trailer_ != (p = p->succ)) 
		{
			ListNodePosi(T) q = find(p->data, r, p);
			q ? derase(q) : r++;
		}
		return oldsize - size_;
	}

	// traverse 遍历
	template <typename T>
	void List<T>::traverse(void(*visit)(T&)) 
	{
		for (ListNodePosi(T) p = header_->succ; p != trailer_; p = p->succ)
			visit(p->data);
	}

	// traverse 借助仿函数机制来进行遍历,事实上,这种方式更加强大.
	template <typename T>
	template <typename VST>
	void List<T>::traverse(VST& visit)
	{
		for (ListNodePosi(T) p = header_->succ; p != trailer_; p = p->succ)
			visit(p->data);
	}

	// uniquify 有序列表去重
	template <typename T>
	int List<T>::uniquify() 
	{
		if (size_ < 2) 
			return 0;
		int oldSize = size_;
		ListNodePosi(T) p = first(); 
		ListNodePosi(T) q;  // p为各区段起点,q为其后继
		while (trailer_ != (q = p->succ))
			if (p->data != q->data) p = q;
			else derase(q);
		return oldSize - size_;
	}

	// search 有序列表查找
	// 在有序列表内节点p(可能是trailer)的n个真前驱中,找到不大于e的最后着
	template <typename T>
	ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const 
	{
		// 不断往前寻找
		while (0 <= n--)
			if (((p = p->pred)->data) <= e) break;
		return p; // 即使也没有找到,也返回停留的位置
	}

	// insertionSort 插入排序
	// 仅供内部使用的函数
	template <typename T>
	void List<T>::insertionSort(ListNodePosi(T) p, int n) 
	{
		for (int r = 0; r < n; r++) 
		{
			// search函数从后向前查找合适的插入位置
			// insertA在位置前面插入
			insertAsSucc(iterator(search(p->data, r, p)), p->data); // 找到合适的位置并插入
			p = p->succ; derase(p->pred);
		}
	}

	// selectionSort 选择排序
	// 仅供内部使用的函数
	template <typename T>
	void List<T>::selectionSort(ListNodePosi(T) p, int n) 
	{
		ListNodePosi(T) head = p->pred; 
		ListNodePosi(T) tail = p;
		for (int i = 0; i < n; i++) 
			tail = tail->succ;
		
		while (1 < n) 
		{
			ListNodePosi(T) max = selectMax(head->succ, n).nodePtr_;
			insertAsPred(iterator(tail), erase(iterator(max))); // 将其移至无序区间末尾
			tail = tail->pred; 
			n--;
		}
	}

	// selectMax
	template <typename T>
	typename List<T>::iterator List<T>::selectMax(const iterator& it, int n) 
	{
		ListNodePosi(T) max = it.nodePtr_; // 最大者暂时定为p
		for (ListNodePosi(T) cur = max; 1 < n; n--)
			if ((cur = cur->succ)->data > max->data)
				max = cur;
		return iterator(max);  // 返回最大节点的位置
	}

	// merge 归并排序
	// 有序列表的归并:当前列表中自p起的n个元素,与列表L中自q起的m个元素归并
	// 内部使用的函数
	template <typename T>
	void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) 
	{
		ListNodePosi(T) prev = p->pred;
		// 如果直接用p,而不是用sp,会出错,我怀疑是编译器的问题.
		// 这是我第一次遇到如此奇葩的bug.
		ListNode<T>* sp = p;
		while (0 < m) 
		{
			if ((0 < n) && (sp->data <= q->data))  // 如果p中元素较小 
			{
				sp = sp->succ; // 指针p后移
				if (q == sp)  
					break; 
				n--;
			}
			else // q中元素较小,需要将q中元素插入到p之前
			{
				q = q->succ;
				T val = L.erase(iterator(q->pred));
				insertAsPred(sp, val);
				m--;
			}
		}
		p = prev->succ;  // 确定归并后区间的新起点
	}

	// mergeSort 归并排序
	// 内部使用的函数
	template <typename T>
	void List<T>::mergeSort(ListNodePosi(T) &p, int n) 
	{
		if (n < 2) return;  // 若待排序范围已经足够小,则直接返回
		int m = n >> 1;
		ListNodePosi(T) q = p; 
		for (int i = 0; i < m; i++) 
			q = q->succ; // 均分列表, 找到中间节点
		mergeSort(p, m); 
		mergeSort(q, n - m);
		merge(p, m, *this, q, n - m);  // 开始归并
	}

	template <typename T>
	T& List<T>::front()
	{
		if (size_ == 0)
			throw UnderflowError("List front(): list is empty");
		return header_->succ.data;
	}

	template <typename T>
	const T& List<T>::front() const
	{
		if (size_ == 0)
			throw UnderflowError("List front(): list is empty");
		return header_->succ.data;
	}

	template <typename T>
	T& List<T>::back()
	{
		if (size_ == 0)
			throw UnderflowError("List back(): list is empty");
		return trailer_->pred.data;
	}

	template <typename T>
	const T& List<T>::back() const
	{
		if (size_ == 0)
			throw UnderflowError("List back(): list is empty");
		return trailer_->pred.data;
	}
}
