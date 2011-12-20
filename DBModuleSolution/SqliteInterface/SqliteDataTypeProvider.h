#pragma once

#include "Singlton.h"
#include "DBCommon/DBDataTypeProviderBase.h"
#include "DBCommon/DBDataTypeCommon.h"

namespace NSDBModule
{
	class CSqliteDataTypeProvider : public CDBDataTypeProvider, public CSingleton<CSqliteDataTypeProvider>
	{
	public:
		CSqliteDataTypeProvider();
	};

	class CSqliteDataTypeText : public CDBDataTypeBase
	{
	public:
		CSqliteDataTypeText()
			: CDBDataTypeBase(TEXT("text"), EnumDBDataTypeCategory::DBString, 6)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppString);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBString(val, -1, buf);	
		}
	};
}