cmake_minimum_required(VERSION 2.6)
add_compile_options(-std=c++11 -g)
project(f_ck)
set (SOURCE main.cpp)
add_executable(goat ${SOURCE})
add_subdirectory(code)
add_subdirectory(compiler)
add_subdirectory(launcher)
#add_subdirectory(lib)
add_subdirectory(model)
add_subdirectory(vm)
target_link_libraries(goat
    code
    launcher
    model
    built_in
    vm
    analyzer
    ast
    codegen
    parser
    ptree
    scanner
    source
)
