#include <iostream>
#include <bitset>
#include <compare>

#define _NSTD_LONGINT_DEBUGGING_

#include "LongInt.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED

int main()
{
	nstd::LongInt<uint> li(256);
	//_STD cout << (li > 1);
	_STD cout << static_cast<uint>(--li);


	std::cout << '\n';
	std::system("pause");
}