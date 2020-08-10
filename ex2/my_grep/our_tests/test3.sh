#! /usr/bin/env bash

../my_grep orbb test1 >tmp
grep orbb test1 | diff tmp -

../my_grep -E "orbb" test1 >tmp
grep orbb test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep orbb test1 >tmp
grep orbb test1 | diff tmp -
