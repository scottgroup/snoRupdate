#include <iostream>

#include "file_reader.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Harcoded values, will make a config from this
    const string file = argv[1];
    const string snoRNAGtfFile = argv[2];
    const string snodbFile = argv[3];
    const string missingSnoRNAFile = argv[4];

    // Generate the snoRNA bed file for the gtf
    FileReader fileReader(file, snoRNAGtfFile);

    // bedtools intersect (STRANDENESS !?)
    string cmd = "bedtools intersect -wa -a " + snodbFile + " -b " + snoRNAGtfFile + " -v -s > " + missingSnoRNAFile;
    const char *command = cmd.c_str();
    system(command);

    return 0;
}
