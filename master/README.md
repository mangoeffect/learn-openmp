## 语法

master构造的语法如下面所示

```
#pragma omp master new-line 
   structured-block
```
用法一般为

```cpp
#pragma omp parallel
{
    /*
     ... 所有线程都执行的程序段
    */
    #pragma omp master
    {
        /*
        ... 仅主线程执行的程序段
        */
    }
}
```

## 例子

这里的主线程指的是在openmp并行优化开启之前的线程，举个例子

```cpp
  
  //在openmp多线程优化之前定义master_v变量，这是属于主线程的
  int master_v = 100;

#pragma omp parallel
    {
        int v = 0;
#pragma omp master
	{
	    v = master_v;
        //为避免线程干扰，使用critical保证单线程完整输出一行
		#pragma omp critical 
	    std::cout << "master thread, v = "<< v << std::endl;
	}
//为避免线程干扰，使用critical保证单线程完整输出一行
#pragma omp critical
	std::cout << "v = "<< v  << std::endl;
    } 
```

执行结果

```
v = 0
master thread, v = 100
v = 100
v = 0
v = 0
```

可以看到的是只有一个线程的v值被赋予了100的数值，与预期的结果相符合。需要注意的是在parallel for循环中master是不适用的，编译会报错。

```cpp
    int master_v = 100;
/**
 * master不可用在parallel for中,编译报错
#pragma omp parallel for 
    for(int i = 0; i < 10; ++i)
    {
        int thread_id  = omp_get_thread_num();
	int v = 0;
#pragma omp master
	{
	    std::cout << "thread id = " << thread_id << std::endl;
	    v = master_v;
	}
	std::cout << "v = " << v << std::endl;
    }
 */
 ```

