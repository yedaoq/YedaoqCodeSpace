#pragma once

#include "..\Module\CPPDataType.h"
#include "..\DBInterface\DBDataType.h"
#include "Singlton.h"

namespace NSDBModule
{
	class CDBDataTypeUnknow : public IDBDataType, public CSingleton<CDBDataTypeUnknow>
	{
		virtual EnumDBDataTypeCategory Category() const { return EnumDBDataTypeCategory::Unknow; }

		virtual const tstring& ToString() const { static tstring name = TEXT("unknow"); return name; }

		virtual EnumCppDataType	PreferredCppDataType() const { return EnumCppDataType::CppUnknow; }

		virtual bool Equal(const IDBDataType* other) const { return ToString() == other->ToString(); };

		virtual bool IsNull(const tchar* val) const { return true; }

		virtual tstring	Wrapper(const tchar* val) const 
		{ 
			return TEXT("null");
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const 
		{ 
			buf.append(TEXT("null")); 
			return 1;
		}

		virtual tstring	UnWrapper(const tchar* val) const {	return TEXT(""); }

		virtual bool CompatibleWith(EnumCppDataType type) const { return false;	}
		virtual bool CompatibleWith(const IDBDataType& type) const { return false; }
	};
}
