#!/bin/sh
set -xe
cd ~/code/c-data-structs/test

[ -d ./build ] && rm -r ./build
mkdir build

[ $# -gt 1 ] && echo "too many arguments." && exit 1

warnings="-Wall -Wextra -Werror"
includes="-Iinclude -I../include"
objs="../lib/my_string.o"

# 1 argument
#  - name (i.e. "my_string")
build_test() {
    name="$1_test"
    gcc -std=c17 -o "build/$name" "src/$name.c" $warnings $includes $objs
}

alias finish_and_cleanup="echo all tests completed successfully.;rm -r build;cd -; exit 0"

if [ $# = 1 ]; then 
    build_test $1 && ./build/$1_test && finish_and_cleanup
fi

build_test my_string  && 
build_test my_vector  && 
build_test my_set     && 
build_test my_hashmap && 

./build/my_string_test   &&
./build/my_vector_test   &&
./build/my_set_test      &&
./build/my_hashmap_test  &&

finish_and_cleanup
