cmake_minimum_required(VERSION 2.6)
project(ptree)
file (GLOB SOURCE "*.cpp")
add_library(ptree STATIC ${SOURCE})
