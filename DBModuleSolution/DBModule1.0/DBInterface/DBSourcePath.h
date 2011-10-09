#pragma once

#include "mytype.h"
#include "Enumerator.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	struct DBConnectionProperty
	{
		tstring Name;
		tstring Value;

		DBConnectionProperty(){}

		DBConnectionProperty(const tstring& name)
			: Name(name)
		{}

		DBConnectionProperty(const tstring& name, const tstring& val)
			: Name(name), Value(val)
		{}
	};

	interface IDBConnection
	{
		typedef IEnumerator<DBConnectionProperty> PropertyEnumerator;

		virtual ~IDBConnection() {}

		virtual PropertyEnumerator* EnumProperty() = 0;
		virtual const tstring&		GetProperty(const tstring& name) = 0;
		virtual bool				SetProperty(const tstring& name, const tstring& val) = 0;

		/*virtual int					Open() = 0;
		virtual int					Close() = 0;*/

		virtual tstring				ToString() = 0;
	};
}