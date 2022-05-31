#include <iostream>
#include "Huff.h"


int main()
{
    std::unordered_map<int, int> map;
    _NSTD_FOR_I(100) {
        _NSTD_FOR(j, 5) {
            if (!map.contains(j)) {
                map[j] = 1;
            } else {
                map[j]++;
            }
        }
    }
    _NSTD_FOR_I(5)
        std::cout << map[i] << '\n';
    std::system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
