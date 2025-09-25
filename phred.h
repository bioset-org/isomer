#pragma once
#include <string>
class PhredConverter
{
public:
	char GetCharCount(int qt);
	char GetPhred(int qt);
	std::string GetCountByPhred(char l);
	int Min() { return 33; };
	int Max() { return 112; }
};