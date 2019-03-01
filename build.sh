[ -f ./goat ] && rm ./goat
mkdir -p build
cd build
cmake ../src
make -j8
cd ..
ls -l build/goat
[ -f build/goat ] && cp build/goat .
[ -f ./goat ] && ./goat program.goat --dump-abstract-syntax-tree --dump-parse-tree --dump-assembler-code --lang=ru
