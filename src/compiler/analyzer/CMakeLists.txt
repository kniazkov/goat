cmake_minimum_required(VERSION 2.6)
project(analyzer)
file (GLOB SOURCE "*.cpp")
add_library(analyzer STATIC ${SOURCE})
