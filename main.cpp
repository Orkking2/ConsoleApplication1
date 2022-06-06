#include <iostream>
#include "Deque.h"
#include "Huff.h"
#include "Timer.h"

#include <functional>
template <class _Ty>
void test(nstd::uint num_tests, _STD function<_Ty(void)> rand_generator, bool err_switch = false) {
	std::vector<_Ty> vals;
	vals.resize(num_tests);
	_NSTD_FOR_I(num_tests)
		vals[i] = rand_generator();

	nstd::HuffTree t1(nstd::HuffTree::gen_freqs<_Ty>(vals));
	_NSTD deque<bool> d(t1.encode(vals));
	if (err_switch) {
		unsigned int error_pos(rand() % d.size());
		// Introduce error
		d.flip(error_pos);
		std::cout << "Error at pos " << error_pos << '\n';
	}
	int d_sz_cashe(d.real_size());
	_STD vector<_Ty> v(t1.decode<_Ty>(d.shrink_fit()));

	if (vals.size() != v.size()) {
		std::cout << "ERROR: vals.size() = " << vals.size() << " != v.size() = " << v.size();
		return;
	}

	_STD vector<unsigned int> indexi;
	_NSTD_FOR_I(vals.size()) {
		if (vals[i] != v[i]) {
			indexi.push_back(i);
		}
	}
	std::cout << "Errors: ";
	if (!indexi.size())
		std::cout << "None";
	_NSTD_FOR_I(indexi.size())
		std::cout << indexi[i] << ", ";
	if(!indexi.empty())
		std::cout << "\nFrom ____ to ____:\n";
	_NSTD_FOR_I(indexi.size())
		std::cout << vals[indexi[i]] << "-" << v[indexi[i]] << ", ";


	std::cout << "\nSize of original array:\n" << sizeof(_Ty) * vals.size();
	std::cout << "\nSize unshrunk:\n" << d_sz_cashe;
	std::cout << "\nUnshrunk percent compression:\n" << (1 - static_cast<double> (d_sz_cashe) / (sizeof(_Ty) * vals.size())) * 100 << '%';
	std::cout << "\nSize shrunk:\n" << d.real_size();
	std::cout << "\nShrunk percent compression:\n" << (1 - static_cast<double> (d.real_size()) / (sizeof(_Ty) * vals.size())) * 100 << '%';
}

int main()
{
    unsigned int num_tests(100000);
	_NSTD_FOR_I(5) {
		std::cout << "\n\n";
		nstd::Timer t;
		test(num_tests * (i + 1), _STD function<char(void)>([]()->char { return (rand() % 100) > 10 ? 'a' : 'a' + rand() % 26; }));
	}
	

	

	std::cout << '\n';
	std::system("pause");
}