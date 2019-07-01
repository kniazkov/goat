rm -rf build
rm ./goat -f
rm ./program.goat.* -f
rm ./*.png -f

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../src
make -j8
cd ..
ls -l build/goat
find ./test -name "output_act.txt" | xargs rm -f
find ./test -name "error_act.txt" | xargs rm -f
echo
python3 ./test.py build/goat -all -failed
