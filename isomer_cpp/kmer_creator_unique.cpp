
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "kmer_creator.h"
#include "gtf.h"
#include "gtf_parser.h"
#include "flat_kmers.h"
#include "nuc2bin.h"
#include <string>
#include <iostream>

FlatKMers::KMerMap* KMerCreator::CreateKMers(std::string gtf, std::string fna, std::string gene_name, std::string type)
{
    kmers = new FlatKMers::KMerMap;
    kmers->Init(100000000, 30);
    prepare_reference(gtf, fna, gene_name, type);
    return kmers;
}
void KMerCreator::prepare_reference(std::string gtf, std::string fna, std::string gene_name, std::string type)
{
    std::cout << "Creating index\n";
    auto index = GTF::CreateIndex(fna);
    GTFParser gtf_parser;

    GTF::OpenFile(fna);



    std::cout << "Parsing GTF\n";
    std::map<std::string, GeneEntry> genes;
    if (type == "gene")
        genes = gtf_parser.ParseGenes(gtf);
    else
        genes = gtf_parser.extractTranscriptCoordinates(gtf);
    std::string seq;
    int added_qt = 0;
    // First: process the selected gene
    for (auto gene : genes)
    {
        std::string full_seq;
        std::cout << gene.first << "\n";
        for (const auto& pair : gene.second.coordinates) {
            const int start = pair.first;
            const int end = pair.second;
            std::string exon_seq = GTF::ReadSequence(index, gene.second.chr, start, (end - start) + 1);
            //std::cout << exon_seq << "\n";
            full_seq += exon_seq;
        }
        if (gene.second.strand == '+')
            full_seq = GTF::MakeComplement(full_seq);
        add_sequence(full_seq, 1);
        added_qt++;
        if (kmers->LoadFactor() > 0.75)
        {
            std::cout << "Full, added " << added_qt << " genes\n";
            int total = 0;
            int unique = 0;
            long long b_qt = kmers->GetBucketCount();
            for (int k = 0; k < b_qt; ++k) {
                unsigned char* key = kmers->GetKMer(k);
                int qt = kmers->ExtractValue(key);
                if (!qt)
                    continue;
                total++;
                char* kmer = conv.ExtractNucs(key, sequence_size);
                //std::cout << kmer << " - " << qt << "\n";
                if (qt == 1)
                    unique++;
                std::cout << total << " - " << unique << "\n";
            }
            std::cin.get();
        }
    }
    auto it = genes.find(gene_name);
    if (it != genes.end()) {
        const GeneEntry& gene = it->second;
        std::cout << "Gene: " << gene_name
            << ", Strand: " << gene.strand << "\n";

        std::string full_seq;
        for (const auto& pair : gene.coordinates) {
            const int start = pair.first;
            const int end = pair.second;
            std::string exon_seq = GTF::ReadSequence(index, gene.chr, start, (end - start) + 1);
            full_seq += exon_seq;
        }

        if (gene.strand == '+')
            full_seq = GTF::MakeComplement(full_seq);
        seq = full_seq;

        std::cout << seq << "\n";
        add_sequence(seq, 1);

        //seq = GTF::MakeComplement(seq);
        //add_sequence(seq, 1);
    }

    // Then: process all *other* genes
    for (const auto& pair : genes) {
        const std::string& gene_id = pair.first;
        const GeneEntry& gene = pair.second;
        if (gene_id == gene_name)
            continue;

        std::string full_seq;
        for (const auto& pair : gene.coordinates) {
            const int start = pair.first;
            const int end = pair.second;
            std::string exon_seq = GTF::ReadSequence(index, gene.chr, start, (end - start) + 1);
            full_seq += exon_seq;
        }


        seq = full_seq;
        if (gene.strand == '+')
            seq = GTF::MakeComplement(seq);
        add_sequence(seq, 0);
    }

    // Final step: output kmers
    long long b_qt = kmers->GetBucketCount();
    for (int k = 0; k < b_qt; ++k) {
        unsigned char* key = kmers->GetKMer(k);
        int qt = kmers->ExtractValue(key);
        if (!qt)
            continue;

        char* kmer = conv.ExtractNucs(key, sequence_size);
        //std::cout << kmer << " - " << qt << "\n";
        if (qt == 1)
            std::cout << kmer << " - " << qt << "\n";
    }

}
void KMerCreator::add_sequence(std::string s_seq, bool mode)
{
    const char* c_read = s_seq.c_str();
    if (seq_size < strlen(c_read))
    {
        seq_size = strlen(c_read) + 1;
        read = new char[strlen(c_read) + 1];
    }
    strcpy(read, c_read);
    int read_len = strlen(read);
    int len = read_len - sequence_size;
    bool found = false;
    char* read_nucs = conv.Nuc2Int(read, read_len, sequence_size);
    bool* ns = conv.GetSkipNs();
    for (int i = 0; i <= len; i++)
    {
        if (ns[i])
            continue;
        unsigned char* converted = conv.Convert(&read_nucs[i], sequence_size);
        //std::cout << "add " << conv.ExtractNucs(converted, sequence_size) << "\n";
        if (mode)
            kmers->Add(converted, 1);
        else
            kmers->AppendValue(converted, 1);
    }
}