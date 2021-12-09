
在上两篇文章

- [【学习openmp-reduction】](https://mangoroom.cn/parallel-programming/learn-openmp-reduction.html)
- [【学习openmp-自定义reduction】](https://mangoroom.cn/parallel-programming/learn-openmp-custom-reduction.html)

介绍了openmp-reduction的用法，解决了并行循环下各个线程存在数据依赖，需要合并各个线程的计算结果的处理方法。该方法建立在使用openmp-reduction功能，该功能分为针对基础类型的版本和可自定义类型的自定义版本。而支持openmp自定义reduction功能需要openmp3.x以上版本，windows平台VS内置编译器仅支持openmp2.0版本，想在windows平台使用openmp自定义reduction功能需要使用其他编译器。具体方法可以看这篇文章介绍:

- [【学习openmp-windows平台使用新版本openmp】](https://mangoroom.cn/parallel-programming/learn-openmp-use-newversion-openmp-on-windows.html)

那有没有办法在windows平台基于VS的默认编译器做存在数据依赖的for循环并行优化呢？答案是有的，方法就是抛弃openmp内置reduction功能，基于parallel-for实现并行功能，最后手动实现归约合并操作。

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