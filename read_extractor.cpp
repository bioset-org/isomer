
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "read_extractor.h"
#include "flat_kmers.h"
#include "file_parser.h"
#include <string>
#include <iostream>

void ReadExtractor::ExtractReads(std::string infile1, std::string infile2, std::string outfile1, std::string outfile2, FlatKMers::KMerMap *kmers)
{
	std::cout << "Reading FASTQ\n";
	FileParser::InFile R1;
	FileParser::InFile R2;
	FileParser::OutFile out1;
	FileParser::OutFile out2;
	R1.Open(infile1, 100000);
	if(infile2!="-")
		R2.Open(infile2, 100000);
	out1.Open(outfile1, 100000);
	out2.Open(outfile2, 100000);
	char line1[100000];
	char line2[100000];
	if (infile2 != "")
	{
		line2[0] = ' ';
		line2[1] = '\0';
	}
	char read1[100000];
	char read2[100000];
	char id1[100000];
	char id2[100000];
	int pos = 0;
	long long parsed_qt = 0;
	long long found_qt = 0;
	char* id_str = new char[10];
	while (R1.GetLine(line1))
	{
		if(infile2!="-")
			R2.GetLine(line2);
		if (pos == 0)
		{
			strcpy(id1, line1);
			strcpy(id2, line2);
			pos++;
			continue;
		}
		else if (pos == 1)
		{
			parsed_qt++;
			if (parsed_qt % 100000 == 0)
				std::cout << "Parsed " << parsed_qt << " reads, found " << found_qt << "\n";
			pos = 2;
			strcpy(read1, line1);
			strcpy(read2, line2);
			continue;
		}
		else if (pos == 2)
		{
			pos = 3;
			continue;
		}
		else if (pos == 3)
		{
			int res1 = check_read(read1, kmers);
			int res2 = check_read(read2, kmers);
			if (res1 || res2)
			{
				int id = (res1 != 0) ? res1 : res2;
				sprintf(id_str, "%d", id);
				std::cout << read1 << "\n";
				found_qt++;
				out1.SaveBlock(id1, strlen(id1));
				out1.SaveBlock(" ", 1);
				out1.SaveBlock(id_str, strlen(id_str));
				out1.SaveBlock("\n", 1);
				out1.SaveBlock(read1, strlen(read1));
				out1.SaveBlock("\n+\n", 3);
				out1.SaveBlock(line1, strlen(line1));
				out1.SaveBlock("\n", 1);
				out2.SaveBlock(id2, strlen(id2));
				out2.SaveBlock(" ", 1);
				out2.SaveBlock(id_str, strlen(id_str));
				out2.SaveBlock("\n", 1);
				out2.SaveBlock(read2, strlen(read2));
				out2.SaveBlock("\n+\n", 3);
				out2.SaveBlock(line2, strlen(line2));
				out2.SaveBlock("\n", 1);
			}
			pos = 0;
			continue;
		}
	}
	out1.Close();
	out2.Close();
}
int ReadExtractor::check_read(char* read, FlatKMers::KMerMap* kmers)
{
	int kmer_size = kmers->GetSequenceSize();
	int read_len = strlen(read);
	int len = read_len - kmer_size;
	bool found = false;
	char* read_nucs = conv.Nuc2Int(read, read_len, kmer_size);
	bool* ns = conv.GetSkipNs();
	for (int i = 0; i <= len; i++)
	{
		if (ns[i])
			continue;
		unsigned char* converted = conv.Convert(&read_nucs[i], kmer_size);
		int res = kmers->Find(converted);
		if (res<1000)
			return res;
	}
	return 0;
}