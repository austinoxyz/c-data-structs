#!/bin/sh

set -xe

warnings="-Wall -Wextra -Werror"
includes="-Iinclude"
build_dir="lib"

[ -d $build_dir ] && rm -r $build_dir
mkdir $build_dir

# 1 argument
#  - name (i.e. "my_string")
build() {
    name="$1"
    gcc -c -o "lib/$name.o" "src/$name.c" $warnings $includes
}

build my_string

echo "all builds completed succesfully."
