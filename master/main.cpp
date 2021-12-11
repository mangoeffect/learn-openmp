#include <omp.h>
#include <iostream>

int main(int argc, char** argv)
{
    int master_v = 100;
/**
 * master不可用在parallel for中,编译报错
#pragma omp parallel for 
    for(int i = 0; i < 10; ++i)
    {
        int thread_id  = omp_get_thread_num();
	int v = 0;
#pragma omp master
	{
	    std::cout << "thread id = " << thread_id << std::endl;
	    v = master_v;
	}
	std::cout << "v = " << v << std::endl;
    }
 */
#pragma omp parallel
    {
        int v = 0;
#pragma omp master
	{
	    v = master_v;
		#pragma omp critical
	    std::cout << "master thread, v = "<< v << std::endl;
	}
#pragma omp critical
	std::cout << "v = "<< v  << std::endl;
    } 
    return 0;
}
