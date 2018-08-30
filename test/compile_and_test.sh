echo compilation...
cd ../subject
./build_dbg.sh
cd ../test
python3 ./test.py ../subject/a.out -failed

