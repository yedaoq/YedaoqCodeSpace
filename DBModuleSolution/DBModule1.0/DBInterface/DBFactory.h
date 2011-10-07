#pragma once

#ifndef interface
#define interface struct
#endif

namespace NSDBModule
{
	interface IDBCommand;
	interface IDBDataAdapter;
	interface IDBCommandBuilder;
	interface IDBDataTypeProvider;

	interface IDBFactory
	{
		virtual ~IDBFactory() = 0 {};
		virtual IDBDataAdapter*			CreateDBAdapter() = 0;
		virtual IDBCommand*				CreateDBCommand() = 0;
		virtual IDBCommandBuilder*		CreateDBCommandBuilder() = 0;
		virtual IDBDataTypeProvider*	CreateDBDataTypeProvider() = 0;
		virtual IDBNameMapping*			CreateDBNameMapping() = 0;
	};

}