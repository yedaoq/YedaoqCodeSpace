#pragma once
#include "mytype.h"

class CSqlConstituentWrapper
{
public:
	static bool WrapValue4DBInteger(const tchar* val, tstring& buf);
	static bool WrapValue4DBFloat(const tchar* val, tstring& buf);
	static bool WrapValue4DBDouble(const tchar* val, tstring& buf);
	static bool WrapValue4DBDecimal(const tchar* val, int length, int precision, tstring& buf);
	
	static bool WrapValue4DBString(const tchar* val, int length, tstring& buf);

	/*static bool WrapValue4DBUnicode(const string& val, int length, tstring& buf);
	static bool WrapValue4DBUnicode(const wstring& val, int length, tstring& buf);
	static bool WrapValue4DBANSI(const string& val, int length, tstring& buf);
	static bool WrapValue4DBANSI(const wstring& val, int length, tstring& buf);*/
};
