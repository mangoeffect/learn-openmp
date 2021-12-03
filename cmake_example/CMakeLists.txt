cmake_minimum_required(VERSION 3.14)

project(cmake_import)

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

if(OpenMP_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
else()
    message(FATAL_ERROR "OpenMP not found!")
endif()

