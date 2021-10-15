// gtf_builder.cpp
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include "gtf_builder.h"

void GtfBuilder::init()
{
    buildNewEntries();
}

void GtfBuilder::buildNewEntries()
{
    std::cout << "Building new entries for " << this->source << "..." << std::endl;

    if(this->source == "RefSeq")
        getChrCorresp(); // Get chr correspondance

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

    writeNewGtf(builtEntries);

    std::cout << "Building " << this->source << " done !" << std::endl;
}


std::vector< std::unordered_map<std::string, std::string> > GtfBuilder::fileReader()
{
    // Declare the variables
    std::vector< std::unordered_map<std::string, std::string> > missingEntries;
    std::vector<std::string> fieldNames = { "chr", "start", "end", "id", "name", "strand", "box_type"};
    std::unordered_map<std::string, std::string> snoEntry;

    // Initialize file stream
    std::ifstream fileStream(this->missingSnoRNAFile);
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

void GtfBuilder::writeNewGtf(std::vector<std::string> builtEntries)
{
    int found = this->gtfFile.find(".gtf");
    std::string baseFileName = this->gtfFile.substr(0, found);
    std::string NewGtfFile = baseFileName + this->suffix + ".gtf";

    // Output stream
    std::ofstream outfile(NewGtfFile);

    // Read from the original file
    std::ifstream fileStream(this->gtfFile);
    std::string line, field;

    while(getline(fileStream, line))
    {
        if(line.find("###") == std::string::npos)
        {
            outfile << line << std::endl;
        }
    }

    for(const std::string& line: builtEntries)
    {
        outfile << line;
    }
}

// =======================================================================
// ------------------------- CREATING ENTRIES ----------------------------
// =======================================================================


const std::string GtfBuilder::createEntry(std::unordered_map<std::string,
                                          std::string>& missingEntry,
                                          const std::string& feature)
{
    std::ostringstream ss;
    ss << getChr(missingEntry["chr"])                            << "\t"            // chr
       << "snoDB"                                                << "\t"            // source
       << feature                                                << "\t"            // feature
       << std::to_string(std::stoi(missingEntry["start"]) + 1)   << "\t"            // start +1 for gtf
       << missingEntry["end"]                                    << "\t"            // end
       << "."                                                    << "\t"            // score
       << missingEntry["strand"]                                 << "\t"            // strand
       << "."                                                    << "\t"            // frame
       << getAttributes(missingEntry, feature)                   << std::endl;      // attribues
    return ss.str();
}

const std::string GtfBuilder::getChr(const std::string& field)
{
    if(this->source == "Ensembl")
        return field.substr(3);
    else
        return getRefseqChr(field);
}

const std::string GtfBuilder::getAttributes(std::unordered_map<std::string, std::string>& missingEntry,
                                            const std::string& feature)
{
    if(this->source == "Ensembl")
        return getEnsemblAttributes(missingEntry, feature);
    else
        return getRefseqAttributes(missingEntry, feature);
}



// -------------------------------------------------------------
// ------------------------ FOR ENSEMBL ------------------------
// -------------------------------------------------------------


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

    // Ensembl gene, transcript and exon examples
    // 21	snoDB	gene	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA";
    // 21	snoDB	transcript	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; transcript_id "snoDB2021-201"; transcript_version "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA"; transcript_name "U3-201"; transcript_source "snoDB"; transcript_biotype "snoRNA"; tag "basic"; transcript_support_level "NA";
    // 21	snoDB	exon	40761744	40761818	.	+	.	gene_id "snoDB2021"; gene_version "1"; transcript_id "snoDB2021-201"; transcript_version "1"; exon_number "1"; gene_name "U3"; gene_source "snoDB"; gene_biotype "snoRNA"; transcript_name "U3-201"; transcript_source "snoDB"; transcript_biotype "snoRNA"; exon_id "snoDB2021"; exon_version "1"; tag "basic"; transcript_support_level "NA";

}


// -------------------------------------------------------------
// ------------------------ FOR REFSEQ ------------------------
// -------------------------------------------------------------

const std::string GtfBuilder::getRefseqChr(const std::string& field) {
    return this->chrCorresp[field];
}

std::unordered_map<std::string, std::string> GtfBuilder::getChrCorresp()
{
    // Initialize file stream
    std::ifstream fileStream(this->gtfFile);
    std::string line, field, chr;

    // initiate the position in line when searching
    std::size_t start;
    std::size_t found;

    // go through the file line by line
    while(getline(fileStream, line))
    {
        if(line[0] == '#') continue;

        start = 0;
        found = 0;

        found = line.find("\t", start);
        field = line.substr(start, found - start);

        // Check if regular chr
        start = field.find("NC_");
        if(start != std::string::npos)
        {
            start += 3;
            found = field.find(".");
            chr = "chr" + std::to_string(std::stoi(field.substr(start, found - start)));

            // Deal with sex chromosome
            if(chr == "chr23") chr = "chrY";
            else if(chr == "chr24") chr = "chrX";

            this->chrCorresp[chr] = field;
        }
    }

    // Make sure that is is really RefSeq
    if(chrCorresp.empty())
    {
        std::cerr << "\e[31mProblems to find the RefSeq chromosomes..."
                  << " Maybe the 'source' in the config.ini file should"
                  << " be Ensembl ?\n\e[39m";
        exit(1);
    }

    return chrCorresp;
}

const std::string GtfBuilder::getRefseqAttributes(std::unordered_map<std::string,
                                                   std::string>& missingEntry,
                                                   const std::string& feature)
{
    std::string snotype = (missingEntry["box_type"] == "scaRNA") ? "scaRNA" : "snoRNA";

    std::ostringstream ss;
    // Attribues to all
    ss << "gene_id \"" << missingEntry["id"] << "\"; ";               // gene_id

    if(feature == "gene")
        ss  << "transcript_id \"\"; ";                                // transcript_id
    else
        ss << "transcript_id \"" << missingEntry["id"] << ".1\"; ";   // transcript_id

    if(feature == "gene") ss << "gbkey \"Gene\"; ";                   // gbkey
    else if(feature == "transcript") ss << "gbkey \"ncRNA\"; ";       // gbkey

    if(missingEntry["name"] != "")
        ss << "gene \"" << missingEntry["name"] << "\"; ";            // gene

    if(feature == "gene")
        ss << "gene_biotype \"" << snotype <<"\";";                   // gene_biotype
    else
        ss << "transcript_biotype \"" << snotype <<"\";";             // transcript_biotype


    if(feature == "exon")
        ss << " exon_number \"1\";";

    return ss.str();

    // Refseq gene, transcript and exon examples
    // NC_000001.11	BestRefSeq	gene	    1304729	1304812	.	-	.	gene_id "SNORD167"; transcript_id ""; db_xref "GeneID:109623456"; db_xref "HGNC:HGNC:51881"; description "small nucleolar RNA, C/D box 167"; gbkey "Gene"; gene "SNORD167"; gene_biotype "snoRNA";
    // NC_000001.11	BestRefSeq	transcript	1304729	1304812	.	-	.	gene_id "SNORD167"; transcript_id "NR_145806.1"; db_xref "GeneID:109623456"; gbkey "ncRNA"; gene "SNORD167"; product "small nucleolar RNA, C/D box 167"; transcript_biotype "snoRNA";
    // NC_000001.11	BestRefSeq	exon	    1304729	1304812	.	-	.	gene_id "SNORD167"; transcript_id "NR_145806.1"; db_xref "GeneID:109623456"; gene "SNORD167"; product "small nucleolar RNA, C/D box 167"; transcript_biotype "snoRNA"; exon_number "1";

}
