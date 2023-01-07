#include <iostream>
#include <bitset>
#include <compare>
#include <vector>

#define _NSTD_LONGINT_TESTER_

#include "LongInt.h"
#include "BinTree.h"
#include "Pair.h"
//#include "Machine_Code_Emulator.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED

template <typename T>
	requires _NSTD _Good_array<T> && _NSTD _Good_pair<typename T::value_type, int, _STD string>
void func() {}


int main()
{
	/*
	_NSTD _LONGINT_TESTER lit;
	lit.test1();
	lit.test2();
	lit.test3();
    */

	_STD vector<_NSTD pair<int, _STD string>> vec = {
		{1, "Hello"},
		{2, "World"},
		{3, "My"},
		{4, "Name"},
		{5, "Is"},
		{6, "Nicolas"}
	};

	//func<decltype(vec)>();
	
	_NSTD BinTree<int, _STD string> b(vec);

	//_STD cout << b.GetVal(1);

	
	std::cout << '\n';
	std::system("pause");
}