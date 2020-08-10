#! /usr/bin/env bash

../my_grep cocoman test1 >tmp
grep cocoman test1 | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep cocoman test1 >tmp
grep cocoman test1 | diff tmp -
