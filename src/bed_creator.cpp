// bed_creator.cpp
#include "bed_creator.h"
#include <iostream>
#include <fstream>


void BedCreator::setSource(const std::string source) {
    this->source = source;
    if(this->source != "Ensembl" && this->source != "RefSeq")
    {
        std::cerr << "\e[31m'"
             << this->source
             << "' is not a valid or supported source ! "
             << "Valid sources are: 'Ensembl' or 'RefSeq'."
             << "\e[39m\n";
        exit(1);
    }
}

void BedCreator::addEntry(std::vector<std::string> &fields)
{
    this->bedRow[0] = parseChr(fields[0]); // chr
    // start, need -1 for conversion to bed format
    this->bedRow[1] = std::to_string(std::stoi(fields[3]) - 1);
    this->bedRow[2] = fields[4]; // end
    this->bedRow[5] = fields[6]; // strand

    this->bedRow[3] = findAttribute(fields[8], this->geneId); // get the gene_id
    this->bedRow[4] = "."; // empty score

    this->bedRows.push_back(this->bedRow);
    // Reinitialize the vector to default empty strings
    for(size_t i = 0; i < this->bedRow.size(); ++i)
        this->bedRow[i] = "";
}

std::string BedCreator::parseChr(const std::string& rawChrom)
{
    if(rawChrom.rfind("NC_", 0) == 0)
    {
        // Check if it is really RefSeq
        if(this->source == "Ensembl")
        {
            std::cerr << "\e[31mChromosome contains RefSeq nomenclature (NC_)... "
                          << "Maybe the 'source' in the config.ini file should be "
                          << "Ensembl ?\n\e[39m";
                exit(1);
        }

        int underscore = rawChrom.find("_");
        int dotPos = rawChrom.find(".");
        int chr = std::stoi(rawChrom.substr(underscore+1, dotPos));

        if(chr == 23) { return "chrY"; }
        else if(chr == 24) { return "chrX"; }
        else { return "chr" + std::to_string(chr); }
    } else if(rawChrom[0] == 'N')
    {
        return rawChrom;
    } else
    {
        return "chr" + rawChrom;
    }
}



std::string BedCreator::findAttribute(const std::string& attributes,
                                      const std::string& attributeName)
{
    int start = 0;
    int found = 0;
    std::string attribute;
    found = attributes.find(attributeName, start);

    // If the attribute is not present
    if(found == std::string::npos)
        return "";

    start = found + 1;
    found = attributes.find("\"", start);
    start = found + 1;
    found = attributes.find("\"", start);

    attribute = attributes.substr(start, found - start);

    return attribute;
}

void BedCreator::saveBedToFile(const std::string bedOutPath)
{
    std::ofstream outfile(bedOutPath);

    for(auto& vec: this->bedRows)
    {
        for(std::size_t i = 0; i < vec.size(); ++i)
        {
            outfile << vec[i];
            if(i != vec.size() - 1)
                outfile << "\t";
        }
        outfile << std::endl;
    }

    outfile.close();
}
