if [ -n "$1" ]
then
    if [ -d test/$1 ]
    then
        echo Test \'$1\' already exists!
    else
        echo Create test \'$1\'...
        mkdir test/$1
        cp program.goat test/$1
        python test.py build/goat -select=$1
        [ -f test/$1/output_act.txt ] &&  mv test/$1/output_act.txt test/$1/output.txt
        [ -f test/$1/error_act.txt ] &&  mv test/$1/error_act.txt test/$1/error.txt
        python test.py build/goat -select=$1
        git add test/$1/*
        git commit -am "+ test"
        echo Done.
    fi
else
    echo "Usage:"
    echo "    ./create_test.sh <test_name>"
fi
