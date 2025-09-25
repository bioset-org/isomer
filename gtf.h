#ifndef GTF_H
#define GTF_H

#include <string>
#include <fstream>
#include <unordered_map>

struct GTFIndex {
    int line_size = 0;
    int sep_size = 0;
    std::unordered_map<std::string, std::streampos> chrs;
};

class GTF {
public:
    static GTFIndex CreateIndex(const std::string& file);
    static void OpenFile(const std::string& file);
    static void CloseFile();
    static std::string ReadSequence(const GTFIndex& index, const std::string& chr, int pos, int len);
    static std::string MakeComplement(const std::string& sequence);

private:
    static std::ifstream file;
    static std::string trim(const std::string& s);
    static std::string to_upper(const std::string& s);
};

#endif // GTF_H
