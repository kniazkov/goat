cmake_minimum_required(VERSION 2.6)
project(codegen)
file (GLOB SOURCE "*.cpp")
add_library(codegen STATIC ${SOURCE})
