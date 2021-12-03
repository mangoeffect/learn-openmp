#include <omp.h>
#include <iostream>

int main(int argc, char** argv)
{
    /**
     * 方法一：#pragma omp parallel_for后直接跟随循环
     */
#pragma omp parallel for
    for(int i = 0; i < 10; ++i)
    {
        std::cout << i << std::endl;
    }

    /**
     * 方法二： parallel与for字段分开放置
     */
#pragma omp parallel 
{
#pragma omp for
    for(int i = 0; i < 10; ++i)
    {
	std::cout << i << std::endl;
    }
}
    /**
     * 要求：
     * 1. for循环索引变量为int类型
     * 2. 循环内数据不能发生依赖关系，必须是互不干扰的
     */
    return 0;
}
