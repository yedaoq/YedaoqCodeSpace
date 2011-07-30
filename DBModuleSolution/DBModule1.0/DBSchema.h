#pragma once
#include "DBDataAdapter.h"

namespace NSDBModule
{
	struct IDBFactory;
	struct IDBDataAdapter;

class CDBSchema
{
public:
	CDBSchema(void);
	int Initialize(IDBFactory& factory);

	IDBDataAdapter* Adapter_;
};

}