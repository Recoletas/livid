
# 设置最低要求的 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(Livid_Interpreter CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含所有源代码文件
set(SOURCE_FILES
    Interpreter.cpp
    livid.cpp
    main.cpp
    Parser.cpp
    Scanner.cpp
    Environment.cpp
)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*/*.cpp")

# 编译生成可执行文件 livid
# 确保所有 .cpp 文件都在这里
add_executable(livid ${SOURCE_FILES})

target_include_directories(livid PUBLIC
    ${PROJECT_SOURCE_DIR}/src/ast
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/interpreter
    ${PROJECT_SOURCE_DIR}/src/parser
    ${PROJECT_SOURCE_DIR}/src/scanner
    ${PROJECT_SOURCE_DIR}/src/environment
)