#pragma once

#include <mytype.h>
#include "ITextFormater.h"
#include "Singlton.h"
#include "Enumerator.h"

struct IEditStyle
{
	virtual ~IEditStyle() = 0 {};
};

class CEditStyleNone : public IEditStyle, public CSingleton<CEditStyleNone>
{

};

class CEditStyleOptional : public IEditStyle
{
public:
	CEditStyleOptional()
		: Options(0) , OtherAcceptable(false)
	{}

	CEditStyleOptional(IEnumerator<tstring>* opts)
		: Options(opts), OtherAcceptable(false)
	{}

	IEnumerator<tstring> *Options;
	bool				  OtherAcceptable;
};

class CEditStyleBool : public IEditStyle
{
public:
	CEditStyleBool()
		: strTrue(TEXT("1")), strFalse(TEXT("0"))
	{}

	CEditStyleBool(const tchar* t, const tchar* f)
		: strTrue(t), strFalse(f)
	{}

	const CEditStyleBool& DefaultInstance()
	{
		static CEditStyleBool default;
		return default;
	}

	tstring strTrue;
	tstring strFalse;
};

class CEditStyleLink : public IEditStyle
{
public:
	virtual ~CEditStyleLink() = 0 {};

	virtual tstring OnLinkClicked(IContext*) = 0;
};
