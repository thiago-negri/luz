#!/usr/bin/env bash

BUILDDIR=.build
LIBDIR=lib
CFLAGS=$(cat compile_flags.txt)

set -x

# Setup clean directories
rm -rf $BUILDDIR include/luz/gen $LIBDIR
mkdir -p $BUILDDIR include/luz/gen $LIBDIR

# Generate defines
cc src-gen/gen-defines.c -o $BUILDDIR/gen-defines
$BUILDDIR/gen-defines > include/luz/gen/gen-defines.h
rm $BUILDDIR/gen-defines

# Compile
cc src/luz.c $CFLAGS -c -O -o $BUILDDIR/luz.o
cc src/luz.c $CFLAGS -fPIC -c -O -o $BUILDDIR/luz.pic.o

# Package
cc -shared $BUILDDIR/luz.pic.o -o $LIBDIR/libluz.so
ar rcs $LIBDIR/libluz.a $BUILDDIR/luz.pic.o
