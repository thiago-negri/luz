#!/usr/bin/env bash

TESTFILES=$(find test/ -type f -name "*.test.c")
CFLAGS=$(cat compile_flags.txt)

set -x

rm -rf .test
mkdir -p .test

for file in $TESTFILES; do
	filename=$(basename $file .test.c)
	cc $file $CFLAGS -Iinclude -Llib -lluz -fsanitize=address -g -o .test/$filename
done

for file in $TESTFILES; do
	filename=$(basename $file .test.c)
	LD_LIBRARY_PATH=lib .test/$filename
done
