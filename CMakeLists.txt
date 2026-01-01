
# 设置最低要求的 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(Livid_Interpreter CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*/*.cpp")

# 编译生成可执行文件 livid
# 确保所有 .cpp 文件都在这里
add_executable(livid ${SOURCE_FILES})

target_include_directories(livid PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)