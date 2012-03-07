#include "EnumContainerDeclaration.h"

#ifndef BEGINENUM
#define BEGINENUM(name,base,flag) struct Enum##name : public CEnumContainer<Enum##name, base, flag>{	\
	static struct constructor {constructor(collection&);} constructor_;	\
	Enum##name(base v = 0) : CEnumContainer(v){}	\
	enum _Type : base {	
#endif

#ifndef ENDENUM
#define ENDENUM };									\
	operator _Type() const { return (_Type)Value_; } bool operator==(_Type val) const { return Value_ == val;} void operator=(_Type val) { Value_ = val; }	\
	};
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

#define endenum ;operator base() const { return Value_; } bool operator==(base val) const { return Value_ == val;} void operator=(base val) { Value_ = val; }