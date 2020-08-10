#! /usr/bin/env bash

../my_grep -E "[a-c]ahar(i1|i)" test1 >tmp
grep '[a-c]ahari1\|[a-c]ahari' test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep -E "[a-c]ahar(i1|i)" test1 >tmp
grep '[a-c]ahari1\|[a-c]ahari' test1 | diff tmp -