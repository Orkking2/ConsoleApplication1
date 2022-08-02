#include <iostream>
#include <bitset>
#include <compare>

//#define _NSTD_LONGINT_DEBUGGING_

#include "LongInt.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED

int main()
{
	nstd::LongInt li(rand());
	nstd::LongInt li2(li);
	uint ui(rand());

	_STD cout << li << '\n' << nstd::LongInt(ui) << "\n\n";

	_STD cout << "Li + Ui: " << static_cast<uint>(li + ui) << " v " << (ui + li) << " (" << static_cast<uint>(li + ui) - (ui + li) << ")\n";
	_STD cout << "Li + Ui - Ui: " << static_cast<uint>(li + ui - ui) << " v " << (ui + li - ui) << " (" << static_cast<uint>(li + ui - ui) - (ui + li - ui) << ")\n";
	_STD cout << "Li * Ui: " << static_cast<uint>(li * ui) << " v " << (ui * li) << " (" << static_cast<uint>(li * ui) - (ui * li) << ")\n";
	_STD cout << "Li / Ui: " << static_cast<uint>(li / ui) << " v " << (static_cast<uint>(li) / ui) << " (" << (static_cast<uint>(li / ui) - static_cast<uint>(li) / ui) << ")\n";
	_STD cout << "Li % Ui: " << static_cast<uint>(li % ui) << " v " << (static_cast<uint>(li) % ui) << " (" << (static_cast<uint>(li % ui) - static_cast<uint>(li) % ui) << ")\n";

	std::cout << '\n';
	std::system("pause");
}