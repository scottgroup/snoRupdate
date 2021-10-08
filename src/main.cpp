#include <iostream>

#include "file_reader.h"
#include "inih/INIReader.h"

using namespace std;

int main(int argc, char *argv[])
{
    const string configFile = argv[1];
    INIReader reader(configFile);
    if (reader.ParseError() != 0) {

        cerr << "\e[31mCan't load 'config.ini' ! Wrong path ?\n\e[39m";
        return 1;
    }

    // Extraction of config elements from config file
    const string file = reader.Get("input", "gtfPath", "UNKNOWN");
    const string snodbFile = reader.Get("input", "snodbFile", "UNKNOWN");
    const string source = reader.Get("properties", "source", "UNKNOWN");
    const string snoRNAGtfFile = reader.Get("intermediateFiles", "gtfExtractedSnoRNA", "UNKNOWN");
    const string missingSnoRNAFile = reader.Get("intermediateFiles", "missingSnoRNAs", "UNKNOWN");

    // Generate the snoRNA bed file for the gtf
    FileReader fileReader(file, snoRNAGtfFile);

    // bedtools intersect
    string cmd = "bedtools intersect -wa -a " + snodbFile \
                 + " -b " + snoRNAGtfFile \
                 + " -v -s > " \
                 + missingSnoRNAFile;
    const char *command = cmd.c_str();
    int bedtoolsReturn = system(command);

    // Check if bedtools was sucessful
    if(bedtoolsReturn){
        cerr << "\n\e[31mBedtools command failed !\n\e[39m"
             << "\e[31mthe command was: \n\e[39m"
             << "\e[1m" + cmd + "\e[22m"
             << endl;
        return 1;
    }

    // TODO:
    //    - Create a bed from the snoDB id file, (-1 for start) and
    //      remove box_type (will be used to generate the new gtf entries)
    //    - add refseq or ensembl in the config file so I can create the
    //      GtfBuilder class and separate entries for refseq and ensembl
    //    - Get the box_type for each missing snoRNA to use for scaRNA/snoRNAs ?
    //    - Create gene, transcript and exon for each snoRNA

    return 0;
}
