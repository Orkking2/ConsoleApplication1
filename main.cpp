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



int main()
{
	//_NSTD _LONGINT_TESTER lit;
	//lit.test1();
	//lit.test2();
	//lit.test3();
    
	_NSTD LongInt<> li(2435298);
	//li / 34;
	//_STD cout << static_cast<uint>(li / 34) << ' ' << static_cast<uint>(li) / 34 << '\n';
	_STD cout << li / 34 << '\n' << _STD bitset<3 * CHAR_BIT>(2435298 / 34);

	std::cout << '\n';
	std::system("pause");
}