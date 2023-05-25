#!/bin/sh
set -xe
cd ~/Code/c-data-structs/test

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
build_test my_hashmap 

echo "all builds completed successfully"
cd -
