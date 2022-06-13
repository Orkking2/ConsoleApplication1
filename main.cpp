#include <iostream>
#include "Deque.h"
#include "Timer.h"


int main()
{
	nstd::deque<bool> d;
	d.push_back(true).push_front(false);
	0010;
	std::cout << d.size() << '\n';
	_NSTD_FOR_I(2)
		std::cout << d.pop_front() << ", ";


	std::cout << '\n';
	std::system("pause");
}