#pragma once

#include "mytype.h"
#include "DBCommand.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	struct DBColumnSchema;
	class CDBTableSchema;
	class CDBRecordBase;
	class CDBRecordComparison;
	interface IDBCommand;
	interface IDBDataType;

	interface IDBCommandBuilder
	{
		virtual ~IDBCommandBuilder() = 0 {}
		virtual int Initialize(CDBTableSchema const*) = 0;

		virtual int GetCmdSelect(const CDBRecordBase&, const CDBRecordComparison&, IDBCommand**) = 0;
		virtual int GetCmdSelect(IDBCommand**) = 0;

		virtual int GetCmdInsert(const CDBRecordBase&, IDBCommand**) = 0;
		virtual int GetCmdDelete(const CDBRecordBase&, const CDBRecordComparison& cmp,  IDBCommand**) = 0;
		virtual int GetCmdUpdate(const CDBRecordBase&, const CDBRecordBase&, const CDBRecordComparison& cmp, IDBCommand**) = 0;

		virtual int GetCmdCreate() = 0;

		virtual int ReleaseCmd(IDBCommand *, bool);
	};
}