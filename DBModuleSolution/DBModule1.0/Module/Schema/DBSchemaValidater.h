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

		static bool ValidateTable(const CDBTableSchema& tbl);
		static bool ValidateBuildInTable(const CDBTableSchema& tbl);
		static bool ValidateExternTable(const CDBTableSchema& tbl);

		static bool ValidateColumn(const DBColumnSchema& tbl);
		static bool ValidateBuildInColumn(const DBColumnSchema& col);
		static bool ValidateExternColumn(const DBColumnSchema& col);

	protected:
		const CDBModule& DBModule;
	};
}


