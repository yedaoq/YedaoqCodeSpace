#pragma once

#include "..\DBInterface\DBDataType.h"
#include <vector>
#include <algorithm>
#include "mytype.h"
#include "SqlConstituentWrapper.h"
#include "boost\format.hpp"
#include "Singlton.h"

namespace NSDBModule
{
	class CDBDataTypeBase : public IDBDataType
	{
	public:
		typedef std::vector<EnumCppDataType> CppTypeCollection;

	public:

		CDBDataTypeBase(tstring name, EnumDBDataTypeCategory category, index_t compatibleCPPTypeCount)
			: Name_(name), Category_(category), CompatibleCPPTypes_(0)
		{}

		virtual EnumDBDataTypeCategory Category() const { return Category_; }
		
		virtual const tstring& ToString() const { return Name_; }
		
		virtual EnumCppDataType	PreferredCppDataType() const { return CompatibleCPPTypes_[0]; }

		virtual bool Equal(const IDBDataType* other) const { return ToString() == other->ToString(); };

		virtual bool IsNull(const tchar* val) const {return val == 0 || 0 == _tcslen(val); }

		virtual tstring	Wrapper(const tchar* val) const 
		{ 
			tstring strRet;
			Wrapper(val, strRet);
			return strRet;
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const = 0;

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
			: CDBDataTypeBase(TEXT("integer"), DBNumeric, 2)
		{
			CompatibleCPPTypes_.push_back(CppInt32);
			CompatibleCPPTypes_.push_back(CppBool);
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBInteger(val, buf);	
		}
	};

	class CDBDataTypeFloat : public CDBDataTypeBase
	{
	public:
		CDBDataTypeFloat()
			: CDBDataTypeBase(TEXT("float"), DBNumeric, 3)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBFloat(val, buf);	
		}
	};

	class CDBDataTypeDouble : public CDBDataTypeBase
	{
	public:
		CDBDataTypeDouble()
			: CDBDataTypeBase(TEXT("double"), DBNumeric, 5)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBDouble(val, buf);	
		}
	};

	class CDBDataTypeDecimal : public CDBDataTypeBase
	{
	public:
		CDBDataTypeDecimal(int len, int precision)
			: CDBDataTypeBase(TEXT(""), EnumDBDataTypeCategory::DBString, 5),
			Length_(len), Precision_(precision)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);

			Name_ = (tformat(TEXT("decimal(%u,%u)"))
				%len
				%precision).str();
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBString(val, Length_, buf);	
		}

	protected:
		index_t Length_;
		index_t	Precision_;
	};

	class CDBDataTypeString : public CDBDataTypeBase
	{
	public:
		CDBDataTypeString(int len, bool bFixedWidth, bool bUnicode)
			: CDBDataTypeBase(TEXT(""), EnumDBDataTypeCategory::DBString, 6),
			  Length_(len), FixedWidth_(bFixedWidth), Unicode_(bUnicode)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppString);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);

			if(bFixedWidth)
			{
				Name_ = (bUnicode) ? TEXT("nchar") : TEXT("char");
			}
			else
			{
				Name_ = (bUnicode) ? TEXT("nvarchar") : TEXT("varchar");
			}

			Name_ = (tformat(TEXT("%s(%u)"))
				%Name_
				%len).str();
		}

		virtual bool Wrapper(const tchar* val, tstring& buf) const
		{
			return CSqlConstituentWrapper::WrapValue4DBString(val, Length_, buf);	
		}

	protected:
		index_t Length_;
		bool	FixedWidth_;
		bool	Unicode_;
	};
}