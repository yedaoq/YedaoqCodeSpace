#pragma once
#include "mytype.h"
#include <winnt.h>

class CStringBuilder
{
public:
	//bool Append(const tstring& str);
	bool Append(LPCTSTR str, int num = 1);
	bool AppendFormat(LPCTSTR fmt, ...);
	bool Append(char val, int num = 1);
	bool Append(int val, int num = 1);
	bool Append(long val, int num = 1);
	bool Append(unsigned int val, int num = 1);
	bool Append(unsigned int val, int num = 1);
	bool Append(float val, int num = 1);
	bool Append(double val, int num = 1);

	int Trim();
	int Trim(int trimCharCount, ...);

	int TrimLeft();
	int TrimLeft(int trimCharCount, ...);

	int TrimRight();
	int TrimRight(int trimCharCount, ...);

	int Replace(const tstring& ori, const tstring& cur);

	size_t Length();
	tstring ToString();

protected:

};

