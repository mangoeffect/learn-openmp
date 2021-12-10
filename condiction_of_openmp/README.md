首先，在CMakeLists.txt中定义项目编译是否启用OpenMP的开关，基于cmake的option指令

```cmake
cmake_minimum_required(VERSION 3.14)

project(condiction_of_openmp)

# 项目是否启用openmp优化开关，默认启用
option(USE_OPENMP "option of enable openmp" ON)

if(USE_OPENMP)
    # 启用OpenMP优化
    find_package(OpenMP REQUIRED)
    if(OpenMP_FOUND)
        add_executable(with_openmp main.cpp)
        target_link_libraries(with_openmp OpenMP::OpenMP_CXX)
    else()
        message(FATAL_ERROR "openmp not found!")
    endif()
else()
    # 不启用OpenMP优化
    add_executable(without_openmp main.cpp)
endif()
```
基于以上的CMakeLists.txt，分别执行以下的构建命令可生成开启OpenMP优化和不包含OpenMP优化的项目：

```bash
cmake .. #默认开启OpenMP优化
cmake -DUSE_OPENMP=OFF .. #关闭OpenMP优化
```
完成在CMakeLists项目构建管理区分是否开启OpenMP优化后，仍需要在代码中区分是否采用OpenMP优化。一般而言在代码中区分不同编译版本采用宏定义区分是一种常见且好用的方法，这里也是采用宏定义区分代码中OpenMP优化版本与非优化版本的编译，基于OpenMP内置的一个_OPENMP宏定义，可以实现区分
```cpp
#include <iostream>

int main(int argc, char** argv)
{
#ifdef _OPENMP
    std::cout<< "Compiled with OpenMP." << std::endl;
#else
    std::cout<< "Compiled without OpenMP." << std::endl;
#endif
    return 0;
}
```
_OPENMP宏定义是编译器的一个内置宏定义，在添加openmp的编译器选项后该宏定义就会生效，基于该特性就可以如上文代码一样区分openmp的代码段了。下面测试该示例的编译情况。

```bash
cmake ..
-- The C compiler identification is GNU 9.3.0
-- The CXX compiler identification is GNU 9.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found OpenMP_C: -fopenmp (found version "4.5") 
-- Found OpenMP_CXX: -fopenmp (found version "4.5") 
-- Found OpenMP: TRUE (found version "4.5")  
-- Configuring done
-- Generating done
```

可以看到默认下是项目是开启了openmp优化，构建日志显示openmp版本信息。

```bash
 $ make
Scanning dependencies of target with_openmp
[ 50%] Building CXX object CMakeFiles/with_openmp.dir/main.cpp.o
[100%] Linking CXX executable with_openmp
[100%] Built target with_openmp
../build$ ./with_openmp 
Compiled with OpenMP.
```
make编译生成with_openmp可执行文件，执行后程序正确运行开启openmp优化的分支。接下来测试关闭openmp优化，cmake命令行构建项目时关闭USE_OPENMP选项。

```bash
cmake -DUSE_OPENMP=OFF ..
-- The C compiler identification is GNU 9.3.0
-- The CXX compiler identification is GNU 9.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
```

可以看到构建信息并未包含opnemp相关信息，接下来编译执行

```bash
$ make
Scanning dependencies of target without_openmp
[ 50%] Building CXX object CMakeFiles/without_openmp.dir/main.cpp.o
[100%] Linking CXX executable without_openmp
[100%] Built target without_openmp
$./without_openmp 
Compiled without OpenMP.
```

make编译生成的是名为without_openmp可执行文件，执行后程序正确执行的是未开启openmp优化的分支。
