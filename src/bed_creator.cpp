// my_class.cpp
#include "bed_creator.h" // header in local directory
#include <iostream> // header in standard library

BedCreator::BedCreator() {}

void BedCreator::addEntry(std::vector<std::string> &fields)
{
    this->bedRow[0] = "chr" + fields[0]; // chr
    // start, need -1 for conversion to bed format
    this->bedRow[1] = std::to_string(std::stoi(fields[3]) - 1);
    this->bedRow[2] = fields[4]; // end
    this->bedRow[5] = fields[6]; // strand

    // dummy
    this->bedRow[3] = findAttribute(fields[8], this->geneName);
    this->bedRow[4] = findAttribute(fields[8], this->geneId);

    this->bedRows.push_back(this->bedRow);
    // Reinitialize the vector to default empty strings
    for(size_t i = 0; i < this->bedRow.size(); ++i)
        this->bedRow[i] = "";


    // for(auto& field : fields)
    // {
    //     std::cout << field << "\t";
    // }
    // std::cout << std::endl;
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

void BedCreator::saveBedToFile()
{
    for(auto& vec: this->bedRows)
    {
        for(std::string& field: vec)
        {
            std::cout << field << "\t";
        }
        std::cout << std::endl;
    }
}
