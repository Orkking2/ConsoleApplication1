#include <iostream>
#include "Huff.h"

#include <functional>
template <class _Ty>
void test(nstd::HuffTree::uint num_tests, _STD function<_Ty(void)> rand_generator, bool err_switch = false) {
	std::vector<_Ty> vals;
	vals.resize(num_tests);
	_NSTD_FOR_I(num_tests)
		vals[i] = rand_generator();
	nstd::HuffTree t1;

	t1.create_tree(nstd::HuffTree::gen_freqs<_Ty>(vals));
	_STD deque<bool> d(t1.encode(vals));
	if (err_switch) {
		// Introduce error
		unsigned int error_pos(rand() % d.size());
		d[error_pos] = !d[error_pos];
		std::cout << "Error at pos " << error_pos << '\n';
	}

	_STD vector<_Ty> v(t1.decode<_Ty>(d));

	if (vals.size() != v.size()) {
		std::cout << "ERROR: vals.size() = " << vals.size() << " != v.size() = " << v.size();
		t1.release_ptrs<_Ty>();
		return;
	}

	_STD vector<unsigned int> indexi;
	_NSTD_FOR_I(vals.size()) {
		if (vals[i] != v[i]) {
			indexi.push_back(i);
		}
	}
	std::cout << "Errors:\n";
	if (!indexi.size())
		std::cout << "None\n";
	_NSTD_FOR_I(indexi.size())
		std::cout << indexi[i] << ", ";
	if(!indexi.empty())
		std::cout << "\nFrom ____ to ____:\n";
	_NSTD_FOR_I(indexi.size())
		std::cout << vals[indexi[i]] << "-" << v[indexi[i]] << ", ";

	std::cout << "\nSize of original array:\n" << sizeof(vals.front()) * vals.size();
	std::cout << "\nSize of encoded array:\n" << d.size() / 8;
	std::cout << "\nPercent of original size:\n" << ((double)d.size() / 8) / (sizeof(vals.front()) * vals.size()) * 100 << "%\n";

	t1.release_ptrs<_Ty>();
}

int main()
{
    unsigned int num_tests(10000);
	_NSTD_FOR_I(4) {
		test(num_tests, _STD function<int(void)>([]()->int { return rand() % 100; }));
		std::cout << "\n\n\n";
	}
	std::vector<int> int_list;
	int_list.resize(num_tests);
	_NSTD_FOR_I(num_tests)
		int_list[i] = rand() % 100;
	auto pairs = nstd::HuffTree::gen_freqs(int_list);

	std::cout << pairs.size() << "\n";
	_NSTD_FOR_I(pairs.size()) {
		if (*reinterpret_cast<int*> (pairs[i].first) == 0) {
			std::cout << pairs[i].second;
			break;
		}
	}
	std::cout << '\n';

	nstd::HuffTree t2;
	t2.create_tree(pairs);


	std::cout << t2.get_ptrs().size() << "\n\n";
	
	_NSTD_FOR_I(100) {
		_STD deque<bool> d = t2.encode(_STD vector<int>{ (int)i });
		_NSTD_FOR_J(d.size())
			std::cout << d[j];
		std::cout << ", ";
	}
	std::cout << "END ENCODE";
	std::cout << "\n\n";
	_NSTD_FOR_I(100) {
		std::cout << t2.decode<int>(t2.encode<int>(_STD vector{ (int)i })).size() << ", ";
	}
	std::cout << "\n\nEncode 0 test:\n";
	_STD deque<bool> d2 = t2.encode(_STD vector<int>{ (int)0 });
	_NSTD_FOR_I(d2.size())
		std::cout << d2[i];

	t2.release_ptrs<int>();
    std::cout << '\n';
    std::system("pause");
}