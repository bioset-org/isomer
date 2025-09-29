#pragma once
#include <string>
#include <vector>
#include "flat_kmers.h"
#include "nuc2bin.h"
#include "file_parser.h"
class KMerCreator
{
public:
	FlatKMers::KMerMap* CreateKMers(std::string gtf, std::string fna, std::string gene_name, std::string type);
private:
	void prepare_reference(std::string gtf, std::string fna, std::string gene_name, std::string type);
	std::vector<std::string> splitByComma(const std::string& input);
	void add_sequence(std::string seq, bool mode, int id);
	bool check_read(char* read);
	FlatKMers::KMerMap* kmers;
	int sequence_size = 30;
	Nuc2Bin conv;
	char* read;
	int seq_size=0;
};