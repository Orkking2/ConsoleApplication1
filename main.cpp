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
	uchar c(0xff);
	nstd::LongInt<> l(1);
	_STD cout << _STD bitset<sizeof(decltype(l)::storage_type) * CHAR_BIT>(static_cast<uint>(l >> 1));
	//_STD cout << nstd::LongInt<>::_Overflows(c, l);

	std::cout << '\n';
	std::system("pause");
}