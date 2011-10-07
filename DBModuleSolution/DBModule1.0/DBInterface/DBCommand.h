#pragma once

#include "mytype.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	enum EnumDBCommandType {Sql, Procedure };

	interface IDBCommand
	{
		virtual const tstring& Text() const = 0;
		virtual const EnumDBCommandType Type() const = 0;
		virtual void SetText(const tstring& text) = 0;
		virtual void SetType(EnumDBCommandType type) = 0;

		virtual ~IDBCommand(){}
	};
}
