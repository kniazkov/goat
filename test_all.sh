./build.sh
find ./test -name "output_act.txt" | xargs rm
find ./test -name "error_act.txt" | xargs rm
echo
python ./test.py build/goat -all -failed