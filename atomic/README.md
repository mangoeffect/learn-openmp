## 起因
在多线程中读写同一个数据是一个非常危险的事情，可能会出现多个线程存在竞争导致数据结果发生异常。例如：

```cpp
int counter = 0;
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
            counter += 1;
        }
    }
    std::cout<< "couter = "<< counter << std::endl;
```
以上程序为开启4个线程并行执行for循环，但处理结果是对循环次数进行计数，存放到一个counter变量。并行运行过程中多个线程可能同时对counter变量进行读取和写入，造成竞争，导致计数结果可能是不准确的。对于线程间数据竞争，很常见的做法是加锁，对数据变量的读写进行线程的限制防止出现同时读写的问题。

```cpp
    std::mutex mtx;
    counter = 0;
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
            mtx.lock();
            counter += 1;
            mtx.unlock();
        }
    }
    std::cout<< "couter = "<< counter << std::endl;
```

## 用法

通过以上程序的加锁处理可以有效避免竞争导致的数据错误问题，但openmp并未提供锁机制，以上加锁处理是基于c++11内置的。如果像实现锁机制的效果，openmp的原子操作(atomic)与临界区(critical)可以实现相同效果，但原子操作效率会更高一些。原子操作的用法非常简单，只需在执行代码前添加openmp预处理即可

```cpp
#pragma omp atomic
        //执行代码语句
```
将上面循环计数程序修改为openmp原子操作：

```cpp
int counter = 0;
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
#pragma omp atomic
               counter += 1;
        }
    }
    std::cout<< "couter = "<< counter << std::endl;
```

同理，基于openmp的临界区(critical)机制也可以完成相同的修改：

```
int counter = 0;
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
#pragma omp critical
               counter += 1;
        }
    }
    std::cout<< "couter = "<< counter << std::endl;
```

下面对以上四组程序进行一下测试，测试结果如下：

```
origin couter = 1024875 cost 16 ms.
lock couter = 4000000 cost 595 ms.
atomic couter = 4000000 cost 96 ms.
critical couter = 4000000 cost 425 ms.
```
可以看到，加锁、原子操作与临界区操作结果是一致的，也是正确的，未作数据数据竞争处理的结果是错误的。耗时方面，加锁的效率是最低的，原子操作的效率是最高的。

## 延申

通常openmp的原子操作只需要加atomic处理即可，这是基本的用法，若像要对数据进行更精确的限制，openmp还提供了read、write和update的修饰限制，read用于多个线程同时读取同一个数据的情况，write用于多个线程同时写入一个数据的情况，而update则是用于多线程并行读写一个数据的情况，即本文中的例子。所以本文例子其实等价于

```cpp
#pragma omp parallel num_threads(4) shared(counter)
    {
        for (int i = 0; i < 1000000; i++)
        {
#pragma omp atomic update
               counter += 1;
        }
    }
```

read和write修饰作用是加强了数据的保护，具体用法例子可以参考以下例子：

```cpp
int AtomicRead(const int *x)
{
  int value;
#pragma omp atomic read
  value = *x;
  return value;
}
void AtomicWrite(int *x, int value)
{
  #pragma omp atomic write
  *x = value;
}

//parallel process
int xx = 0;
#pragma omp parallel num_threads(4) shared(xx)
    {
        for (int i = 0; i < 1000000; i++)
        {
            std::cout << AtomicRead(i) << std::endl;
            AtomicWrite(xx, i);
        }
    }
```


