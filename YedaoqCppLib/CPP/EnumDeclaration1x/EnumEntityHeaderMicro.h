#include "EnumEntityDeclaration.h"

#ifndef ENUMNAME
#define ENUMNAME(name) Enum##name
#endif

#ifndef BEGINENUM
#define BEGINENUM(name) BEGINENUMEX(name, int)
#endif

#ifndef BEGINENUMEX
#define BEGINENUMEX(name,base)								\
	const ENUMENTITY<base>& EnumEntityOf##name();			\
	enum ENUMNAME(name) : base { 
#endif

#ifndef BEGININNERENUM	
#define BEGININNERENUM(name,owner) BEGININNERENUMEX(name, int, owner)
#endif

#ifndef BEGININNERENUMEX
#define BEGININNERENUMEX(name, base, owner)					\
	static const ENUMENTITY<base>& EnumEntityOf##name();	\
	enum ENUMNAME(name) : base { 
#endif

#ifndef ENDENUM
#define ENDENUM };
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



