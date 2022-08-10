#include <iostream>
#include <bitset>
#include <compare>
#include <vector>

//#define _NSTD_LONGINT_DEBUGGING_

#include "LongInt.h"
//#include "Deque.h"
//#include "Timer.h"

_NSTD_UNSIGNED

struct dictionary {
	static bool is_word(const char* w) {
		return true;
	}
};

constexpr char** int_to_char{ {'a', 'b', 'c'}, {'e', 'f', 'g'} };



_STD vector<_STD vector<char>> word_arr(const _STD vector <char> prev_list, const _STD vector<int> ints, int index) {
	_STD vector<char> temp(prev_list);
	_STD vector<_STD vector<char>> out;
	
	if (index == ints.size()) {
		_NSTD_FOR_I((ints[index] == 7 || ints[index] == 9 ? 4 : 3)) {
			temp.push_back(int_to_char[ints[index]][_I]);
			if (dictionary::is_word(temp.data()))
				out.push_back(temp);
			temp.pop_back();
		}
	} else {
		_NSTD_FOR_I((ints[index] == 7 || ints[index] == 9 ? 4 : 3)) {
			_STD vector<_STD vector<char>> out_rec(word_arr(temp.push_back(int_to_char[ints[index]][_I]), ints, index + 1));
			if (out_rec.size()) {
				out.insert(out.begin(), out_rec.begin(), out_rec.end());
			}
			temp.pop_back();
		}
	}
	return out;
}





int main()
{
	const char* in;
	

	std::cout << '\n';
	std::system("pause");
}