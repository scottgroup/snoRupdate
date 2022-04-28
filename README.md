<img src="snoRupdate.png" alt="snoRupdate" width="400"/><hr>
# snoDB snoRupdate version 0.1

__Author__: Danny Bergeron\
__E-mail__: danny.bergeron@usherbrooke

# Description
snoRupdate is a simple tool that completes a gtf file (Ensembl or RefSeq
are supported) with snoRNAs present in snoDB and missing in a provided annotation.

## Requirements
- Make [Click here for installation guidelines](https://linuxhint.com/install-make-ubuntu/)
- Cmake [Click here for installation guidelines](https://cmake.org/install/)
- bedtools [Click here for installation guidelines](https://bedtools.readthedocs.io/en/latest/content/installation.html)

## Installing snoRupdate
First, you must clone the git repository to your favorite directory.
```bash
cd /path/to/clone/
git clone https://github.com/scottgroup/snoRupdate.git
```

Then you need to compile the tool with cmake and make. In order to do that,
you can simply run the `compile.sh` script from main directory:
```bash
./compile.sh
```

The compiled script (snoRupdate) will be in the `build` directory.

## Usage

#### Config file
Before running the script, you must modify the config file (config.ini) in the
main directory. First change the `gtfPath` to the path of your gtf file. It could
either be a absolute or a relative path. By default, you could put your gtf file
in the `data` directory and change the gtf name to fit yours. 
*Make sure that your gtf file is not compressed (.gtf and not .gtf.gz)

Then, if you are
using a gtf from RefSeq, change the `source` to RefSeq, and if it is from
ensembl, leave it as is. You can change the other values to suit your needs
if you want, but you can also leave them unchanged.

#### Running the tool
This is the simplest part. Just execute the `run.sh` script from the main folder.
```bash
./run.sh
```

It will download an input file from snoDB (a bed file containing all the
snoRNAs in snoDB), then run the snoRupdate tool, with the config file provided
in the `config.ini` file.

The final completed gtf file will be available in the same directory where
the original gtf file was located. The original file name will be prefixed
with the prefix provided in the config file (`_snoRNAs` by default).

That's it, you're done !

## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

For more information about the GNU General Public License, see http://www.gnu.org/licenses/.
