[ -f a.out ] && rm a.out
find . -name *.cpp -exec g++ -g -std=c++11 {} +
ls -l ./a.out
[ -f a.out ] && ./a.out program.goat --dump-abstract-syntax-tree --dump-parse-tree --dump-assembler-code
