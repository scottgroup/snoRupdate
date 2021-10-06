#!/bin/bash

# http://derekmolloy.ie/hello-world-introductions-to-cmake/
cd build
cmake ..
make
echo -e 'compiling done !\n'

./snoRupdate \
    /home/danx/Documents/projects/snoDB_snoRupdate/data/ensembl.gtf \
    /tmp/snoRupdate_annotation_snoRNAs.bed \
    /home/danx/Documents/projects/snoDB_snoRupdate/data/snoDB_info.bed \
    /tmp/snoRupdate_missing_snoRNAs.bed
