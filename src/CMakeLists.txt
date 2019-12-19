# Copyright (C) 17-19 Ivan Kniazkov

# This file is part of interpreter of programming language
# codenamed "Goat" ("Goat interpreter").

# Goat interpreter is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Goat interpreter is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  add_compile_options(-std=c++11 -O3 -Werror)
else()
  add_compile_options(-std=c++11 -g -Werror)
endif()
project(f_ck)
set (SOURCE main.cpp)
add_executable(goat ${SOURCE})
add_subdirectory(code)
add_subdirectory(compiler)
add_subdirectory(launcher)
add_subdirectory(lib)
add_subdirectory(global)
add_subdirectory(resource)
add_subdirectory(model)
add_subdirectory(vm)
target_link_libraries(goat
    lib
    global
    resource
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
    compiler_common
)
