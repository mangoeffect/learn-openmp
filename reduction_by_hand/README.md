
## 步骤

以并行求和为例，首先为各个线程分别定义一个线程私有变量，并初始化

```cpp
//定义线程数量大小的变量以存储每个线程的计算结果
auto threads_count = omp_get_max_threads();
std::vector<MyClass> sub_sum(threads_count, 0);
```

使用parallel-for并行处理每个线程，每个线程结果存放到该线程的私有变量

```cpp
//并行计算各个线程的求和
#pragma omp parallel for
    for(int i = 0; i <  vec.size(); ++i)
    {
        //omp_get_thread_num()获取当前线程的id
        sub_sum[omp_get_thread_num()].data += vec[i].data;
    }
```

合并各个线程的计算结果

```cpp
//手动合并各个线程求和结果
    for(auto& s : sub_sum)
        sum.data += s.data;
```