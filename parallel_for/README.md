
本文介绍openmp parallel for的使用方法，parallel for的作用是将for循环做并行优化。例如原本有如下的for循环处理程序：

```cpp
for(int i = 0; i < 10; ++i)
{
    std::cout << i << std::endl;
}
```

程序非常简单，正常执行结果应为：

```
0
1
..
9
```
在这基础之上使用openmp做并行优化非常简单，秩序在for循环代码之前添加下面一句处理

```cpp
#pragma omp parallel for
```
即

```cpp
    /**
     * 方法一：#pragma omp parallel_for后直接跟随循环
     */
#pragma omp parallel for
    for(int i = 0; i < 10; ++i)
    {
        std::cout << i << std::endl;
    }
```
或者 parallel与for字段分开声明

```cpp

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
```

即可完成for循环的并行优化，结果执行可能为：

```
0
952

8
3
1
6
7
```

paralel for并行优化非常简单，但需要注意以下两点：

- **1.for循环索引变量为int类型**

```cpp
for(size_t i = 0; i < 10; ++i) //i不可使用size_t类型
```

- **2.循环内数据不能发生依赖关系，必须是互不干扰的**

```cpp
std::vector<int> vec(10, 1);
for(int i = 1; i < 10; ++i)
{
    //数据处理不互相独立，并行优化会存在隐患
    std::cout << vec[i] + [i - 1] << std::endl;
}
```

最后，测试代码如下：

```cpp
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
```
并行输出的结果不固定，每次的结果可能都不一样。可能的测试结果为：

```
0
952

8
3
1
6
7

4
97
0

81
64
52


3
```


