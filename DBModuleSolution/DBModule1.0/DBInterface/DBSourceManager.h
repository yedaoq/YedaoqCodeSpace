#pragma once

#include "mytype.h"
#include "Enumerator.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBSourcePath;

	interface IDBSourceManager
	{
		virtual ~IDBSourceManager() {}

		virtual bool SourceEnumerable() = 0;
		virtual IEnumerator<IDBSourcePath>* EnumDBSource() = 0;
		virtual IDBSourcePath* PromptOpenDBSource() = 0;
		virtual IDBSourcePath* PromptCreateDBSource() = 0;
		virtual IDBSourcePath* OpenDBSource(const tstring& connStr) = 0;
	};
}
