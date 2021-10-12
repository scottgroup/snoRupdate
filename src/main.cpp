#include <iostream>

#include "inih/INIReader.h"

#include "file_reader.h"
#include "gtf_builder.h"

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
    const string gtfFile = reader.Get("input", "gtfPath", "UNKNOWN");
    const string snodbFile = reader.Get("input", "snodbFile", "UNKNOWN");
    const string source = reader.Get("properties", "source", "UNKNOWN");
    const string snoRNAGtfFile = reader.Get("intermediateFiles", "gtfExtractedSnoRNA", "UNKNOWN");
    const string missingSnoRNAFile = reader.Get("intermediateFiles", "missingSnoRNAs", "UNKNOWN");
    const string suffix = reader.Get("output", "suffix", "UNKNOWN");

    // Generate the snoRNA bed file for the gtf
    FileReader fileReader(gtfFile, snoRNAGtfFile);

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

    // Create the new entries for the missing snoRNAs
    GtfBuilder gtfBuilder(missingSnoRNAFile, source, gtfFile, suffix);

    // TODO:
    //    - Append to the file for Ensembl and append, but remove the last
    //      line for RefSeq

    return 0;
}
