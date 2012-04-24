#include "EnumContainerDeclaration.h"

#ifndef BEGINENUM
#define BEGINENUM(name,base,flag) struct Enum##name : public nsYedaoqEnum::CEnumContainer<Enum##name, base, flag>{	\
	Enum##name(base v = 0) : CEnumContainer(v){}	\
	static void Initialize();						\
	enum _Type : base {	
#endif

#ifndef ENDENUM
#define ENDENUM };};
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
