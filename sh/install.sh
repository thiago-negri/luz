#!/usr/bin/env bash

set -x

mkdir -p \
	/usr/local/lib \
	/usr/local/include/luz \
	/usr/local/include/luz/gen

cp lib/libluz.so /usr/local/lib/libluz.so
cp lib/libluz.a /usr/local/lib/libluz.a
cp include/luz/luz.h /usr/local/include/luz/luz.h
cp include/luz/gen/gen-defines.h /usr/local/include/luz/gen/gen-defines.h
