#! /usr/bin/env bash

../my_grep bla bla >tmp
grep bla bla | diff tmp -

../my_grep -E "bla" bla >tmp
grep bla bla | diff tmp -

valgrind --quiet --leak-check=yes ../my_grep bla bla >tmp
grep bla bla | diff tmp -
