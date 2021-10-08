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
               const std::string suffix)
       : missingSnoRNAFile(missingSnoRNAFile),
         source(source),
         suffix(suffix)
         { init(); }

private:
    const std::string missingSnoRNAFile;
    const std::string source;
    const std::string suffix;
    void init();
    void buildEnsembl();
    void buildRefseq();
    std::vector< std::unordered_map<std::string, std::string> > fileReader();
    const std::string createEntry(std::unordered_map<std::string, std::string>&,
                                  const std::string& feature);
    const std::string getEnsemblAttributes(std::unordered_map<std::string, std::string>&,
                                            const std::string& feature);
};

#endif /* GTF_BUILDER_H */
