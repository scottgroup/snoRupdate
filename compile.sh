#!/bin/bash

# http://derekmolloy.ie/hello-world-introductions-to-cmake/
cd build
cmake ..
make
echo -e 'compiling done !\n'

./snoRupdate
