#! /usr/bin/env bash

../my_grep -x -E "." test1 >tmp
grep -x '.' test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -x -E "." test1 >tmp
grep -x '.' test1 | diff tmp -


../my_grep -x -E " " test1 >tmp
grep -x ' ' test1 | diff tmp -


valgrind --quiet --leak-check=yes ../my_grep -x -E " " test1 >tmp
grep -x ' ' test1 | diff tmp -



