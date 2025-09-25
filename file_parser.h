#pragma once
#include <string>
#include <fstream>
#include "bzip2/bzlib.h"
#include "zlib/zlib.h"

namespace FileParser
{
	std::string get_format(std::string file_name);
	class InFile
	{
	public:
		bool Open(std::string file_name, int size=0, std::string cur_format = "");
		void ReadBlock(char *str, long long *size);
		std::string GetError() { return error; }
		bool GetLine(char *str);
		bool HasError() { return has_error; }
		bool Close();
	private:
		bool has_error;
		void (FileParser::InFile::*get_block)(char*, long long*);
		void get_plain_block(char* str, long long* size);
		void get_bz2_block(char* str, long long* size);
		void get_gz_block(char* str, long long* size);
		std::ifstream t_file;
		long long gz_str_pos;
		FILE **a_file;
		BZFILE** archive;
		gzFile gz_file;
		bool bz_stream_end;
		int format;
		long long file_size;
		long long buffer_size;
		long long main_buffer_size;
		long long buffer_position;
		char *buffer;
		std::string file_name;
		std::string error;
		long long file_position;
	};
	class OutFile
	{
	public:
		bool Open(std::string file_name, long long size, std::string cur_format = "");
		bool SaveBlock(const char *str, int size);
		std::string GetError() { return error; }
		bool Close();
		bool HasError() { return has_error; }
	private:
		bool has_error;
		bool (OutFile::*save_block)(const char*, int);
		bool save_plain_block(const char *str, int size);
		bool save_bz2_block(const char *str, int size);
		bool save_gz_block(const char *str, int size);
		std::ofstream t_file;
		long long buffer_size;
		long long buffer_position;
		char *buffer;
		FILE **a_file;
		BZFILE** archive;
		z_stream *gz_stream;
		unsigned char *gz_str;
		FileParser::OutFile *main_gz_file;
		int gz_flush_mode;
		std::string file_name;
		std::string error;
		long long file_position;
		int format;
	};
}