#!/bin/bash

# http://derekmolloy.ie/hello-world-introductions-to-cmake/
mkdir -p build
cd build
cmake ..
make
echo -e '\n\033[0;32mCompilation done !\n'
