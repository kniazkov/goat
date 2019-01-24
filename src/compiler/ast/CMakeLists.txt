cmake_minimum_required(VERSION 2.6)
project(ast)
file (GLOB SOURCE "*.cpp")
add_library(ast STATIC ${SOURCE})
