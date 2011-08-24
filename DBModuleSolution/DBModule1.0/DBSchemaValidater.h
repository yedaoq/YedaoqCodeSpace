#pragma once

namespace NSDBModule
{
	class CDBModule;
	class CDBTableSchema;
	struct DBColumnSchema;

	class CDBSchemaValidater
	{
	public:
		CDBSchemaValidater(const CDBModule& module)
			: DBModule(module)
		{}

		bool Validate();

		bool ValidateTable(const CDBTableSchema& tbl);
		bool ValidateBuildInTable(const CDBTableSchema& tbl);
		bool ValidateExternTable(const CDBTableSchema& tbl);

		bool ValidateColumn(const DBColumnSchema& tbl);
		bool ValidateBuildInColumn(const DBColumnSchema& col);
		bool ValidateExternColumn(const DBColumnSchema& col);

	protected:
		const CDBModule& DBModule;
	};

	//enum EnumBuildInSchemaValidity
}


