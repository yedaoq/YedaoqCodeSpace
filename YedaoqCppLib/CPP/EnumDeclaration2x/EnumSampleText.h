#include "EnumContainerDeclaration.h"

struct EnumSampleTest:public CEnumContainer<EnumSampleTest>
{
	enum{A, B, C,}
};

//struct EnumSampleTest:public CEnumContainer<EnumSampleTest>
//{
//	BEGINENUM
//		ENUMITEM2(A, 3)
//		ENUMITEM0(B)
//		ENUMITEM2(C, 8)
//		ENUMITEM0(D)
//	ENDENUM
//};