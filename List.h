#pragma once
#include "Exception.h"

namespace YSTL 
{
	typedef int Rank;
#define ListNodePosi(T) ListNode<T>*	// �б�ڵ�λ��
	
	//
	// ListNode ��������ڼ�¼ʵ�ʵ�ֵ,����Ӧ�ñ�¶���ⲿ.
	//
	template <typename T> 
	struct ListNode 
	{
		// ��Ա
		T data;
		ListNodePosi(T) pred;  // ǰ
		ListNodePosi(T) succ;  // ���

		// ���캯��
		ListNode() {}
		ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
			: data(e), pred(p), succ(s)
		{}

		// �����ӿ�
		ListNodePosi(T) insertAsPred(T const& e);	// ���浱ǰ�ڵ�֮ǰ�����½��
		ListNodePosi(T) insertAsSucc(T const& e);	// ���浱ǰ�ڵ�֮������½��
	};

	// insertAsPred ǰ����
	template <typename T>
	ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) 
	{
		ListNodePosi(T) x = new ListNode(e, pred, this);	// �����½ڵ�
		pred->succ = x;
		pred = x;
		return x;      // �����½ڵ��λ��
	}

	// insertAsSucc �����
	template <typename T>
	ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) 
	{
		ListNodePosi(T) x = new ListNode(e, this, succ);
		succ->pred = x;
		succ = x;
		return x;
	}

	//
	// List һ��˫������.
	//
	template <typename T> class List
	{
		class iterator;
		class const_iterator;
	private:
		int size_;					// ���ڼ�¼����Ĵ�С
		ListNodePosi(T) header_;	// ͷ�ڱ�
		ListNodePosi(T) trailer_;	// β�ڱ�
	
	protected:
		void init();	// �б���ʱ�ĳ�ʼ��
		int clear();	// ������еĽڵ�
		void copyNodes(ListNodePosi(T) p, int n); // �����б�����λ��p���n��
		void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); // �鲢
		void mergeSort(ListNodePosi(T) &p, int n);		// �Դ�p��ʼ��n���ڵ���й鲢����
		void selectionSort(ListNodePosi(T) p, int n);	// �Դ�p��ʼ��n���ڵ����ѡ������
		void insertionSort(ListNodePosi(T) p, int n);	// �Դ�p��ʼ��n���ڵ���в�������
		void derase(ListNodePosi(T) p);

		void merge(List<T>& L) 
		{
			merge(first(), size_, L, L.first(), L.size_); // ȫ�б�鲢
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

		// valid �ж�λ��p�Ƿ����Ϸ�
		bool valid(ListNodePosi(T) p) 
		{ 
			return p && (trailer_ != p) && (header_ != p);
		}

	public:
		// ���캯��,��������
		List() 
		{ 
			init(); 
		}

		List(List<T> const& L);			 // ���帴������L
		List(const iterator& it, int n); // �����б�����λ��p���n��
		~List();						 // �ͷ�(����ͷβ�ڱ����ڵ�)���нڵ�
	
	public:
		// ֻ�����ʽӿ�
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
		int disordered() const;		// �ж��б��Ƿ��Ѿ�����
		
		// find �����б����, �������б�����Ѱ��e
		iterator find(T const& e) const 
		{ 
			return iterator(find(e, size_, trailer_));
		}

		
		// search �����б����
		iterator search(T const& e) const
		{  
			return iterator(search(e, size_, trailer_));
		}
	
	protected:
		ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) it) const;
		ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; // �����������
	
	public:

		iterator selectMax(const iterator& it, int n); // ��*it�Լ�n-1�������ѡ�������

		// selectMax �ҳ�����������
		iterator selectMax() 
		{ 
			return selectMax(iterator(header_->succ), size_);
		}

	public:
		// ��д���ʽӿ�
		iterator inseartAsFirst(T const& e);  // ��e�����׽ڵ����
		iterator inseartAsLast(T const& e);   // ��e����ĩ�ڵ����
		
		iterator insertAsSucc(const iterator& it, T const& e); // ��e����p�ĺ�̲���
		iterator insertAsPred(const iterator& it, T const& e); // ��e����p��ǰ������

		T erase(const iterator& p);

		void popFront();
		void popBack();
		const T& front() const;
		const T& back() const;
		T& front();
		T& back();


		int deduplicate(); // ����ȥ��
		int uniquify(); // ����ȥ��
		//void reverse(); // ǰ����

	public:
		// ����
		void traverse(void(*visit)(T&));
		template<typename VST>
		void traverse(VST&);

	public:
		class iterator
		{
		public:
			friend class const_iterator;
			friend class List<T>;
			// ���캯��
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


	// init ��ʼ������.
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
	// ����[]������,���Ƽ�ʹ���������.
	//
	template<typename T>
	T& List<T>::operator[](Rank r) const 
	{
		if (r >= size_)
			throw IndexRangeError("List: Index Range Error", r, size_);
		ListNodePosi(T) p = header_->succ;  // �б�����
		while (0 < r--) 
			p = p->succ;
		return p->data; // Ŀ��ڵ�,�������������Ԫ��
	}

	// find
	// �����ڲ�ʹ��
	template <typename T>
	ListNodePosi(T) List<T>::find(T const&e, int n, ListNodePosi(T) p) const 
	{
		// ������ǰ��
		while (0 < n--)
			if (e == (p = p->pred)->data)
				return p;
		// û���ҵ��Ļ�,����end()
		return NULL;
	}

	// ��e�����׽ڵ����
	template <typename T>
	typename List<T>::iterator List<T>::inseartAsFirst(T const& e) 
	{
		size_++;
		return iterator(header_->insertAsSucc(e));
	}

	// ��e����ĩ�ڵ����
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

	// insertSucc ��e����p�ĺ�̲���
	template <typename T>
	typename List<T>::iterator List<T>::insertAsSucc(const iterator& it, T const& e) 
	{
		size_++;
		return iterator(it.nodePtr_->insertAsSucc(e));
	}

	// insertPred ��e����p��ǰ������
	template <typename T>
	typename List<T>::iterator List<T>::insertAsPred(const iterator& it, T const& e) 
	{
		size_++;
		return iterator((it.nodePtr_)->insertAsPred(e));
	}

	// copyNodes���б��ڲ�����,�����б�����λ��p���n��
	template <typename T>
	void List<T>::copyNodes(ListNodePosi(T) p, int n) 
	{
		init();
		// ��p����n��ʱ,�ܿ����������
		// ��������Ĵ���,��Ϊ����ӿڲ����ṩ���ⲿ��,���Բ����ܳ��ֲ���n������
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

	// derase����,�����ڲ�ʹ��
	template <typename T>
	void List<T>::derase(ListNodePosi(T) p)
	{
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		delete p; size_--;
	}

	// erase ɾ���Ϸ��ڵ�e,��������ֵ
	template <typename T>
	T List<T>::erase(const iterator& it) 
	{
		ListNodePosi(T) p = it.nodePtr_;
		T e = p->data;
		derase(p);
		return e;
	}

	// clear ����б�
	template <typename T>
	int List<T>::clear() 
	{
		int oldsize = size_;
		while (0 < size_)
			erase(header_->succ);
		return oldsize;
	}

	// deduplicate �޳������б��е��ظ��ڵ�
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

	// traverse ����
	template <typename T>
	void List<T>::traverse(void(*visit)(T&)) 
	{
		for (ListNodePosi(T) p = header_->succ; p != trailer_; p = p->succ)
			visit(p->data);
	}

	// traverse �����º������������б���,��ʵ��,���ַ�ʽ����ǿ��.
	template <typename T>
	template <typename VST>
	void List<T>::traverse(VST& visit)
	{
		for (ListNodePosi(T) p = header_->succ; p != trailer_; p = p->succ)
			visit(p->data);
	}

	// uniquify �����б�ȥ��
	template <typename T>
	int List<T>::uniquify() 
	{
		if (size_ < 2) 
			return 0;
		int oldSize = size_;
		ListNodePosi(T) p = first(); 
		ListNodePosi(T) q;  // pΪ���������,qΪ����
		while (trailer_ != (q = p->succ))
			if (p->data != q->data) p = q;
			else derase(q);
		return oldSize - size_;
	}

	// search �����б����
	// �������б��ڽڵ�p(������trailer)��n����ǰ����,�ҵ�������e�������
	template <typename T>
	ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const 
	{
		// ������ǰѰ��
		while (0 <= n--)
			if (((p = p->pred)->data) <= e) break;
		return p; // ��ʹҲû���ҵ�,Ҳ����ͣ����λ��
	}

	// insertionSort ��������
	// �����ڲ�ʹ�õĺ���
	template <typename T>
	void List<T>::insertionSort(ListNodePosi(T) p, int n) 
	{
		for (int r = 0; r < n; r++) 
		{
			// search�����Ӻ���ǰ���Һ��ʵĲ���λ��
			// insertA��λ��ǰ�����
			insertAsSucc(iterator(search(p->data, r, p)), p->data); // �ҵ����ʵ�λ�ò�����
			p = p->succ; derase(p->pred);
		}
	}

	// selectionSort ѡ������
	// �����ڲ�ʹ�õĺ���
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
			insertAsPred(iterator(tail), erase(iterator(max))); // ����������������ĩβ
			tail = tail->pred; 
			n--;
		}
	}

	// selectMax
	template <typename T>
	typename List<T>::iterator List<T>::selectMax(const iterator& it, int n) 
	{
		ListNodePosi(T) max = it.nodePtr_; // �������ʱ��Ϊp
		for (ListNodePosi(T) cur = max; 1 < n; n--)
			if ((cur = cur->succ)->data > max->data)
				max = cur;
		return iterator(max);  // �������ڵ��λ��
	}

	// merge �鲢����
	// �����б�Ĺ鲢:��ǰ�б�����p���n��Ԫ��,���б�L����q���m��Ԫ�ع鲢
	// �ڲ�ʹ�õĺ���
	template <typename T>
	void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) 
	{
		ListNodePosi(T) prev = p->pred;
		// ���ֱ����p,��������sp,�����,�һ����Ǳ�����������.
		// �����ҵ�һ��������������bug.
		ListNode<T>* sp = p;
		while (0 < m) 
		{
			if ((0 < n) && (sp->data <= q->data))  // ���p��Ԫ�ؽ�С 
			{
				sp = sp->succ; // ָ��p����
				if (q == sp)  
					break; 
				n--;
			}
			else // q��Ԫ�ؽ�С,��Ҫ��q��Ԫ�ز��뵽p֮ǰ
			{
				q = q->succ;
				T val = L.erase(iterator(q->pred));
				insertAsPred(sp, val);
				m--;
			}
		}
		p = prev->succ;  // ȷ���鲢������������
	}

	// mergeSort �鲢����
	// �ڲ�ʹ�õĺ���
	template <typename T>
	void List<T>::mergeSort(ListNodePosi(T) &p, int n) 
	{
		if (n < 2) return;  // ��������Χ�Ѿ��㹻С,��ֱ�ӷ���
		int m = n >> 1;
		ListNodePosi(T) q = p; 
		for (int i = 0; i < m; i++) 
			q = q->succ; // �����б�, �ҵ��м�ڵ�
		mergeSort(p, m); 
		mergeSort(q, n - m);
		merge(p, m, *this, q, n - m);  // ��ʼ�鲢
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
