#pragma once

#include "..\DBInterface\DBFactory.h"

namespace NSDBModule
{
	class CSqliteFactory : public IDBFactory
	{
		virtual IDBCommand*				CreateDBCommand();
		virtual IDBCommandBuilder*		CreateDBCommandBuilder();
		virtual IDBDataTypeProvider*	CreateDBDataTypeProvider();
	};

}