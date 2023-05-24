#!/bin/sh

set -xe

[ -d ./build ] && rm -r ./build
mkdir build

warnings="-Wall -Wextra -Werror"
includes="-Iinclude -I../include"
objs="../lib/my_string.o"

# 1 argument
#  - name (i.e. "my_string")
build_test() {
    name="$1_test"
    gcc -o "build/$name" "src/$name.c" $warnings $includes "$objs"
}

build_test my_string  && 
build_test my_vector  && 
build_test my_set     && 
build_test my_hashmap && 

./build/my_string_test   &&
./build/my_vector_test   &&
./build/my_hashmap_test  &&
./build/my_set_test      &&

echo "all tests completed succesfully."
