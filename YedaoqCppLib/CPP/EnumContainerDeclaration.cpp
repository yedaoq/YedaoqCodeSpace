/* ___________________________________________________________________________
@ 
@ file - EnumContainerDeclaration.cpp
@ 
@ auth - yedaoq@gmail.com          http://blog.yedaoq.info
@ date - 2012-1-19
@ info -
@     
/* ___________________________________________________________________________*/
#pragma once
#include "Enumerator.h"

#ifndef BEGININNERENUM	
#define BEGININNERENUM(name, base, flag) struct Enum##name : CEnumContainer<base, flag> { \
	enum _Type : base {
#endif

#ifndef ENDENUM
#define ENDENUM }; \
	operator _Type() const { return _Value; } \
	CEnumContainer& operator=(_Type val) { _Value = val; return *this; }
#endif

#ifndef ENUMITEM0
#define ENUMITEM0(name) name,
#endif

#ifndef ENUMITEM1
#define ENUMITEM1(name, desc) name, 
#endif

#ifndef ENUMITEM2
#define ENUMITEM2(name, val) name = val, 
#endif

#ifndef ENUMITEM3
#define ENUMITEM3(name, val, desc) name = val,
#endif

template<typename enumbase>
struct tagEnumItem
{
	enumbase	Val;
	tstring		ValStr;
	tstring		Description;
};

template<typename enumbase = int, bool flag = false>
struct CEnumContainer
{
protected:
	enumbase _Value;

public:
	typedef	tagEnumItem<enumbase>				enumitem;
	typedef IEnumerator<tagEnumItem<enumbase>>	enumerator;

	operator enumbase() const { return _value; }
	CEnumContainer& operator=(enumbase val) { _Value = val; return *this; }

	const tstring&			str() const;
	const tstring&			desc() const;

	static enumerator*		items();
	static const enumitem&	item(enumbase val) const;
	static const enumitem&	item(const tstring& val) const;
	static const enumitem&	item(const tchar*	val) const;
};