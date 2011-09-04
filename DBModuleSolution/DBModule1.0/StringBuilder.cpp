#include "StringBuilder.h"
#include <crtdefs.h>
//#include <wchar.h>

bool CStringBuilder::Append(const tchar* str)
{
	Story.append(str);
	return true;
}

bool CStringBuilder::Append(const tchar* format, ...)
{
	//va_list args(format);

	return true;
}

bool CStringBuilder::Append(char val, int num)
{
	Story.append(num, val);
	return false;
}

bool CStringBuilder::Append(int val, int num)
{
	//Story.append()
	return false;
}

bool CStringBuilder::Append(long val, int num)
{
	return false;
}

bool CStringBuilder::Append(unsigned int val, int num)
{
	return false;
}

bool CStringBuilder::Append(unsigned long val, int num)
{
	return false;
}

bool CStringBuilder::Append(float val, int num)
{
	return false;
}

bool CStringBuilder::Append(double val, int num)
{
	return false;
}

int CStringBuilder::Trim()
{
	return false;
}

int CStringBuilder::Trim(int trimCharCount, ...)
{
	return 0;
}

int CStringBuilder::TrimLeft()
{
	return 0;
}

int CStringBuilder::TrimLeft(int trimCharCount, ...)
{
	return 0;
}

int CStringBuilder::TrimRight()
{
	return 0;
}

int CStringBuilder::TrimRight(int trimCharCount, ...)
{
	return 0;
}

int CStringBuilder::Replace(const tstring& ori, const tstring& cur)
{
	return 0;
}

size_t CStringBuilder::Length()
{
	return 0;
}

tstring CStringBuilder::ToString()
{
	return tstring();
}
