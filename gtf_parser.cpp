#include "gtf_parser.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <iostream>

std::map<std::string, GeneEntry> GTFParser::extractTranscriptCoordinates(const std::string& gtfFilePath) {
    std::ifstream file(gtfFilePath);
    std::map<std::string, GeneEntry> transcripts;

    if (!file.is_open()) {
        std::cerr << "Failed to open GTF file: " << gtfFilePath << std::endl;
        return transcripts;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string chrom, source, feature, startStr, endStr, score, strandStr, frame, attributes;
        std::getline(ss, chrom, '\t');
        std::getline(ss, source, '\t');
        std::getline(ss, feature, '\t');
        std::getline(ss, startStr, '\t');
        std::getline(ss, endStr, '\t');
        std::getline(ss, score, '\t');
        std::getline(ss, strandStr, '\t');
        std::getline(ss, frame, '\t');
        std::getline(ss, attributes);

        if (feature != "exon") continue;

        std::string transcriptId = extractTranscriptId(attributes);
        if (transcriptId.empty()) continue;

        int start = std::stoi(startStr);
        int end = std::stoi(endStr);
        char strand = strandStr.empty() ? '.' : strandStr[0];

        GeneEntry& info = transcripts[transcriptId];
        info.chr = chrom;
        info.strand = strand;
        info.coordinates.emplace_back(start, end);
    }

    file.close();
    return transcripts;
}
std::string GTFParser::extractTranscriptId(const std::string& attributes) {
    std::string key = "transcript_id \"";
    auto start = attributes.find(key);
    if (start == std::string::npos) return "";

    start += key.size();
    auto end = attributes.find('"', start);
    if (end == std::string::npos) return "";

    return attributes.substr(start, end - start);
}
std::map<std::string, GeneEntry>  GTFParser::ParseGenes(const std::string& filepath) {
    std::map<std::string, GeneEntry> genes;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open GTF file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string chrom, source, feature, t;
        int start, end;
        std::string score, strand, frame, attributes;

        // GTF fields are tab-separated
        std::getline(iss, chrom, '\t');
        std::getline(iss, source, '\t');
        std::getline(iss, feature, '\t');
        iss >> start >> end >> t >> strand;
        std::getline(iss, frame, '\t');
        std::getline(iss, attributes);

        if (feature != "gene") continue;

        // Parse gene_id from attributes
// Parse gene_id from attributes
        std::smatch match;
        std::regex gene_id_regex("gene_id\\s+\"([^\"]+)\"");

        if (std::regex_search(attributes, match, gene_id_regex)) {
            GeneEntry& entry = genes[match[1]];
            entry.gene_id = match[1];
            entry.chr = chrom;
            entry.coordinates.emplace_back(start, end);
            entry.strand = strand.empty() ? '.' : strand[0];
        }

    }

    return genes;
}
