#include <iostream>
#include "Huff.h"


int main()
{
    std::vector<char> vals;
    _NSTD_FOR_I(100) 
        vals.push_back('a' + rand() % 26);

    std::cout << "Original:\n";
    _NSTD_FOR_I(vals.size())
        std::cout << vals[i];
    
    nstd::HuffTree t1;

    t1.create_tree(nstd::HuffTree::gen_freqs(vals));
    _STD deque<bool> d(t1.encode(vals));
    std::cout << "\nEncode:\n";
    _NSTD_FOR_I(d.size())
        std::cout << d[i];

    _STD vector<char> v(t1.decode<char>(d));
    std::cout << "\nDecode:\n";
    _NSTD_FOR_I(v.size())
        std::cout << v[i];

    _STD vector<unsigned int> indexi;
    _NSTD_FOR_I(vals.size()) {
        if (vals[i] != v[i]) {
            indexi.push_back(i);
        }
    }
    std::cout << "\nErrors:\n";
    if (!indexi.size())
        std::cout << "None";
    _NSTD_FOR_I(indexi.size())
        std::cout << indexi[i] << ", ";

    t1.release_ptrs<char>();
    std::cout << '\n';
    std::system("pause");
}