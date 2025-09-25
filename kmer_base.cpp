#include "kmer_base.h"
#include "nuc2bin.h"
#include "phred.h"
#include "MurmurHash3.h"
#include <iostream>
#include <map>
#include <bitset>
#include <mutex>

int blocks[256];
int cmp_qt;

FlatKMers::KMerBase::KMerBase()
{
	map_size = 0;
	map_created = false;
	sample_count = 1;
	sample = 0;
	memset(virtual_kmer, 0, 1000);
	kmer_size = 0;
	bit_mode = false;
	values_list = NULL;
}
void FlatKMers::KMerBase::SetBitMode(bool mode)
{
	if (mode)
		SetValueMode(2);
	else
		SetValueMode(0);
}
void FlatKMers::KMerBase::SetCharMode(bool mode)
{
	if (mode)
		SetValueMode(1);
	else
		SetValueMode(0);
}
void FlatKMers::KMerBase::SetValueMode(int mode)
{
	bit_mode = false;
	int_mode = false;
	char_mode = false;
	if (mode == 1)
		char_mode = true;
	else if (mode == 2)
		bit_mode = true;
	else
		int_mode = true;
	SetSampleCount(sample_count);
}
bool FlatKMers::KMerBase::LoadFactor()
{
	return (kmer_qt >= max_kmer_count) ? true : false;
}
int FlatKMers::KMerBase::ExtractValue(unsigned char *kmer)
{
	int res = 0;
	if (!bit_mode)
		memcpy(&res, &kmer[number_position], sizeof(int));
	else
		res = (kmer[byte_position] >> bit_pos) & 1;
	return res;
}
void FlatKMers::KMerBase::set_bytes(int nuc_qt)
{
	cmp_qt = 0;
	key_size = nuc_qt / 4;
	compare_bits = (nuc_qt % 4) * 2;
	mask_byte = 0;
	if (compare_bits == 2)
		mask_byte = 63;
	else if (compare_bits == 4)
		mask_byte = 15;
	else if (compare_bits == 6)
		mask_byte = 3;
	/*mask_byte = 0;
	for(int i=0;i<compare_bits;i++)
	mask_byte*/
	if (compare_bits)
		key_size++;
	last_byte_pos = key_size - 1;
	flag_position = key_size;
	absent = 0;
	SetSampleCount(sample_count);
}
unsigned char *FlatKMers::KMerBase::find_kmer(unsigned char *key)
{
	set_last_byte(key);
	//int cur_qt = 0;
	long long bucket_pos = get_bucket_pos(key);
	//std::cout << bucket_pos << "\n";
	//unsigned char *t = &map[bucket_pos];;
	char res = map[bucket_pos + flag_position];
	kmer_qt;
	bucket_qt;
	int qt = 0;
	while (res)
	{
		qt++;
		//cur_qt++;
		if (!memcmp(&map[bucket_pos], key, key_size))
		{
			//std::cout << "found at " << i << "\n";
			set_last_byte(key);
			return &map[bucket_pos];
		}
		bucket_pos += kmer_size;//*pow(num, 2);
		if (bucket_pos + kmer_size > map_size)
		{
			bucket_pos = key[0] * kmer_size;
		}
		res = map[bucket_pos + flag_position];
		//if (num > 10)
		//	num = 0;
		//if (cur_qt > cmp_qt)
		//{
		//	std::cout << kmer_qt << " - " << cur_qt << "\n";
		//	cmp_qt = cur_qt;
		//}
	}
	restore_last_byte(key);
	last_bucket_position = &map[bucket_pos];
	return NULL;
}
void FlatKMers::KMerBase::set_kmer_value(unsigned char *kmer, int value)
{
	//my_byte = my_byte | (1 << pos);
	if (!bit_mode)
		memcpy(&kmer[number_position], &value, sizeof(int));
	else
	{
		kmer[byte_position] |= 1 << bit_pos;
	}
}
void FlatKMers::KMerBase::add_kmer(unsigned char *key, unsigned char *pos)
{
	set_last_byte(key);
	memcpy(pos, key, key_size);
	kmer_qt++;
	pos[flag_position] = '\1';
	restore_last_byte(key);
}
bool FlatKMers::KMerBase::SetSampleCount(int qt, int cur_sample)
{
	if (qt <= 0)
	{
		std::cout << "Wrong sample count - " << qt << "\n";
		return false;
	}
	sample_count = qt;
	kmer_size = calculate_kmer_size();

	bucket_qt = map_size / kmer_size;
	block_size = bucket_qt / 4;
	max_kmer_count = bucket_qt * 0.68;
	hash_block_size = (uint64_t) 18446744073709551615 / (uint64_t) bucket_qt;
	if (cur_sample == -1)
		cur_sample = sample;
	SetSample(cur_sample);
	if (values_list)
		delete[] values_list;
	values_list = new int[sample_count];
}
bool FlatKMers::KMerBase::SetSample(int pos)
{
	if (pos < 0 || pos >= sample_count)
	{
		std::cout << "Wrong sample - " << pos << ". Sample count - " << sample_count << "\n";
		return false;
	}
	sample = pos;
	calculate_number_position();
}
int FlatKMers::KMerBase::calculate_kmer_size()
{
	int size;
	if (!bit_mode)
		size = key_size + 1 + sizeof(int)*sample_count;
	else
	{
		bit_byte_qt = sample_count / 8;
		if (sample_count % 8 != 0)
			bit_byte_qt++;
		size = key_size + 1 + bit_byte_qt;
	}
	return size;
}
void FlatKMers::KMerBase::calculate_number_position()
{
	if (!bit_mode)
		number_position = key_size + 1 + sizeof(int)*sample;
	else
	{
		byte_position = key_size + 1 + sample / 8;
		bit_pos = sample % 8;
		bit_pos = 7 - bit_pos;
	}
}
void FlatKMers::KMerBase::set_last_byte(unsigned char *key)
{
	/*std::bitset<8>n1(mask_byte);
	std::cout << n1 << "\n";
	std::bitset<8>n2(key[last_byte_pos]);
	std::cout << n2 << "\n";*/
	last_byte = key[last_byte_pos];
	key[last_byte_pos] &= ~mask_byte;
	//std::bitset<8>n3(key[last_byte_pos]);
	//std::cout << n3 << "\n";
}
void FlatKMers::KMerBase::restore_last_byte(unsigned char *key)
{
	key[last_byte_pos] = last_byte;
}
void FlatKMers::KMerBase::set_offsets()
{
	float conc[256];
	for (int i = 0; i < 256; i++)
	{
		conc[i] = (float)blocks[i] / (float)1000000;
	}
	long long pos = 0;
	for (int i = 0; i < 256; i++)
	{
		buckets[i] = conc[i] * bucket_qt;
		offsets[i] = pos;
		pos += conc[i] * bucket_qt;
	}
	std::cout << "OK";
}
long long FlatKMers::KMerBase::get_bucket_pos(unsigned char *key)
{
	//unsigned int val = 0;
	//memcpy(&val, key, 4);
	//val &= 0x00ffffff;
	//std::cout << val << "\n";
	//return val * kmer_size;
	//size_t bucket_pos = MurmurHash64A(key, key_size, key[0]);
	uint64_t bucket_pos[2];
	MurmurHash3_x64_128(key, key_size, key[0], &bucket_pos);
	////int offset = 0;
	////offset |= (key[0] >> 6);
	////int x = offset * 25000000;
	////int y = bucket_pos % 25000000;
	//long long bucket = hash/ hash_block_size;
	//bucket_qt;
	long long pos = (bucket_pos[0]%bucket_qt)*kmer_size;
	return pos;
	int key_pos = 0;
	//long long bucket;
	//unsigned int val = 0;
	//val |= (key[0] << 18) | (key[1] << 10) | (key[2] << 8) | (key[3] >> 6);
	//memcpy(&val, key, 4);
	//val &= 0x000fffff;
	////return val*key_size;
	//		std::bitset<32> x(val);
	//		std::cout << x << " 3\n";
	//		std::bitset<8> z(key[0]);
	//		std::cout << z << "\n";
	//		std::bitset<8> q(key[1]);
	//		std::cout << q << "\n";
	//		std::bitset<8> w(key[2]);
	//		std::cout << w << "\n";
	//		std::bitset<8> e(key[3]);
	//		std::cout << e << "\n";
	//		std::cout << val << "\n";
	//		std::cin.get();
	//while (1)
	//{
	//	memcpy(&val, &key[key_pos], (key_pos >= key_size - sizeof(long long)) ? sizeof(long long) : key_size - sizeof(long long));
	//	bucket = val%bucket_qt;
	//	key_pos += sizeof(long long);
	//	//if (key_pos >= key_size)
	//		break;
	//}
	//return bucket*key_size;
}
long long FlatKMers::KMerBase::get_num_by_preffix(unsigned char *key)
{
	long long id = 0;
	return id;
}
long long FlatKMers::KMerBase::MurmurHash64A(const void * key, int len, unsigned int seed)
{
	const long long m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len / 8);

	while (data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch (len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}