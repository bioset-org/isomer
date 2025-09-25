#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include "file_parser.h"
#include "zlib\zlib.h"
#include <iostream>

using namespace FileParser;

std::string FileParser::get_format(std::string file_name)
{
	int pos = file_name.find_last_of(".");
	std::string ext = file_name.substr(pos + 1);
	if (!_stricmp(ext.c_str(), "gz") || !_stricmp(ext.c_str(), "tgz"))
		return "gz";
	else if (!_stricmp(ext.c_str(), "bz2"))
		return "bz2";
	else
		return "plain";
}
bool InFile::Open(std::string name, int size, std::string cur_format)
{
	if (!size)
		size = 10000;
	has_error = false;
	if (!cur_format.length())
		cur_format = get_format(name);
	file_name = name;
	buffer = NULL;
	file_position = 0;
	if (cur_format == "plain")
	{
		format = 0;
		get_block = &FileParser::InFile::get_plain_block;
		t_file.open(file_name, std::ios::binary);
		if (t_file.fail())
		{
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
		t_file.seekg(0, std::ios::end);
		file_size = t_file.tellg();
		t_file.seekg(0);
	}
	else if (cur_format == "bz2")
	{
		int bzerror;
		format = 1;
		get_block = &InFile::get_bz2_block;
		a_file = new FILE*;
		*a_file = fopen(file_name.c_str(), "rb");
		bz_stream_end = false;
		if (!*a_file)
		{
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
		archive = new BZFILE*;
		*archive = BZ2_bzReadOpen(&bzerror, *a_file, 0, 1, NULL, 0);
		if (bzerror != BZ_OK) {
			error = "Unable to read \"" + file_name + "\"";
			has_error = true;
			BZ2_bzReadClose(&bzerror, *archive);
			return false;
		}
	}
	else if (cur_format == "gz")
	{
		gz_file= gzopen(name.c_str(), "r");
		format = 2;
		get_block = &InFile::get_gz_block;
		if (!gz_file)
		{
			error = "Could not open file " + name;
			has_error = true;
			return false;
		}
	}
	else
	{
		error = "Unknown file format.";
		has_error = true;
		return false;
	}
	if (size)
	{
		buffer_size = size;
		main_buffer_size = size;
		buffer_position = 0;
		buffer = new char[buffer_size];
		buffer[buffer_size - 1] = '\0';
		if (format == 0 && buffer_size > file_size)
			buffer_size = file_size;
		ReadBlock(buffer, &buffer_size);
		file_position = buffer_size;
	}
	return true;
}
bool InFile::Close()
{
	if (format == 0)
	{
		t_file.close();
	}
	else if (format == 1)
	{
		int bzerror;
		BZ2_bzReadClose(&bzerror, *archive);
		delete archive;
		delete a_file;
	}
	else if (format == 2)
	{
		gzclose(gz_file);
	}
	if(buffer)
		delete[] buffer;
	return true;
}
void InFile::ReadBlock(char *str, long long *size)
{
	(*this.*get_block)(str, size);
	str[*size]='\0';
}
void InFile::get_plain_block(char *str, long long *size)
{
	file_name;
	t_file.seekg(file_position);
	if (file_size - (file_position + *size) <= 0)
	{
		*size = file_size - file_position;
		if (*size <= 0)
		{
			*size = 0;
			return;
		}
	}
	t_file.read(str, *size);
	str[*size]='\0';
	file_position += *size;
	if (t_file.fail())
	{
		error = "Error reading file " + file_name + ".\n";
		has_error = true;
	}
}
void InFile::get_bz2_block(char *str, long long *size)
{
	int bzerror;
	*size = BZ2_bzRead(&bzerror, *archive, str, *size);
	if (bzerror != BZ_OK) {
		if (bzerror == BZ_STREAM_END)
		{
			str[*size] = '\0';
			bz_stream_end = true;
			return;
		}
		if (!bz_stream_end)
		{
			error = "Error reading file.";
			has_error = true;
			size = 0;
			return;
		}
	}
	str[*size] = '\0';
}
void InFile::get_gz_block(char *str, long long *size)
{
	long long gz_str_size = *size;
	int ret=gzread(gz_file, (char*)str, gz_str_size);
	str[ret] = '\0';
}
bool InFile::GetLine(char *str)
{
	file_name;
	char *found = strchr(&buffer[buffer_position], '\n');
	char *t = &buffer[buffer_position];
	if (!found)
	{
		if (buffer_position <= buffer_size - 1)
		{
			memmove(buffer, &buffer[buffer_position], buffer_size - buffer_position);
		}
		long long pos = buffer_size - buffer_position;
		long long read_size = main_buffer_size - pos;
		ReadBlock(&buffer[pos], &read_size);
		if (!read_size)
			return false;
		buffer_position = 0;
		buffer_size = pos + read_size;
		found = strchr(&buffer[buffer_position], '\n');
	}
	if (!found)
		return GetLine(str);
	int str_size = (int)(found - &buffer[buffer_position]);
	memcpy(str, &buffer[buffer_position], str_size);
	str[str_size] = '\0';
	buffer_position += str_size + 1;
	return true;
}
bool OutFile::Open(std::string name, long long size, std::string cur_format)
{
	has_error = false;
	if (!cur_format.length())
		cur_format = get_format(name);
	file_name = name;
	buffer_size = size;
	buffer = new char[buffer_size+1];
	buffer_position = 0;
	file_position = 0;
	if (cur_format == "plain")
	{
		format = 0;
		save_block = &OutFile::save_plain_block;
		t_file.open(file_name, std::ios::binary | std::ios::out);
		if (t_file.bad())
		{
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
		t_file.seekp(0, std::ios::beg);
	}
	else if (cur_format == "bz2")
	{
		int bzerror;
		format = 1;
		save_block = &OutFile::save_bz2_block;
		a_file = new FILE*;
		*a_file = fopen(file_name.c_str(), "wb");
		if (!*a_file)
		{
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
		archive = new BZFILE*;
		*archive = BZ2_bzWriteOpen(&bzerror, *a_file, 1, 0, 30);
		if (bzerror != BZ_OK) {
			BZ2_bzWriteClose(&bzerror, *archive, 0, 0, 0);
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
	}
	else if (cur_format == "gz")
	{
		format = 2;
		gz_stream = new z_stream;
		save_block = &OutFile::save_gz_block;
		int level = Z_DEFAULT_COMPRESSION;
		gz_stream->zalloc = Z_NULL;
		gz_stream->zfree = Z_NULL;
		gz_stream->opaque = Z_NULL;
		int ret = deflateInit2(gz_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15 + 16), 8, Z_DEFAULT_STRATEGY);
		gz_str = new unsigned char[buffer_size];
		main_gz_file = new OutFile;
		gz_flush_mode = Z_NO_FLUSH;
		bool result = main_gz_file->Open(file_name, buffer_size, "plain");
		if (!result)
		{
			error = "Unable to open \"" + file_name + "\"";
			has_error = true;
			return false;
		}
	}
	else
	{
		error = "Unknown file format.";
		has_error = true;
		delete[] buffer;
		return false;
	}
}
bool OutFile::SaveBlock(const char *str, int size)
{
	if (!size)
		return true;
	file_name;
	//std::cout << file_name <<" - " << buffer_position << "\n";
	if (buffer_size - buffer_position <= size)
	{
		//std::cout << "flush\n";
		char t = buffer[buffer_position - 1];
		bool res = (*this.*save_block)(buffer, buffer_position - 1);
		buffer_position = 1;
		buffer[0] = t;
		if (!res)
		{
			error = "Error writing to file " + file_name + "\n";
			has_error = true;
			return false;
		}
	}
	memcpy(&buffer[buffer_position], str, size);
	buffer_position += size;
	return true;
}
bool OutFile::save_plain_block(const char *str, int size)
{
	t_file.seekp(file_position, std::ios::beg);
	t_file.write(str, size);
	if (t_file.fail())
	{
		error = "Error reading file " + file_name + ".\n";
		has_error = true;
	}
	file_position += size;
	return true;
}
bool OutFile::save_bz2_block(const char *str, int size)
{
	return true;

	int bzerror;
	//1 > C:\genes\kmeromics\isomer\file_parser.cpp(316, 2) : error C2664 : 'void BZ2_bzWrite(int *,BZFILE *,void *,int)' : cannot convert argument 3 from 'const char *' to 'void *'

	//BZ2_bzWrite(&bzerror, *archive, str, size);
	//if (bzerror == BZ_IO_ERROR) {
	//	BZ2_bzWriteClose(&bzerror, *archive, 0, NULL, NULL);
	//	error = "Error writing data to file.";
	//	has_error = true;
	//	return false;
	//}
	return true;
}
bool OutFile::save_gz_block(const char *str, int size)
{
	gz_stream->avail_in = size;
	gz_stream->next_in = (unsigned char*)str;
	do {
		gz_stream->avail_out = buffer_size;
		gz_stream->next_out = gz_str;
		int ret = deflate(gz_stream, gz_flush_mode);

		if (ret != Z_OK && ret != Z_STREAM_END)
		{
			(void)deflateEnd(gz_stream);
			error = "Error compressing output data.";
			has_error = true;
			return false;
		}
		long long str_size = buffer_size - gz_stream->avail_out;
		if (!main_gz_file->SaveBlock((char*)gz_str, str_size))
		{
			(void)deflateEnd(gz_stream);
			error = "Unable to write to file " + file_name + ".";
			has_error = true;
			return false;
		}
	} while (gz_stream->avail_out == 0);
}
bool OutFile::Close()
{
	if (format == 0)
	{
		if (buffer_position)
			save_plain_block(buffer, buffer_position);
		t_file.close();
	}
	else if (format == 1)
	{
		int bzerror;
		if (buffer_position)
			save_bz2_block(buffer, buffer_position);
		BZ2_bzWriteClose(&bzerror, *archive, 0, NULL, NULL);
		if (bzerror == BZ_IO_ERROR) {
			error = "Unable to save file.";
			has_error = true;
			return false;
		}
		fclose(*a_file);
		delete a_file;
		delete archive;
	}
	else if (format == 2)
	{
		gz_flush_mode = Z_FINISH;
		save_gz_block(buffer, buffer_position);
		(void)deflateEnd(gz_stream);
		main_gz_file->Close();
		delete[] gz_str;
		delete gz_stream;
		delete main_gz_file;
	}
	delete[] buffer;
	return true;
}