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
		virtual IDBDataAdapter*			CreateDBAdapter();
		virtual IDBCommand*				CreateDBCommand();
		virtual IDBCommandBuilder*		CreateDBCommandBuilder();
		virtual IDBDataTypeProvider*	CreateDBDataTypeProvider();
		virtual IDBNameMapping*			CreateDBNameMapping();
	};

}