// file_reader.cpp
#include <iostream> // header in standard library
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "file_reader.h" // header in local directory
#include "bed_creator.h" // header in local directory

void FileReader::init() {
    validateFile(this->gtfPath);

    readFile(this->gtfPath);

    this->bedCreator.saveBedToFile(this->bedOutPath);
}

void FileReader::validateFile(const std::string& file)
{
    // Make sure the file exists
    struct stat buffer;
    if((stat(file.c_str(), &buffer) == 0) == 0)
    {
        std::cerr << "\e[31mFile does not exist !\e[39m" << std::endl;
        exit(0);
    }
}

void FileReader::readFile(const std::string& file) {

    // Declare the variables
    std::ifstream fileStream(file);
    char _buffer[1024];
    fileStream.rdbuf()->pubsetbuf(_buffer, 16184);
    std::string line, field;

    // initiate the position in line when searching
    std::size_t found;
    std::size_t start;

    // go through the file line by line
    while(getline(fileStream, line))
    {
        if(line[0] == '#') continue;
        start = 0; // reinitialize the first position
        found = 0;
        std::vector<std::string> fields(9, "");
        // go directly to the last column
        for(int i=0; i<9; i++)
        {
            found = line.find("\t", start+1);
            fields[i] = line.substr(start, found - start);
            start = found + 1;
        }

        if(this->isSmallGene(fields) && this->isSnoRNAOrScaRNA(fields[8])) {
            this->bedCreator.addEntry(fields);
        }
    }
}

bool FileReader::isSmallGene(std::vector<std::string>& fields){
    int MAX_SNORNA_SIZE = 1000;
    if(fields[2] == "gene" && std::stoi(fields[4]) - std::stoi(fields[3]) <= MAX_SNORNA_SIZE)
    {
        return true;
    }
    return false;
}

bool FileReader::isSnoRNAOrScaRNA(std::string& attributes){
    if(attributes.find("snoRNA") != std::string::npos
        || attributes.find("scaRNA") != std::string::npos
        || attributes.find("guide_RNA") != std::string::npos // for refseq, some snoRNA are guide_RNA
        || attributes.find("SNORD") != std::string::npos) // for refseq, some pseudogene are SNORDs
    {
        return true;
    }
    return false;
}
