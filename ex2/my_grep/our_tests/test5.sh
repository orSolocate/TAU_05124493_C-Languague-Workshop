#! /usr/bin/env bash

../my_grep orb -b -n -A 3 test1 >tmp
grep orb -b -n -A 3 test1 | diff tmp -

../my_grep "orb" -b -E -n -A 3 test1 >tmp
grep orb -b -n -A 3 test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep orb -b -n -A 3 test1 >tmp
grep orb -b -n -A 3 test1 | diff tmp -
