#! /usr/bin/env bash

../my_grep a emptyfile >tmp
grep a emptyfile | diff tmp -

../my_grep -E "a" emptyfile >tmp
grep a emptyfile | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep a emptyfile >tmp
grep a emptyfile | diff tmp -
