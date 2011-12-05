#pragma once
#include <EnumEntityHeaderMicro.h>

namespace NSDBModule
{
	/*enum EnumDMLCommand
	{
		Unknow = 0,
		Insert = 1,
		Update = 2,
		Delete = 4,
	};*/

	BEGINENUM(DMLCommand)

		ENUMITEM3(DMLUnknow,	0,	Unknow)
		ENUMITEM3(DMLInsert,	1,	Insert)
		ENUMITEM3(DMLUpdate,	2,	Update)
		ENUMITEM3(DMLDelete,	4,	Delete)

	ENDENUM
}
