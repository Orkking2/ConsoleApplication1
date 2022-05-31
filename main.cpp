#include <iostream>
#include "Huff.h"


int main()
{
    std::vector<char> vals;
    std::vector<char> chars(_NSTD_ITSL);
    _NSTD_FOR_I(100) 
        vals.push_back(chars[rand() % chars.size()]);
    
    nstd::HuffTree t1;

    t1.create_tree(nstd::HuffTree::gen_freqs(vals));
    auto map(t1.gen_val_map<char>());

    std::system("pause");
}