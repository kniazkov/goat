g++ -std=c++11 src/*.cpp src/code/*.cpp src/compiler/analyzer/*.cpp src/compiler/ast/*.cpp src/compiler/codegen/*.cpp src/compiler/common/*.cpp src/compiler/parser/*.cpp src/compiler/pt/*.cpp src/compiler/scanner/*.cpp src/compiler/source/*.cpp src/global/*.cpp src/launcher/*.cpp src/lib/*.cpp src/model/*.cpp src/model/built_in/*.cpp src/resource/*.cpp src/vm/*.cpp -Isrc -ldl -lpthread -g -o ./goat
