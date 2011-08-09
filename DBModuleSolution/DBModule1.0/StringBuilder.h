#pragma once
#include "mytype.h"
#include <winnt.h>

class CStringBuilder
{
public:
	bool Append(const tstring&);
	bool Append(LPCTSTR);
	bool Append(char);

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

