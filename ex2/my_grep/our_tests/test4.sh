#! /usr/bin/env bash

cat 3lines | ../my_grep bla > tmp
cat 3lines | grep bla | diff tmp -

cat 3lines | ../my_grep -E "bla" > tmp
cat 3lines | grep bla | diff tmp -

valgrind --quiet --leak-check=yes cat 3lines | ../my_grep bla > tmp
cat 3lines | grep bla | diff tmp -
