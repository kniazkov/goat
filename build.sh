mkdir -p build
cd build
cmake ../src
make
cd ..
ls -l build/goat
[ -f build/goat ] && build/goat program.goat --dump-abstract-syntax-tree --dump-parse-tree --dump-assembler-code
