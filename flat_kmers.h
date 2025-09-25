#pragma once
#include "kmer_base.h"

namespace FlatKMers
{
	class KMerMap : public KMerBase
	{
	public:
		int nuc2int[4];
		bool Init(long long mem, int nuc_qt);
		int Add(unsigned char *key, int value);
		int Find(unsigned char *key);
		int AppendValue(unsigned char *key, int value, bool reset = false);
		unsigned char *InsertVirtual(unsigned char *kmer, int value);
		unsigned char* InsertBucket(unsigned char *bucket);
		long long GetKmerCount() { return kmer_qt; }
		void Clear();
		void FreeMemory();
		void RewriteSingle(bool val) { rewrite_single = val; }
		void SetAbsentValue(int v) { absent = v; }
		unsigned char *GetMap() { return map; }
		unsigned char *get_map_pointer() { return map; }
		bool SetValueByKMerNumber(int num, int val);
		int *GetStatistics();
		unsigned char *GetKMer(long long pos);
		int GetValueByKMerNumber(int num);
		const int *GetAllValues(unsigned char *kmer);
	private:
		void set_memory(long long mem);
	};
}