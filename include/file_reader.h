// file_reader.h
#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <vector>

#include "bed_creator.h" // To generate the bed file


class FileReader
{
public:
    FileReader(const std::string& file, const std::string& bedOutput)
      : gtfPath(file),
        bedOutPath(bedOutput)
        { init(); }

private:
    const std::string gtfPath;
    const std::string bedOutPath;
    BedCreator bedCreator;

    void init();
    void validateFile(const std::string&);
    void readFile(const std::string&);
    bool isSmallGene(std::vector<std::string>& fields);
    bool isSnoRNAOrScaRNA(std::string& attributes);
};

#endif /* FILE_READER_H */
