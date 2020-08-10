#! /usr/bin/env bash

../my_grep -E ")" test1 >tmp
grep ')' test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -E ")" test1 >tmp
grep ')' test1 | diff tmp -

../my_grep -E "\)" test1 >tmp
grep ')' test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -E "\)" test1 >tmp
grep ')' test1 | diff tmp -


#../my_grep -E "(" test1 >tmp
#grep '(' test1 | diff tmp -

#valgrind --quiet --leak-check=yes ../my_grep -E "(" test1 >tmp
#grep '(' test1 | diff tmp -

../my_grep -E "\(" test1 >tmp
grep '(' test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -E "\(" test1 >tmp
grep '(' test1 | diff tmp -