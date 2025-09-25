#pragma once
#include <inttypes.h>
class Nuc2Bin
{
public:
	Nuc2Bin();
	unsigned char* Convert(char *seq, int dna_len);
	char *ExtractNucs(unsigned char *kmer, int len, char *out=0);
	char *Nuc2Int(char *read, int read_len, int sequence_size, char *vals =0, bool *skips=0);
	bool *GetSkipNs() { return skip_n; }
	char Phred2Count(int qt);
	char GetPhred(int qt);
	int CalculateByteLen(int len);
private:
	unsigned char get_bit(unsigned char num, int k, int p);
	int default_len = 0;
	char *int2nuc;
	char *nuc2int;
	bool *skip_n;
	unsigned char *bit_seq;
	int nucs_arr[128];
	unsigned char nuc_bits[128];
};