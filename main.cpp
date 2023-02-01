#include <iostream>
#include <bitset>
#include <compare>
#include <vector>
#include <thread>
#include "Defines.h"

#define _NSTD_LONGINT_TESTER_


//#include "Threads.h"
//#include "LongInt.h"
//#include "Multilock.h"
//#include "BinTree.h"
#include "Pair.h"
//#include "ContiguousContainer.h"
//#include "Deque.h"
//#include "Timer.h"
#include "Huff.hpp"

_NSTD_UNSIGNED


int main()
{
	_STD vector<int> ints;
	_NSTD_FOR_I(7)
		ints.push_back(_I);

	_STD vector<_NSTD pair<int, uint>> pints;
	_NSTD_FOR_I(ints.size())
		pints.push_back({ ints[_I], (uint)ints[_I] });
	

	_NSTD HuffTree<int> ht(pints);
	
	auto vec = ht.encode(ints);

	auto vec3 = ht.encode(1);

	_STD cout << _STD boolalpha;

	_NSTD_FOR_I(vec3.size())
		_STD cout << vec3[_I] << " ";
	
	//_STD cout << ht.decode(vec3.begin());

	//auto vec2 = ht.decode(vec);


	std::cout << '\n';
	std::system("pause");
}