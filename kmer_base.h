#pragma once
#include <mutex>

namespace FlatKMers
{
	class KMers
	{
		char *kmer;
	};
	class Bucket
	{
	public:
	private:
		unsigned char *block;
	};
	class KMerBase
	{
	public:
		KMerBase();
		long long GetBucketCount() { return bucket_qt; }
		int GetBucketSize() { return kmer_size; }
		int GetKeySize() { return key_size; }
		long long GetMapSize() { return map_size; }
		int GetValuePosition() { return number_position; }
		bool SetSampleCount(int qt, int cur_sample=-1);
		long long GetMemoryRequirement(long long kmer_qt) { return kmer_qt * kmer_size * 1.5; }
		bool SetSample(int pos);
		int GetSequenceSize() { return sequence_size; }
		int GetFlagPosition() { return flag_position; }
		int ExtractValue(unsigned char *kmer);
		bool LoadFactor();
		void SetValueMode(int mode);
		void SetBitMode(bool mode);
		void SetCharMode(bool mode);
	protected:
		void set_bytes(int sequence_size);
		long long MurmurHash64A(const void * key, int len, unsigned int seed);
		long long get_bucket_pos(unsigned char *key);
		long long get_num_by_preffix(unsigned char *key);
		void set_last_byte(unsigned char *key);
		void restore_last_byte(unsigned char *key);
		unsigned char *find_kmer(unsigned char *kmer);
		void set_kmer_value(unsigned char *kmer, int value);
		void add_kmer(unsigned char *kmer, unsigned char *pos);
		int calculate_kmer_size();
		void calculate_number_position();
		int flag_position;
		unsigned char *last_bucket_position;
		int sample;
		void set_offsets();
		unsigned char *map;
		int sample_count;
		int block_size;
		int compare_bits;
		unsigned char last_byte;
		int last_byte_pos;
		char mask_byte;
		bool map_created;
		int realloc_container_size;
		bool rewrite_single;
		long long offsets[256];
		long long buckets[256];
		std::mutex mtx;
		int absent;
		long long map_size;
		uint64_t key_size;
		int kmer_size;
		uint64_t bucket_qt;
		uint64_t kmer_qt;
		int number_position;
		int kmer_counts[256];
		int sequence_size;
		unsigned char virtual_kmer[1000];
		uint64_t hash_block_size;
		bool bit_mode;
		bool int_mode;
		bool char_mode;
		int bit_pos;
		int bit_byte_qt;
		int byte_position;
		bool loaded;
		long long max_kmer_count;
		int *values_list;
	};
}