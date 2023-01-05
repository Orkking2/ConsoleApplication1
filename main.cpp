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

template <typename array_type>
	requires _NSTD _Good_array<array_type, int, _STD string>
void func(array_type t) {}

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
	
	using _M = _NSTD rebind_t<_NSTD remove_template_t<_STD vector<int>>, char>;

	//_NSTD BinTree<int, _STD string> b(vec);
	
	//timemachineman<nothing, _STD vector<int>>();
	//static_assert(!_STD is_error_code_enum_v<decltype(_STD declval<_STD vector<int>>().operator[](_STD declval<nothing>()))>);

	std::cout << '\n';
	std::system("pause");
}