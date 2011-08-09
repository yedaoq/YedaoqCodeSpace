#pragma once

#ifndef interface
#define interface struct
#endif

interface IDBCommand;
interface IDBDataAdapter;

interface IDBProvider
{
	virtual IDBCommand*		CreateDBCommand();
	virtual IDBDataAdapter*	CreateDBAdapter();
};