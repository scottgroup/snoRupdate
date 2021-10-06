#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <string>
#include <string.h>


#include "bed_creator.h"

using namespace std;

int main()
{
    // Hard codded, need to CHANGE THIS !
    string file = "/home/danx/Documents/projects/snoDB_snoRupdate/data/refseq.gtf";
    // Decalte functions
    void validateFile(const string&);
    void readFile(const string&, BedCreator &);

    validateFile(file);

    BedCreator bedCreator;

    readFile(file, bedCreator);

    return 0;
}

void validateFile(const string& file)
{
    // Make sure the file exists
    struct stat buffer;
    if((stat(file.c_str(), &buffer) == 0) == 0)
    {
        cerr << "File does not exist !" << endl;
        exit(0);
    }
}


void readFile(const string& file, BedCreator &bedCreator) {

    // Declare the variables
    ifstream fileStream(file);
    char _buffer[1024];
    fileStream.rdbuf()->pubsetbuf(_buffer, 16184);
    string line, field;
    // Decalte functions
    bool isSmallGene(vector<string>&);
    bool isSnoRNAOrScaRNA(string&);

    // initiate the position in line when searching
    size_t found;
    size_t start;

    // go through the file line by line
    while(getline(fileStream, line))
    {
        if(line[0] == '#') continue;
        start = 0; // reinitialize the first position
        found = 0;
        vector<string> fields(9, "");
        // go directly to the last column
        for(int i=0; i<9; i++)
        {
            found = line.find("\t", start+1);
            fields[i] = line.substr(start, found - start);
            start = found + 1;
        }

        if(isSmallGene(fields) && isSnoRNAOrScaRNA(fields[8])) {
            bedCreator.addEntry(fields);
        }
    }

    bedCreator.saveBedToFile();
}

bool isSmallGene(vector<string>& fields){
    int MAX_SNORNA_SIZE = 1000;
    if(fields[2] == "gene" && stoi(fields[4]) - stoi(fields[3]) <= MAX_SNORNA_SIZE)
    {
        return true;
    }
    return false;
}

bool isSnoRNAOrScaRNA(string& attributes){
    if(attributes.find("snoRNA") != string::npos
        || attributes.find("scaRNA") != string::npos)
    {
        return true;
    }
    return false;
}
