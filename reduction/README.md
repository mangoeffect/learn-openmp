## openmp-redcution使用场景

本文介绍openmp的reduction使用，reduction一般译为规约，与很多二分法中的合并操作。举一个最常见的例子，比如说你需要从一个整数数组中查找最小的数字，基于分治法的思想，把数组分为n个数组，分别找到每个数组中的最小值，得到n个最小值，最后从n个最小值中找到最小值即为整个数组的最小值。这就是openmp-reduction典型的使用场景，基于分治法的思想，将任务拆分为n个小任务，每个小任务分配线程并行执行，最后再将结果合并一起找到最终结果（规约）。所以reduction的使用场景一般包含以下两个特点的任务：

- 任务拥有大量重复的处理操作，可以基于分治法拆分为小任务，多线程并行
- 分别处理小任务后，需要对小任务结果进行合并处理。即最终的结果是依赖在全部数据、全部小任务的结果之上，并非各个小任务完全独立（如该这样直接使用parallel-for并行优化就可以）



## 例子

基于以上结论，reduction常见的使用例子有并行求和、并行求取最大最小值。openmp-reduction使用格式为

```cpp
#pragma omp parallel for reduction(reduction合并操作:并行处理的数组)
for(...)
///...
```
- reduction合并操作：原for循环中处理数组数据的具体操作，比如求和时为+，最大最小值则为max,min
- 并行处理的数组:输入的并行处理的数组数据

### 并行求和

```cpp
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
```

### 最大最小值求取

```cpp
//最大最小值
    int max_num = std::numeric_limits<int>::min();
    std::vector<int> vec = {1, 7, 2, 6, 4, 8, 0, 3};
#pragma omp parallel for reduction(max:max_num)
    for(int i = 0;  i < vec.size(); ++i)
    {
	max_num = std::max(max_num, vec[i]);
    }
    std::cout << "max number = "<< max_num << std::endl;
```

### 多个数组的reduction

openmp支持多个数组的同类型reduction，比如两个数组都需要求和时：

```cpp
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
```
但并不支持，多个数组多个reduction操作

```cpp

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
```


