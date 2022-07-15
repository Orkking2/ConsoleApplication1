#include <iostream>
#include <bitset>
#include <compare>

#include "LongInt.h"
#include "Deque.h"
#include "Timer.h"

_NSTD_UNSIGNED

int divide(int dividend, int divisor, int origdiv) {
    int quotient(1);

    if (dividend == divisor) {
        return 1;
    } else if (dividend < divisor) {
        return 0;
    }

    while (divisor <= dividend) {
        divisor <<= 1;
        quotient <<= 1;
    }

    if (divisor > dividend) {
        divisor >>= 1;
        quotient >>= 1;
    }

    quotient += divide(dividend - divisor, origdiv, origdiv);

    return quotient;
}

int main()
{
    

	std::cout << '\n';
	std::system("pause");
}