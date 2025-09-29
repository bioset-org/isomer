#include "gtf.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// Define the static file handle
std::ifstream GTF::file;

GTFIndex GTF::CreateIndex(const std::string& filePath) {
    GTFIndex out;
    std::ifstream f(filePath, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::string line;
    int line_size = 0, sep_size = 0;

    while (std::getline(f, line)) {
        if (!line.empty() && line[0] == '>') {
            std::istringstream iss(line);
            std::string chrname;
            iss >> chrname;
            chrname.erase(0, 1); // remove '>'
            out.chrs[chrname] = f.tellg();
        }
        else {
            if (line_size == 0) {
                std::string trimmed = trim(line);
                line_size = static_cast<int>(trimmed.length());
                sep_size = static_cast<int>(line.length() - trimmed.length());
            }
        }
    }

    out.line_size = line_size;
    out.sep_size = 1;// sep_size;

    return out;
}

void GTF::OpenFile(const std::string& filePath) {
    if (file.is_open()) file.close();
    file.open(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
}

void GTF::CloseFile() {
    if (file.is_open()) {
        file.close();
    }
}

std::string GTF::ReadSequence(const GTFIndex& index, const std::string& chr, int pos, int len) {
    len++;
    if (!file.is_open()) {
        throw std::runtime_error("File not opened. Call OpenFile() first.");
    }

    auto it = index.chrs.find(chr);
    if (it == index.chrs.end()) {
        throw std::runtime_error("Chromosome not found: " + chr);
    }

    std::streampos start_pos = it->second;
    int lines_before = (pos - 1) / index.line_size;
    int position_in_line = (pos - 1) % index.line_size;

    std::streampos file_pos = start_pos + static_cast<std::streampos>(
        lines_before * (index.line_size + index.sep_size) + position_in_line);

    file.clear(); // Clear EOF
    file.seekg(file_pos);

    std::string out, line;
    while (static_cast<int>(out.length()) < len && std::getline(file, line)) {
        out += trim(line);
    }

    return to_upper(out.substr(0, len));
}

std::string GTF::trim(const std::string& s) {
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

std::string GTF::to_upper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}
std::string GTF::MakeComplement(const std::string& sequence) {
    std::string result;
    result.reserve(sequence.size());

    for (auto it = sequence.rbegin(); it != sequence.rend(); ++it) {
        char c = std::toupper(*it);
        switch (c) {
        case 'A': result += 'T'; break;
        case 'T': result += 'A'; break;
        case 'C': result += 'G'; break;
        case 'G': result += 'C'; break;
        case 'N': result += 'N'; break;
        default:  result += 'N'; break; // handle unknown bases
        }
    }

    return result;
}
