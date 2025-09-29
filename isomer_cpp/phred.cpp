#include "phred.h"
#include <string>

std::string PhredConverter::GetCountByPhred(char l)
{
	if (l <= 42)
		return std::to_string(l - 33);
	if (l <= 51)
		return std::to_string((l - 42) * 10) + "-" + std::to_string((l - 41) * 10);
	if (l <= 60)
		return std::to_string((l - 51)*100)+"-"+ std::to_string((l - 50) * 100);
	if (l <= 69)
		return std::to_string((l - 60) * 1000) + "-" + std::to_string((l - 59) * 1000);
	if (l <= 76)
		return std::to_string((l - 69) * 10000) + "-" + std::to_string((l - 68) * 10000);
	if (l <= 85)
		return std::to_string((l - 76) * 100000) + "-" + std::to_string((l - 75) * 100000);
	if (l <= 94)
		return std::to_string((l - 85) * 1000000) + "-" + std::to_string((l - 84) * 1000000);
	if (l <= 103)
		return std::to_string((l - 94) * 10000000) + "-" + std::to_string((l - 93) * 1000000);
	if (l < 112)
		return std::to_string((l - 103) * 100000000) + "-" + std::to_string((l - 102) * 100000000);
	return ">1000000000";
}
char PhredConverter::GetPhred(int qt)
{
	if (qt < 1)
		return 33;
	if (qt < 10)
		return 33 + qt;
	if (qt < 100)
		return 42 + qt / 10;
	if (qt < 1000)
		return 51 + qt / 100;
	if (qt < 10000)
		return 60 + qt / 1000;
	if (qt < 100000)
		return 69 + qt / 10000;
	if (qt < 1000000)
		return 76 + qt / 100000;
	if (qt < 10000000)
		return 85 + qt / 1000000;
	if (qt < 100000000)
		return 94 + qt / 10000000;
	if (qt < 1000000000)
		return 103 + qt / 100000000;
	return 112;
}
char PhredConverter::GetCharCount(int qt)
{
	if (qt < 10)
		return 33 + qt;
	if (qt < 100)
		return 42 + qt / 10;
	if (qt < 1000)
		return 51 + qt / 100;
	if (qt < 10000)
		return 60 + qt / 1000;
	if (qt < 100000)
		return 69 + qt / 10000;
	if (qt < 1000000)
		return 76 + qt / 100000;
	if (qt < 10000000)
		return 85 + qt / 1000000;
	if (qt < 100000000)
		return 94 + qt / 10000000;
	if (qt < 1000000000)
		return 103 + qt / 100000000;
	return 112;
}