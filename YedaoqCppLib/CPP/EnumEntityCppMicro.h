#include "EnumEntityDeclaration.h"

#undef BEGINENUM
#undef ENDENUM
#undef ENUMITEM0
#undef ENUMITEM1
#undef ENUMITEM2
#undef ENUMITEM3

#define ENUMNAME(name) Enum##name

#define BEGINENUM(name) BEGINENUMEX(name, int)

#define BEGINENUMEX(name, base)								\
	const ENUMENTITY<base>& EnumEntityOf##name(){			\
	static ENUMENTITY<base> entity;							\
	static bool				bInitialized = false;			\
	if(!bInitialized)		{bInitialized = true;			\
	base idx = 0;											\

#ifndef BEGININNERENUM
#define BEGININNERENUM(name,owner)	BEGININNERENUMEX(name, int, owner)
#endif

#ifndef BEGININNERENUMEX
#define BEGININNERENUMEX(name, base, owner)					\
	const ENUMENTITY<base>& owner##::##EnumEntityOf##name(){\
	static ENUMENTITY<base> entity;							\
	static bool				bInitialized = false;			\
	if(!bInitialized)		{bInitialized = true;			\
	base idx = 0;
#endif

#define ENDENUM } return entity; }

#define ENUMITEM0(name) entity.Add(make_enumitem(idx++, TEXT(#name), TEXT("")));
#define ENUMITEM1(name, desc) entity.Add(make_enumitem(idx++, TEXT(#name), TEXT(""))); 
#define ENUMITEM2(name, val) idx = val; entity.Add(make_enumitem(idx++, TEXT(#name), TEXT(""))); 
#define ENUMITEM3(name, val, desc) idx = val; entity.Add(make_enumitem(idx++, TEXT(#name), TEXT(#desc)));
