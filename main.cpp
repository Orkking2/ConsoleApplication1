#include <iostream>
#include "Threads.h"


int main()
{
    unsigned int num_threads(4);
    nstd::thread_pool pool(num_threads);
    std::vector<nstd::thread_pool::_Pair_fvp> tasks;
    std::function<void(void*)> hello_world = [](void* p) { { std::lock_guard<std::mutex> lock(*reinterpret_cast<std::mutex*> (p)); std::cout << "Hello world from thread " << std::this_thread::get_id() << "\n"; } };
    std::mutex m;
    _NSTD_FOR(10)
        tasks.push_back(std::make_pair(hello_world, (void*)&m));
    pool.add_task(tasks);
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
