#!/bin/bash

# Loading the submodule inig
git submodule init
git submodule update

# http://derekmolloy.ie/hello-world-introductions-to-cmake/
mkdir -p build
cd build
cmake ..
make
echo -e '\n\033[0;32mCompilation done !\n'
