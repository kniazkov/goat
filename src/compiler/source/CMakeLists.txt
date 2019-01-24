cmake_minimum_required(VERSION 2.6)
project(source)
file (GLOB SOURCE "*.cpp")
add_library(source STATIC ${SOURCE})
