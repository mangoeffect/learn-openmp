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