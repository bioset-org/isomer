#include "nuc2bin.h"
#include <inttypes.h>
#include <iostream>
#include <bitset>

Nuc2Bin::Nuc2Bin()
{
	nucs_arr['A'] = 0;
	nucs_arr['C'] = 1;
	nucs_arr['G'] = 2;
	nucs_arr['T'] = 3;
	nucs_arr['N'] = 4;
	nuc_bits[0] = 'A';
	nuc_bits[1] = 'C';
	nuc_bits[2] = 'G';
	nuc_bits[3] = 'T';
}
unsigned char* Nuc2Bin::Convert(char *seq, int dna_len)
{
	memset(bit_seq, 0, dna_len / 4 + 1);

	/* for each base of the DNA sequence */
	for (size_t i = 0; i < dna_len; ++i)
	{
		char l = seq[i];
		uint8_t shift = 6 - 2 * (i % 4);
		bit_seq[i / 4] |= seq[i] << shift;
		unsigned char l2 = bit_seq[i / 4];

		shift = (shift == 0) ? 6 : shift - 2;
	}
	return bit_seq;
}
char *Nuc2Bin::Nuc2Int(char *read, int read_len, int sequence_size, char *vals, bool *skip)
{
	if (strlen(read) >= default_len)
	{
		default_len = strlen(read) + 1;
		int2nuc = new char[default_len];
		nuc2int = new char[default_len];
		skip_n = new bool[default_len];
		bit_seq = new unsigned char[default_len];
	}
	char *ints;
	bool *skips;
	ints = (vals) ? vals : nuc2int;
	skips = (skip) ? skip : skip_n;
	int nuc_pos = -1;
	int shift = 0;
	memset(skips, 0, sizeof(bool)*read_len);
	for (int i = 0; i < read_len; i++)
	{
		ints[i] = nucs_arr[read[i]];
		//if (!values_added)
		//{
		//skip_n[thread][i] = false;
		if (ints[i] > 3 || ints[i] < 0)
		{
			for (int j = i - sequence_size; j <= i; j++)
			{
				if (j < 0)
					j = 0;
				skips[j] = true;
			}
			ints[i] = 0;
		}
		//}
	}
	return ints;
}
unsigned char Nuc2Bin::get_bit(unsigned char number, int pos, int qt)
{
	return (((1 << pos) - 1) & (number >> (qt - 1)));
}
char *Nuc2Bin::ExtractNucs(unsigned char *kmer, int len, char *out)
{
	memset(int2nuc, '\0', len + 1);
	int pos = 0;
	int long_block_qt = len / 4 + ((len % 4) ? 1 : 0);;
	for (int i = 0; i < long_block_qt; i++)
	{
		for (int j = 7; j > 0; j -= 2)
		{
			int nuc_num = get_bit(kmer[i], 2, j);
			int2nuc[pos] = nuc_bits[nuc_num];
			pos++;
		}
	}
	int2nuc[len] = '\0';
	return int2nuc;
}
int Nuc2Bin::CalculateByteLen(int len)
{
	int size = len / 4;
	if (len % 4 != 0)
		size++;
	return size;
}
char Nuc2Bin::GetPhred(int qt)
{
	if (qt < 1)
		return 33;
	if (qt < 10)
		return 33 + qt;
	if (qt < 100)
		return 42 + qt / 10;
	if (qt < 1000)
		return 51 + qt / 100;
	if (qt < 10000)
		return 60 + qt / 1000;
	if (qt < 100000)
		return 69 + qt / 10000;
	if (qt < 1000000)
		return 76 + qt / 100000;
	if (qt < 10000000)
		return 85 + qt / 1000000;
	if (qt < 100000000)
		return 94 + qt / 10000000;
	if (qt < 1000000000)
		return 103 + qt / 100000000;
	return 112;
}
char Nuc2Bin::Phred2Count(int qt)
{
	if (qt < 10)
		return 33 + qt;
	if (qt < 100)
		return 42 + qt / 10;
	if (qt < 1000)
		return 51 + qt / 100;
	if (qt < 10000)
		return 60 + qt / 1000;
	if (qt < 100000)
		return 69 + qt / 10000;
	if (qt < 1000000)
		return 76 + qt / 100000;
	if (qt < 10000000)
		return 85 + qt / 1000000;
	if (qt < 100000000)
		return 94 + qt / 10000000;
	if (qt < 1000000000)
		return 103 + qt / 100000000;
	return 112;
}