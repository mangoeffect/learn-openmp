project(use_openmp_newversion_on_windows CXX)

# 推荐环境
# Visual Studio 2019 version 16.10.0 or later
# Install clang-llvm module on visual Studio

# 如何使用
# cmake -G "Visual Studio 16 2019" -T ClangCL ..

# for use llvm openm define
SET(COMMON_COMPILE_FLAGS "-fopenmp")
# find openmp
find_package(OpenMP)

# target link openmp
add_executable(${PROJECT_NAME} "main.cpp")
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

