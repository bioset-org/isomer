// TempCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <iostream>
#include <vector>
#include "gtf.h"
#include "kmer_creator.h"
#include "flat_kmers.h"
#include "read_extractor.h"

int main(int argc, char **argv)
{
	KMerCreator kmer_creator;
	FlatKMers::KMerMap* kmers = NULL;
	std::cout << "save to " << argv[6] << "\n";
	kmers = kmer_creator.CreateKMers(argv[1], argv[2], argv[3], "transcript");
	ReadExtractor extractor;
	extractor.ExtractReads(argv[4], argv[5], argv[6], argv[7], kmers);

	return 0;
}

