#pragma once

#include "mytype.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBSourcePath
	{
		virtual ~IDBSourcePath() {}
		virtual tstring ToString() = 0;
	};
}