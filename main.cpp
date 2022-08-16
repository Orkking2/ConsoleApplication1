#include <iostream>
#include <bitset>
#include <compare>
#include <vector>

//#define _NSTD_LONGINT_DEBUGGING_

#include "LongInt.h"
#include "Machine_Code_Emulator.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED


int main()
{
	nstd::emulator::run(_STD vector<uint> {901, 705, 111, 311, 600, 511, 902});
	

	std::cout << '\n';
	std::system("pause");
}