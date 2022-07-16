#include <iostream>
#include <bitset>
#include <compare>

#include "LongInt.h"
#include "Deque.h"
#include "Timer.h"

_NSTD_UNSIGNED

int division(int dividend, int divisor, int origdiv)
{
    int quotient = 1;

    if (dividend == divisor) {
        return 1;
    } else if (dividend < divisor) {
        return 0;
    }

    while (divisor <= dividend){
        divisor <<= 1;
        quotient <<= 1;
    }

    if (dividend < divisor){
        divisor >>= 1;
        quotient >>= 1;
    }

    return quotient += division(dividend - divisor, origdiv, origdiv);
}

void _Gen(nstd::LongInt l) {
    if (!l)
        return;
    --l;
    _Gen(l);
}

int main()
{
	nstd::LongInt li(256Ui64);
	uint ui(li);
	uint divisor(5);
	_STD cout << "Dividend: " << ui << "\nDivisor: " << divisor << "\nQuotient: " << static_cast<uint>(li / divisor) << "\nRealQ: " << ui / divisor;

    //_STD cout << division(ui, divisor, divisor) << " v " << ui/divisor;

	std::cout << '\n';
	std::system("pause");
}