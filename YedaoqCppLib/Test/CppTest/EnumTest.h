#include "EnumDeclaration3x/EnumHeaderMicro.h"

BEGINENUM(Test, short, false)
	ENUMITEM0(A)
	ENUMITEM1(B, bd)
	ENUMITEM2(C, 5)
	ENUMITEM3(D, 7, dd)
	ENUMITEM0(E)
ENDENUM

// struct EnumTest : public CEnumContainer<EnumTest, short, false>
// { 
// 	static struct constructor 
// 	{
// 		constructor(CEnumContainer<EnumTest, short, false>::collection&);
// 	} constructor_; 
// 	EnumTest(short v = 0) : CEnumContainer(v){} 
// 	enum _Type : short {
// 		A,
// 		B,
// 		C = 5,
// 		D = 7,
// 		E,
// 	}; 
// 	operator _Type() const { return (_Type)Value_; } 
// 	bool operator==(_Type val) const { return Value_ == val;} 
// 	void operator=(_Type val) { Value_ = val; } 
// };

