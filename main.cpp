#include "Vector.h"
#include "Operations.h"
#include "List.h"
#include <iostream>
using namespace YSTL;
using namespace std;

void VECTOR_TEST()
{
	Vector<int> vec;
	vec.insert(10);
	vec.insert(20);
	vec.traverse(Double<int>());
	std::cout << vec[0] << " " << vec[1] << endl;
}


template <typename T>
void PRINT_LST(const List<T>& lst) {
	for (auto it = lst.begin(); it != lst.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

void LIST_TEST() 
{
	List<int> lst;
	lst.inseartAsLast(1);
	lst.inseartAsLast(7);
	lst.inseartAsLast(3);
	lst.inseartAsLast(4);
	lst.inseartAsLast(5);
	lst.inseartAsLast(5);
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
	LIST_TEST();
	getchar();
}