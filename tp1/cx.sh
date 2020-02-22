#!/bin/bash

name=$1
c_ext="$name.c"
o_ext="$name.o"

# removes the previous executables, not showing a message when they do not exist
rm $name $o_ext &>/dev/null

if gcc -Wall -o $name $c_ext &>/dev/null && ./$name ; then
    echo "Done"
else
    echo "Compilation error"
fi