#pragma once

#include "../DBFactory.h"

namespace NSDBModule
{
	class CSqliteFactory
	{
		virtual IDBDataAdapter*			CreateDBAdapter();
		virtual IDBCommand*				CreateDBCommand();
		virtual IDBCommandBuilder*		CreateDBCommandBuilder();
		virtual IDBDataTypeProvider*	CreateDBDataTypeProvider();
		virtual IDBNameMapping*			CreateDBNameMapping();
	};

}