#!/bin/bash

mydir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p $mydir/build/arm
cd $mydir/build/arm

cmake $mydir -DPROJECT_TITLE=SCAndSkip -DANDROID_ABI=armeabi-v7a
make -j4

if [ -f "$mydir/build/arm/libSCAndSkip.so" ]; then
	mv $mydir/build/arm/libSCAndSkip.so $mydir/build
fi

mkdir -p $mydir/build/arm64
cd $mydir/build/arm64

cmake $mydir -DPROJECT_TITLE=SCAndSkip64 -DANDROID_ABI=arm64-v8a
make -j4

if [ -f "$mydir/build/arm64/libSCAndSkip64.so" ]; then
	mv $mydir/build/arm64/libSCAndSkip64.so $mydir/build
fi