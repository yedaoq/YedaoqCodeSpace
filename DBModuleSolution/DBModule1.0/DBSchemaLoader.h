#pragma once

namespace NSDBModule
{
	class CDBModule;
	class CDBTableSchema;
	struct DBColumnSchema;

	class CDBSchemaLoader
	{
	public:
		CDBSchemaLoader(const CDBModule& module)
			: DBModule(module)
		{}

		bool Validate();

	protected:
		CDBModule
	};
}

