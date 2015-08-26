#include "commonIncludes.h"
#include <iostream>
#include <vector>
#include <chrono>

#ifdef WITHBOOST
#include "threadPool.hpp"
#else
#include "ThreadPool.hpp"
#endif

void busyloop();

int main()
{
    
    ThreadPool pool(4);
    std::vector< std::shared_future<int> > results;
#ifdef WITHBOOST
		std::cout << "Running with boost version" << std::endl;
#else
		std::cout << "Running with std version" << std::endl;
#endif

		auto queTime = std::chrono::system_clock::now();
		auto endTime = std::chrono::system_clock::now();
		auto startTime = std::chrono::system_clock::now();
    for(int i = 0; i < 1024; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
								busyloop();
                return i*i;
            })
        );
    }
		queTime = std::chrono::system_clock::now();

    for(auto && result: results) result.get();
		endTime = std::chrono::system_clock::now();

		auto qdur = queTime - startTime;
		auto fdur = endTime - queTime;
		auto tdur = endTime - startTime;
		std::cout << "queue time: " << prettyPrint::format(qdur) << ", finish time: " << prettyPrint::format(fdur) << ", total time: " << prettyPrint::format(tdur) << std::endl;
    
    return 0;
}
