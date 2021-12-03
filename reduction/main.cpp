#include <omp.h>
#include <iostream>
#include <vector>
#include <limits>

int main(int argc, char** argv)
{
    //没有reduction处理
    int sum = 0;
#pragma omp parallel for
    for(int i = 0; i < 10; ++i)
    {
        sum += i;
    }
    std::cout << "without reduction sum = " << sum << std::endl;
    sum = 0;
    //reduction
#pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < 10; ++i)
    {
	 sum += i;
    }
    std::cout << "reduction sum = " << sum << std::endl;
    
    //最大最小值
    int max_num = std::numeric_limits<int>::min();
    std::vector<int> vec = {1, 7, 2, 6, 4, 8, 0, 3};
#pragma omp parallel for reduction(max:max_num)
    for(int i = 0;  i < vec.size(); ++i)
    {
	max_num = std::max(max_num, vec[i]);
    }
    std::cout << "max number = "<< max_num << std::endl;

    //多变量reduction,openmp仅支持单个归约操作符，以下例子包含+、min两种归约操作，编译不通过
    //可以支持多个归约变量使用同一个归约操作
    /*sum = 0;
    int min_num = std::numeric_limits<int>::max();
#pragma omp parallel for reduction(+:sum,min:min_num)
    for(int i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
	min_num = std::min(min_num, vec[i]);
    }
    std::cout << "sum = "<< sum << std::endl;
    std::cout << "min number = " << min_num << std::endl;
    */

    int sum1 = 0;
    int sum2 = 0;
#pragma omp parallel for reduction(+:sum1, sum2)
    for(int i = 0; i < vec.size(); ++i)
    {
       sum1 += i;
       sum2 += vec[i];
    }
    std::cout << "sum1 = "<< sum1 << std::endl;
    std::cout << "sum2 = "<< sum2 << std::endl;
    return 0;
}
