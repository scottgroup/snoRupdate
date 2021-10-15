// gtf_builder.h
#ifndef GTF_BUILDER_H
#define GTF_BUILDER_H

#include <vector>
#include <unordered_map>

class GtfBuilder
{
public:
    GtfBuilder(const std::string missingSnoRNAFile,
               const std::string source,
               const std::string gtfFile,
               const std::string suffix)
       : missingSnoRNAFile(missingSnoRNAFile),
         source(source),
         gtfFile(gtfFile),
         suffix(suffix)
         { init(); }

private:
    const std::string missingSnoRNAFile;
    const std::string source;
    const std::string gtfFile;
    const std::string suffix;
    void init();
    void buildNewEntries();
    std::vector< std::unordered_map<std::string, std::string> > fileReader();
    const std::string createEntry(std::unordered_map<std::string, std::string>&,
                                  const std::string& feature);
    const std::string getChr(const std::string& field);
    const std::string getAttributes(std::unordered_map<std::string, std::string>&,
                                            const std::string& feature);
    void writeNewGtf(std::vector<std::string> builtEntries);

    // Specific to Ensembl
    const std::string getEnsemblAttributes(std::unordered_map<std::string, std::string>&,
                                            const std::string& feature);
    // Specific to Refseq
    std::unordered_map<std::string, std::string> chrCorresp;
    const std::string getRefseqChr(const std::string& field);
    std::unordered_map<std::string, std::string> getChrCorresp();
    const std::string getRefseqAttributes(std::unordered_map<std::string, std::string>&,
                                            const std::string& feature);
};

#endif /* GTF_BUILDER_H */
