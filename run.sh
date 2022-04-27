#!/bin/bash

# Download the input snoRNA file from snoDB
wget 'https://bioinfo-scottgroup.med.usherbrooke.ca/snoDB/download_snoRupdate' \
    --no-check-certificate \
    -O data/snoDB_snoRupdate_snoRNAs.bed

# Run the tool and provinding the config file path
./build/snoRupdate config.ini
