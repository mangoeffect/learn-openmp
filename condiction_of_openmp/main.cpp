#include <iostream>

int main(int argc, char** argv)
{
#ifdef _OPENMP
    std::cout<< "Compiled with OpenMP." << std::endl;
#else
    std::cout<< "Compiled without OpenMP." << std::endl;
#endif
    return 0;
}

