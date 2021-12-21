#include <omp.h>
#include <iostream>
#include <mutex>
#include <chrono>

int main(int argc, char** argv)
{

    int counter = 0;
    auto t0 = std::chrono::system_clock::now();
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
            counter += 1;
        }
    }
    auto t1 = std::chrono::system_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout<< "origin couter = "<< counter << " cost "<< t << " ms."<<std::endl;

    std::mutex mtx;
    counter = 0;
    t0 = std::chrono::system_clock::now();
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
            mtx.lock();
            counter += 1;
            mtx.unlock();
        }
    }
    t1 = std::chrono::system_clock::now();
    t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout<< "lock couter = "<< counter << " cost "<< t << " ms."<<std::endl;

    counter = 0;
    t0 = std::chrono::system_clock::now();
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
#pragma omp atomic 
               counter += 1;
        }
    }
    t1 = std::chrono::system_clock::now();
    t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout<< "atomic couter = "<< counter << " cost "<< t << " ms."<<std::endl;

    counter = 0;
    t0 = std::chrono::system_clock::now();
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
#pragma omp critical 
               counter += 1;
        }
    }
    t1 = std::chrono::system_clock::now();
    t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout<< "critical couter = "<< counter << " cost "<< t << " ms."<<std::endl;
    return 0;
}
