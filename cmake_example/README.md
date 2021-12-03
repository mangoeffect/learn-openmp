## cmake引入openmp

cmake引入openmp的方式非常简单，遵循以下步骤即可：

### 1 cmake find_package查找openmp

使用cmake中find_package指令查找openmp,格式如下

```cmake
#.. cmake other message
find_package(OpenMP REQUIRED)

#... cmake other message
```

### 2 cmake target_link_libraries链接openmp

与常见链接第三方库一样，使用target_link_libraries指令将openmp链接到具体项目，格式如下：

```cmake

# 在链接之前先判断是否已经搜索到openmp
if(OpenMP_FOUND)
    target_link_libraries(${you_executable_name} OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "openmp not found!")
endif()
```

### 3 代码中引入

在需要openmp并行优化的代码文件中包含openmp头文件，例子如下：

```cpp
#include <omp.h>
#include <iostream>

int main(int argc, char** argv)
{
   std::cout<< "lean openmp: cmake import openmp example.\n";
#pragma omp parallel for
   for(int i = 0; i < 10; ++i)
   {
       std::cout<<"i = "<< i << std::endl;
   }
   return 0;
}>
```

