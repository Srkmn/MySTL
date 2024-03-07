#include <iostream>
#include <vector>
#include "Vector.h"
using namespace std;

int main() {

	/*
	int arr[10] = {0};
	for (int i = 0; i < 3; i++) {
		cin >> arr[i];
	}
	*/

	myvector<int> a = {1,2 , 3, 4, 5, 6, 7, 8,9};
	myvector<int> b(a);
	//myvector<int> b(std::move(a));

	b = std::move(a);
	
	cout << "copy constructor b£º" << endl;
	cout << b << endl;

	b.erase(b.begin()+3, b.begin()+7);
	cout << "The capacity of vector b after erase3-7£º" << endl;
	cout << b.capacity() << endl;

	b.shrink_to_fit();
	cout << "Vector b after self replacement:" << endl;
	cout << b << endl;
	
	cout << "Capacity of self replaced vector b:" << endl;
	cout << b.capacity() << endl;

	cout << "Insert 55 into position 2" << endl;
	myvector<int>::iterator it = b.find(2);
	if (it!=b.end())
	{
		b.insert(it, 55);
	}
	else cout << "Element not found" << endl;
	cout << b << endl;

	cout << "Insert 55 into the first position" << endl;
	it = b.begin();
	if (it != b.end())
	{
		b.insert(it, 55);
	}
	else cout << "Element not found" << endl;
	cout << b << endl;

	cout << "Insert 55 at the end position" << endl;
	it = b.end()-1;
	b.emplace(it, 55);
	cout << *(b.end() - 1) << endl;
	cout << b.size() << endl;
	cout << b << endl;

	b.reverse(b.begin(), b.end() - 1);
	cout << b << endl;

	cout << "front():" << endl;
	cout << b.front() << endl;

	cout << "back():" << endl;
	cout << b.back() << endl;

	cout << "at(2)" << endl;
	cout << b.at(2) << endl;

	myvector<int> c(b);
	myvector<int> d = {1, 2, 3};
	
	cout << "c == b" << endl;
	const bool ok = c == b;
	cout << ok << endl;

	cout << "c != b" << endl;
	const bool ok2 = c != b;
	cout << ok2 << endl;

	cout << "d != b" << endl;
	const bool ok3 = d != b;
	cout << ok3 << endl;
	

	return 0;
}