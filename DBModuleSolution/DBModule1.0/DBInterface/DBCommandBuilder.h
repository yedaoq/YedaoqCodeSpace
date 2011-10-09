#pragma once

#include "mytype.h"
#include "DBCommand.h"

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBRecord;
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

		virtual int GetCmdSelect(const IDBRecord&, const CDBRecordComparison&, IDBCommand**) = 0;
		virtual int GetCmdSelect(IDBCommand**) = 0;

		virtual int GetCmdInsert(const IDBRecord&, IDBCommand**) = 0;
		virtual int GetCmdDelete(const IDBRecord&, const CDBRecordComparison& cmp,  IDBCommand**) = 0;
		virtual int GetCmdUpdate(const IDBRecord&, const IDBRecord&, const CDBRecordComparison& cmp, IDBCommand**) = 0;

		virtual int GetCmdCreate(IDBCommand**) = 0;

		virtual int ReleaseCmd(IDBCommand *, bool) = 0;
	};
}