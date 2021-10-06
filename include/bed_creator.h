// bed_creator.h
#ifndef BED_CREATOR_H // include guard
#define BED_CREATOR_H

#include <iostream>
#include <vector>


class BedCreator
{
public:
    BedCreator();
    void addEntry(std::vector<std::string>&);
    void saveBedToFile();

private:
    const std::string geneName = "gene_name";
    const std::string geneId = "gene_id";
    std::vector< std::vector<std::string> > bedRows;
    std::vector<std::string> bedRow = std::vector<std::string>(6, "");

    std::string findAttribute(const std::string& attributes,
                              const std::string& attributeName);
};

#endif /* BED_CREATOR_H */
