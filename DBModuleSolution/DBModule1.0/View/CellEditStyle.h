#pragma once

#include <mytype.h>
#include "Singlton.h"
#include "Enumerator.h"
#include <ITextFormater.h>

namespace NSDBModule
{
	struct ICellEditStyle
	{
		virtual ~ICellEditStyle() = 0 {};
	};

	class CCellEditStyleNone : public ICellEditStyle, public CSingleton<CCellEditStyleNone>
	{

	};

	class CCellEditStyleOptional : public ICellEditStyle
	{
	public:
		CCellEditStyleOptional()
			: Options(0) , OtherAcceptable(false)
		{}

		CCellEditStyleOptional(IEnumerator<tstring>* opts)
			: Options(opts), OtherAcceptable(false)
		{}

		IEnumerator<tstring> *Options;
		bool				  OtherAcceptable;
	};

	class CCellEditStyleBool : public ICellEditStyle
	{
	public:
		CCellEditStyleBool()
			: strTrue(TEXT("1")), strFalse(TEXT("0"))
		{}

		CCellEditStyleBool(const tchar* t, const tchar* f)
			: strTrue(t), strFalse(f)
		{}

		CCellEditStyleBool(const CCellEditStyleBool& other)
		{
			strTrue = other.strTrue;
			strFalse = other.strFalse;
		}

		CCellEditStyleBool& operator=(const CCellEditStyleBool& other)
		{
			strTrue = other.strTrue;
			strFalse = other.strFalse;
			return *this;
		}

		tstring strTrue;
		tstring strFalse;

		static const CCellEditStyleBool DefaultInstance;
	};

	class CCellEditStyleLink : public ICellEditStyle
	{
	public:
		virtual ~CCellEditStyleLink() = 0 {};

		virtual tstring OnLinkClicked(IContext*) = 0;
	};
}