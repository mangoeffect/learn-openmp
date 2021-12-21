cmake_minimum_required(VERSION 3.14)

project(atomic)

find_package(OpenMP REQUIRED)

if(OpenMP_FOUND)
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "openmp not found!")
endif()