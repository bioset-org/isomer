#pragma once
#include "flat_kmers.h"
#include "nuc2bin.h"
#include <string>
class ReadExtractor
{
public:
	void ExtractReads(std::string infile1, std::string infile2, std::string outfile1, std::string outfile2, FlatKMers::KMerMap *kmers);
private:
	int check_read(char* read, FlatKMers::KMerMap* kmers);
	int kmer_size;
	Nuc2Bin conv;
};