#ifndef GTF_PARSER_H
#define GTF_PARSER_H

#include <string>
#include <map>
#include <vector>

struct GeneEntry {
    std::string gene_id;
    std::string chr;
    std::vector<std::pair<int, int>> coordinates;
    char strand;
};

class GTFParser {
public:
    std::map<std::string, GeneEntry> ParseGenes(const std::string& filepath);
    std::map<std::string, GeneEntry> extractTranscriptCoordinates(const std::string& gtfFilePath);
    std::string extractTranscriptId(const std::string& attributes);
};

#endif // GTF_PARSER_H
