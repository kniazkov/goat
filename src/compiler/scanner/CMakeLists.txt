cmake_minimum_required(VERSION 2.6)
project(scanner)
file (GLOB SOURCE "*.cpp")
add_library(scanner STATIC ${SOURCE})
