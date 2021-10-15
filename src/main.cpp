#include <iostream>
#include <unordered_map>

#include "inih/INIReader.h"

#include "file_reader.h"
#include "gtf_builder.h"

using namespace std;

void validateConfig(unordered_map<string, string> config)
{
    for (auto& it: config) {
        if(it.second == "UNKNOWN" || it.second == "")
        {
            cerr << "\n\e[31m'"
                 << it.first
                 << "' must be provided in the config.ini file !"
                 << "\n\e[39m"
                 << endl;
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    const string configFile = argv[1];
    INIReader reader(configFile);
    if (reader.ParseError() != 0) {
        cerr << "\e[31mCan't load 'config.ini' ! Wrong path ?\n\e[39m";
        return 1;
    }

    // Extraction of config elements from config file
    unordered_map<string, string> config({
        {"gtfFile", reader.Get("input", "gtfPath", "UNKNOWN")},
        {"snodbFile", reader.Get("input", "snodbFile", "UNKNOWN")},
        {"source", reader.Get("properties", "source", "UNKNOWN")},
        {"snoRNAGtfFile", reader.Get("intermediateFiles", "gtfExtractedSnoRNA", "UNKNOWN")},
        {"missingSnoRNAFile", reader.Get("intermediateFiles", "missingSnoRNAs", "UNKNOWN")},
        {"suffix", reader.Get("output", "suffix", "UNKNOWN")},
    });

    // Validate the config file
    validateConfig(config);

    // Generate the snoRNA bed file for the gtf
    FileReader fileReader(config["gtfFile"],
                          config["snoRNAGtfFile"],
                          config["source"]);

    // bedtools intersect
    string cmd = "bedtools intersect -wa -a " + config["snodbFile"] \
                 + " -b " + config["snoRNAGtfFile"] \
                 + " -v -s > " \
                 + config["missingSnoRNAFile"];
    const char *command = cmd.c_str();
    int bedtoolsReturn = system(command);

    // Check if bedtools was sucessful
    if(bedtoolsReturn)
    {
        cerr << "\n\e[31mBedtools command failed !\n\e[39m"
             << "\e[31mthe command was: \n\e[39m"
             << "\e[1m" + cmd + "\e[22m"
             << endl;
        return 1;
    }

    // Create the new entries for the missing snoRNAs
    GtfBuilder gtfBuilder(config["missingSnoRNAFile"],
                          config["source"],
                          config["gtfFile"],
                          config["suffix"]);

    return 0;
}
