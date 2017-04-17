#include "Vector.hpp"
#include "Operations.h"
#include "List.hpp"
#include <iostream>
using namespace YSTL;
using namespace std;

template <typename T>
void PRINT_LST(const List<T>& lst) 
{
	for (auto it = lst.begin(); it != lst.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

template <typename T>
void PRINT_VEC(const Vector<T>& vec)
{
	int size = vec.size();
	for (int i = 0; i < size; i++)
		cout << vec[i] << " ";
	cout << endl;
}

void VECTOR_TEST()
{
	Vector<int> vec;
	vec.pushBack(20);
	vec.pushBack(20);
	vec.pushBack(90);
	vec.pushBack(75);
	PRINT_VEC(vec);
	vec.selectionSort();
	PRINT_VEC(vec);
}


void LIST_TEST() 
{
	List<int> lst;
	lst.pushBack(1);
	lst.pushBack(7);
	lst.pushBack(3);
	lst.pushBack(4);
	lst.pushBack(5);
	lst.pushBack(5);
	//PRINT_LST(lst);
	//lst.selectionSort();
	//lst.mergeSort();
	//lst.insertionSort();
	lst.deduplicate();
	lst.mergeSort();
	lst.uniquify();
	PRINT_LST(lst);
	cout << *lst.selectMax() << endl;
}

int main() {
	VECTOR_TEST();
	//LIST_TEST();
	getchar();
}