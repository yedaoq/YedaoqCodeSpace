#pragma once

#include "DBDataType.h"
#include <vector>
#include <algorithm>

namespace NSDBModule
{
	class CDBDataTypeBase : public IDBDataType
	{
	public:
		typedef std::vector<EnumCppDataType> CppTypeCollection;

		virtual EnumDBDataTypeCategory Category() const { return Category_; }
		virtual const tstring& ToString() const { return Name_; }
		virtual EnumCppDataType	PreferredCppDataType() const { return CompatibleCPPTypes_[0]; }

		virtual bool IsNull(const tchar* val) const {return val == 0 || 0 == _tcslen(val); }

		virtual tstring	Wrapper(const tchar* val) const { return tstring(val ? val : TEXT("null"));	}
		virtual tstring	UnWrapper(const tchar* val) const {	return tstring(val); }

		virtual bool CompatibleWith(EnumCppDataType type) const 
		{ 
			return 
				std::find(CompatibleCPPTypes_.begin(), CompatibleCPPTypes_.end(), type) 
				!= CompatibleCPPTypes_.end();
		}
		virtual bool CompatibleWith(const IDBDataType& type) const { return false; }

	protected:
		tstring					Name_;
		EnumDBDataTypeCategory	Category_;
		CppTypeCollection		CompatibleCPPTypes_;
	};

	class CDBDataTypeInteger : public CDBDataTypeBase
	{
	public:
		CDBDataTypeInteger()
			: Name_(TEXT("integer")), Category_(DBValue), CompatibleCPPTypes_(2)
		{
			CompatibleCPPTypes_.push_back(CppInt32);
			CompatibleCPPTypes_.push_back(CppBool);
		}

		virtual tstring	Wrapper(const tchar* val) const { return tstring(val ? val : TEXT("null"));	}
		virtual tstring	UnWrapper(const tchar* val) const {	return tstring(val); }

		//virtual bool CompatibleWith(const IDBDataType& type) const = 0;
	};

	class CDBDataTypeFloat : public CDBDataTypeBase
	{
	public:
		CDBDataTypeFloat()
			: Name_(TEXT("float")), Category_(DBValue), CompatibleCPPTypes_(3)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
		}

		virtual tstring	Wrapper(const tchar* val) const { return tstring(val ? val : TEXT("null"));	}
		virtual tstring	UnWrapper(const tchar* val) const {	return tstring(val); }

		//virtual bool CompatibleWith(const IDBDataType& type) const = 0;
	};

	class CDBDataTypeDouble : public CDBDataTypeBase
	{
	public:
		CDBDataTypeDouble()
			: Name_(TEXT("double")), Category_(DBValue), CompatibleCPPTypes_(5)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);
		}

		virtual tstring	Wrapper(const tchar* val) const { return tstring(val ? val : TEXT("null"));	}
		virtual tstring	UnWrapper(const tchar* val) const {	return tstring(val); }

		//virtual bool CompatibleWith(const IDBDataType& type) const = 0;
	};

	class CDBDataTypeVarCharUnicode : CDBDataTypeBase
	{
		
	};

	class CDBDataTypeVarCharANSI : CDBDataTypeBase
	{

	};

	class CDBDataTypeCharUnicode : CDBDataTypeBase
	{

	};

	class CDBDataTypeCharANSI : CDBDataTypeBase
	{

	};
}