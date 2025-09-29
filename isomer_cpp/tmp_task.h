#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <string>
#include <vector>
#include "read_genome.h"
#include "nuc2bin.h"

class TmpTask
{
public:
	void RunTask();
	void get_unique(std::vector<GenomeSequence> genes);
	void parse_sequence(char* str);
	void check_sequence(char* str, char *header);
	char* GetSequenceByNumber(int num, int size, char* seq);
	char* cur_kmer;
	void convert(char* sequence);
	long long kmer_size;
	char* nuc2int;
	bool* skip_n;
	int* kmer_counts;
	long long kmer_qt = 0;
	unsigned long long bit_seq;
	Nuc2Bin bin_converter;
};