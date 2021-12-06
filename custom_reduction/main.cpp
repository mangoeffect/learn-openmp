#include <omp.h>
#include <iostream>
#include <vector>

struct MyClass
{
    int data;
    MyClass(const int& data_): data(data_){}
    MyClass& operator += (const MyClass& other)
    {
        data += other.data;
        return *this;
    }
};

//语法
//#pragma omp declare reduction (reduction-identifier : typename-list : combiner) [initializer-clause]
//reduction-identifier：归约标识符，相当于openmp自带的+,这里命名为MyAdd
//typename-list: 归约操作的数据类型，这里为MyClass
//combiner: 合并链接具体操作，+=为具体操作，omp_out与omp_in为固定的标识符
//initializer-clause: 归约操作的每个线程的初始值，比如求和操作时赋值100则等效于100xn(线程数）基础上再求和数组,定义格式为initializer(omp_priv=MyClass(100)) 
#pragma omp declare reduction(MyAdd: MyClass: omp_out += omp_in) initializer(omp_priv=MyClass(0)) 

#pragma omp declare reduction(MyAdd_with100: MyClass: omp_out += omp_in) initializer(omp_priv=MyClass(100)) 


int main(int argc, char** argv)
{
    MyClass sum(0);
    std::vector<MyClass> vec(100, 1);
    
#pragma omp parallel for reduction(MyAdd: sum)
    for(int i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    std::cout << "custom reduction sum = " <<sum.data << std::endl;
 
    sum.data = 0;
#pragma omp parallel for reduction(MyAdd_with100: sum)
    for(int i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
        std::cout << "initializer value = 100,  threads count = "<< omp_get_num_threads() << std::endl;
    }

    std::cout << "custom reduction with 100  initializer sum = " <<sum.data << std::endl;
    return 0;
}
