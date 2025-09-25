#include "flat_kmers.h"
#include "phred.h"
#include <iostream>
#include <bitset>
bool FlatKMers::KMerMap::Init(long long mem, int nuc_qt)
{
	sequence_size = nuc_qt;
	//for (int i = 0;i < 256;i++)
	//	blocks[i] = 0;
	set_memory(mem);
	set_bytes(nuc_qt);
	//std::cout << preffix << " - " << offset << "\n";
	return true;
}
void FlatKMers::KMerMap::set_memory(long long mem)
{
	if (map_size != mem)
	{
		if (map_created)
			delete[] map;
		map_size = mem;
		map = new unsigned char[map_size];
		map_created = true;
	}
	Clear();
}
void FlatKMers::KMerMap::Clear()
{
	memset(map, 0, map_size * sizeof(unsigned char));
	kmer_qt = 0;
}
void FlatKMers::KMerMap::FreeMemory()
{
	if (map_created)
		delete[] map;
	map_size = 0;
	map_created = false;
}
int FlatKMers::KMerMap::Add(unsigned char *key, int value)
{
	unsigned char *pos = find_kmer(key);
	int res = 0;
	if (pos)
		memcpy(&res, &pos[number_position], sizeof(int));
	else
	{
		pos = last_bucket_position;
		add_kmer(key, pos);
	}
	set_kmer_value(pos, value + res);
	return res;
}
int FlatKMers::KMerMap::AppendValue(unsigned char *key, int value, bool reset)
{
	unsigned char *pos = find_kmer(key);
	if (!pos)
		return absent;
	int res = 0;
	memcpy(&res, &pos[number_position], sizeof(int));
	if (!reset)
		res += value;
	else
		res = value;
	set_kmer_value(pos, res);
	return res;
}
unsigned char *FlatKMers::KMerMap::InsertVirtual(unsigned char *kmer, int value)
{
	add_kmer(kmer, virtual_kmer);
	set_kmer_value(virtual_kmer, value);
	return virtual_kmer;
}
const int *FlatKMers::KMerMap::GetAllValues(unsigned char *kmer)
{
	if (!bit_mode)
		return (const int*) &kmer[flag_position + 1];
}
unsigned char* FlatKMers::KMerMap::InsertBucket(unsigned char *bucket)
{
	unsigned char *pos = find_kmer(bucket);
	int res = 0;
	if (!pos)
	{
		pos = last_bucket_position;
	}
	set_last_byte(bucket);
	memcpy(pos, bucket, kmer_size);
	restore_last_byte(bucket);
	kmer_qt++;
	return pos;
}
unsigned char *FlatKMers::KMerMap::GetKMer(long long pos)
{
	long long kmer_pos = pos*kmer_size;
	if (kmer_pos >= map_size)
		return NULL;
	if (kmer_pos < 0)
		return NULL;
	return &map[kmer_pos];
}
int FlatKMers::KMerMap::Find(unsigned char *key)
{
	unsigned char *pos = find_kmer(key);
	if (!pos)
		return absent;
	return ExtractValue(pos);
}
bool FlatKMers::KMerMap::SetValueByKMerNumber(int num, int val)
{
	unsigned char *pos = GetKMer(num);
	if (!pos)
		return false;
	memcpy(&pos[number_position], &val, sizeof(int));
	return true;
}
int FlatKMers::KMerMap::GetValueByKMerNumber(int num)
{
	unsigned char *pos = GetKMer(num);
	if (!pos)
		return false;
	return ExtractValue(pos);
}
int *FlatKMers::KMerMap::GetStatistics()
{
	static int res[256];
	int count;
	PhredConverter phred;
	for (int i = 0; i < 256; i++)
		res[i] = 0;
	for (long long i = 0; i < bucket_qt; i++)
	{
		memcpy(&count, &map[i*kmer_size + number_position], sizeof(int));
		if (!count)
			continue;
		res[phred.GetPhred(count)]++;
	}
	return res;
}