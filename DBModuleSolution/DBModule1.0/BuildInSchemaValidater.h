#pragma once

namespace NSDBModule
{
	class CDBModule;
	class CDBTableSchema;
	struct DBColumnSchema;

	class CBuildInSchemaValidater
	{
	public:
		CBuildInSchemaValidater(const CDBModule& module)
			: DBModule(module)
		{}

		bool Invalidate();
		bool ValidateTableSchema(const CDBTableSchema& tbl);
		bool ValidateColumnSchema(const DBColumnSchema& col);

	protected:
		const CDBModule& DBModule;
	};

	//enum EnumBuildInSchemaValidity
}


