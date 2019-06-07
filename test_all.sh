./build.sh
find ./test -name "output_act.txt" | xargs rm -f
find ./test -name "error_act.txt" | xargs rm -f
echo
python ./test.py build/goat -all -failed
