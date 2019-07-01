./build.sh
find ./test -name "output_act.txt" | xargs rm -f
find ./test -name "error_act.txt" | xargs rm -f
echo
python3 ./test.py build/goat -all -failed
python3 ./test.py build/goat -debug -all -failed
