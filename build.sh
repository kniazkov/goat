rm ./goat -f
rm ./program.goat.* -f
rm ./*.png -f
mkdir -p build
cd build
cmake ../src
make -j1
cd ..
ls -l build/goat
[ -f build/goat ] && cp build/goat .
[ -f ./goat ] && ./goat program.goat --compile --dump-abstract-syntax-tree --dump-parse-tree --dump-assembler-code --lang=ru --lib=./lib
[ -f ./program.goat.bin ] && ./goat --bin program.goat.bin --dump-memory-usage-report --heap-size=256 --lang=ru
[ -f ./program.goat.tokens.txt ] && dot -Tpng program.goat.tokens.txt -oprogram.goat.tokens.png
#[ -f ./program.goat.ptree.txt ] && dot -Tpng program.goat.ptree.txt -oprogram.goat.ptree.png
