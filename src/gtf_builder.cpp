// gtf_builder.cpp
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include "gtf_builder.h"

void GtfBuilder::init()
{
    if(this->source == "Ensembl")
        buildEnsembl();
    else if(this->source == "RefSeq")
        buildRefseq();
    else
    {
        std::cerr << "\e[31m'"
             << this->source
             << "' is not a valid or supported source ! "
             << "Valid sources are: 'Ensembl' or 'RefSeq'."
             << "\e[39m\n";
        exit(1);
    }
}

void GtfBuilder::buildEnsembl()
{
    std::cout << "buildEnsembl !" << std::endl;

    std::vector< std::unordered_map<std::string, std::string> > missingEntries = this->fileReader();
    std::vector<std::string> entryTypes = {"gene", "transcript", "exon"};
    std::vector<std::string> builtEntries;

    // Iterate over the missing entries
    for(int i = 0; i < missingEntries.size(); ++i)
    {
        for(const std::string& entryType: entryTypes)
        {
            std::string entry = createEntry(missingEntries[i], entryType);
            builtEntries.push_back(entry);
        }
    }

    // Print for now, but should be added to the gtf file (concatenation) // TODO !!
    for(const std::string& line: builtEntries)
    {
        std::cout << line;
    }

    std::cout << "buildEnsembl  Done !" << std::endl;
}

void GtfBuilder::buildRefseq()
{
    // TODO
    std::cout << "buildRefseq !" << std::endl;
}

std::vector< std::unordered_map<std::string, std::string> > GtfBuilder::fileReader()
{
    // Declare the variables
    std::vector< std::unordered_map<std::string, std::string> > missingEntries;
    std::vector<std::string> fieldNames = { "chr", "start", "end", "id", "name", "strand", "box_type"};
    std::unordered_map<std::string, std::string> snoEntry;

    // Initialize file stream
    std::ifstream fileStream(this->missingSnoRNAFile);
    char _buffer[1024];
    fileStream.rdbuf()->pubsetbuf(_buffer, 16184);
    std::string line, field;

    // initiate the position in line when searching
    std::size_t found;
    std::size_t start;
    std::size_t position;

    // go through the file line by line
    while(getline(fileStream, line))
    {
        start = 0; // reinitialize the first position
        found = 0;
        position = 0;
        // go directly to the last column
        for(size_t i=0; i<fieldNames.size(); ++i)
        {
            found = line.find("\t", start);
            snoEntry[fieldNames[i]] = line.substr(start, found - start);
            start = found + 1;
        }

        missingEntries.push_back(snoEntry);
        snoEntry.clear();
    }

    return missingEntries;
}

// =======================================================================
// ------------------------- CREATING ENTRIES ----------------------------
// =======================================================================


const std::string GtfBuilder::createEntry(std::unordered_map<std::string,
                                          std::string>& missingEntry,
                                          const std::string& feature)
{
    std::ostringstream ss;
    ss << missingEntry["chr"].substr(3)                          << "\t"            // chr
       << "snoDB"                                                << "\t"            // source
       << feature                                                << "\t"            // feature
       << std::to_string(std::stoi(missingEntry["start"]) + 1)   << "\t"            // start +1 for gtf
       << missingEntry["end"]                                    << "\t"            // end
       << "."                                                    << "\t"            // score
       << missingEntry["strand"]                                 << "\t"            // strand
       << "."                                                    << "\t"            // frame
       << getEnsemblAttributes(missingEntry, feature)            << std::endl;      // attribues
    return ss.str();
}

const std::string GtfBuilder::getEnsemblAttributes(std::unordered_map<std::string,
                                                   std::string>& missingEntry,
                                                   const std::string& feature)
{
    std::string snotype = (missingEntry["box_type"] == "scaRNA") ? "scaRNA" : "snoRNA";

    std::ostringstream ss;
    // Attribues to all
    ss << "gene_id \"" << missingEntry["id"] << "\"; "               // gene_id
       << "gene_version \"1\"; ";                                    // gene_version

    if(missingEntry["name"] != "")
        ss << "gene_name \"" << missingEntry["name"] << "\"; ";      // gene_name

    ss << "gene_source \"snoDB\"; "                                  // gene_source
       << "gene_biotype \"" << snotype <<"\";";                      // gene_biotype

    if(feature == "transcript" or feature == "exon")
    {
        ss << " transcript_id \"" << missingEntry["id"] << "-201\"; "                 // transcript_id
           << "transcript_version \"1\"; ";                                           // transcript_version

        if(missingEntry["name"] != "")
            ss << "transcript_name \"" << missingEntry["name"] << "-201\"; ";         // transcript_name

        ss << "transcript_source \"snoDB\"; "                                         // transcript_source
           << "transcript_biotype \"" << snotype << "\"; "                            // transcript_biotype
           << "tag \"basic\"; "                                                       // tag
           << "transcript_support_level \"NA\";";                                     // transcript_support_level
    }

    if(feature == "exon")
    {
        ss << " exon_id \"" << missingEntry["id"] << "\"; "               // exon_id
           << "exon_number \"1\"; "                                       // exon_number
           << "exon_version \"1\";";                                      // exon_version
    }

    return ss.str();

    // Gene, transcript and exon examples
    // 21	snoDB	gene	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA";
    // 21	snoDB	transcript	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; transcript_id "snoDB2021-201"; transcript_version "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA"; transcript_name "U3-201"; transcript_source "snoDB"; transcript_biotype "snoRNA"; tag "basic"; transcript_support_level "NA";
    // 21	snoDB	exon	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; transcript_id "snoDB2021-201"; transcript_version "1"; exon_number "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA"; transcript_name "U3-201"; transcript_source "snoDB"; transcript_biotype "snoRNA"; exon_id "snoDB2021"; exon_version "1"; tag "basic"; transcript_support_level "NA";

}
