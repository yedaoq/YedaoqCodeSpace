#pragma once

#include <mytype.h>
#include "EnumEntityHeaderMicro.h"

namespace NSDBModule
{
	BEGINENUMEX(DBColumnSchemaFlag, flag_t)

	ENUMITEM2(None,			0x0000)
	ENUMITEM2(BuildIn,		0x0001)
	ENUMITEM2(DBExist,		0x0002)
	ENUMITEM2(KeyColumn,	0x0004)

	ENUMITEM2(DBPrimaryKey, 0x0010)
	ENUMITEM2(DBUnnull,	0x0020)

	ENDENUM
}
