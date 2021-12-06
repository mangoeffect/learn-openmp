#include <omp.h>
#include <iostream>
#include <vector>

struct MyClass
{
    int data;
    MyClass(const int& data_): data(data_){}
    MyClass& operator = (const MyClass& other)
    {  
        return *this;
    }
    MyClass& operator += (const MyClass& other)
    {
        data += other.data;
        return *this;
    }
};


int main(int argc, char** argv)
{
    MyClass sum(0);
    std::vector<MyClass> vec(100, 1);

//定义线程数量大小的变量以存储每个线程的计算结果
    auto threads_count = omp_get_max_threads();
    std::vector<MyClass> sub_sum(threads_count, 0);

//并行计算各个线程的求和
#pragma omp parallel for
    for(int i = 0; i <  vec.size(); ++i)
    {
        sub_sum[omp_get_thread_num()].data += vec[i].data;
    }

//手动合并各个线程求和结果
    for(auto& s : sub_sum)
        sum.data += s.data;

    std::cout << "reduction by hand sum = "<< sum.data << std::endl;
    return 0;
}

