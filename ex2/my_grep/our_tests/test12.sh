#! /usr/bin/env bash

../my_grep -E "[a-c]ahar[a-y]" test1 >tmp
grep "[a-c]ahar[a-y]" test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -E "[a-c]ahar[a-y]" test1 >tmp
grep "[a-c]ahar[a-y]" test1 | diff tmp -
