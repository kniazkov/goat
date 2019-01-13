[ -f a.out ] && rm a.out
find . -name *.cpp -exec g++ -g -std=c++11 {} +
[ -f a.out ] && ./a.out
