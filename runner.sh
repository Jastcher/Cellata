#! /bin/bash

mkdir -p Build >/dev/null 2>&1
dir=$(dirname $(which $0 2>/dev/null || realpath ./$0))

cd $dir/Build
cmake ../

cmake --build .

if [ $1 ]; then
  ./Sandbox
fi

cd ..
