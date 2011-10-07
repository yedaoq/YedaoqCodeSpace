#include "SqlConstituentWrapper.h"

bool CSqlConstituentWrapper::WrapValue4DBInteger(const tchar* val, tstring& buf)
{
	if(!val || _tcslen(val) <= 0)
	{
		buf.append(TEXT("null"));
	}
	else
	{
		buf.append(val);
	}
	return true;
}

bool CSqlConstituentWrapper::WrapValue4DBFloat(const tchar* val, tstring& buf)
{
	if(!val || _tcslen(val) <= 0)
	{
		buf.append(TEXT("null"));
	}
	else
	{
		buf.append(val);
	}
	return true;
}

bool CSqlConstituentWrapper::WrapValue4DBDouble(const tchar* val, tstring& buf)
{
	if(!val || _tcslen(val) <= 0)
	{
		buf.append(TEXT("null"));
	}
	else
	{
		buf.append(val);
	}
	return true;
}

bool CSqlConstituentWrapper::WrapValue4DBDecimal(const tchar* val, int length, int precision, tstring& buf)
{
	if(!val || _tcslen(val) <= 0)
	{
		buf.append(TEXT("null"));
	}
	else
	{
		buf.append(val);
	}

	return true;
}

bool CSqlConstituentWrapper::WrapValue4DBString(const tchar* val, int length, tstring& buf)
{
	buf.reserve(buf.length() + _tcslen(val) + 10);
	buf.append(TEXT("'"));

	// convert '\'' to "''"
	const tchar* pTail = val + _tcslen(val);
	const tchar* pPosPre = val;
	const tchar* pPosCur = 0;
	while(pPosPre < pTail && (pPosCur = _tcschr(pPosPre, '\'')) != NULL)
	{
		if(pPosCur - pPosPre > 1)
		{
			buf.append(pPosPre, pPosCur);
			buf.append(TEXT("''"));
		}

		pPosPre = pPosCur + 1;
	}

	if(pPosPre < pTail)
	{
		buf.append(pPosPre, pTail);
	}

	buf.append(TEXT("'"));

	return true;
}