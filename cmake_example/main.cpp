#include <omp.h>
#include <iostream>

int main(int argc, char** argv)
{
   std::cout<< "lean openmp: cmake import openmp example.\n";
#pragma omp parallel for
   for(int i = 0; i < 10; ++i)
   {
       std::cout<<"i = "<< i << std::endl;
   }
   return 0;
}
