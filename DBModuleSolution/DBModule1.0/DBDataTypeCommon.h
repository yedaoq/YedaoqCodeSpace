#pragma once

#include "DBDataType.h"
#include <vector>
#include <algorithm>
#include "mytype.h"
#include "SqlConstituentWrapper.h"

namespace NSDBModule
{
	class CDBDataTypeBase : public IDBDataType
	{
	public:
		typedef std::vector<EnumCppDataType> CppTypeCollection;

		virtual EnumDBDataTypeCategory Category() const { return Category_; }
		
		virtual const tstring& ToString() const { return Name_; }
		
		virtual EnumCppDataType	PreferredCppDataType() const { return CompatibleCPPTypes_[0]; }

		virtual bool Equal(const IDBDataType* other){ return ToString() == other->ToString(); };

		virtual bool IsNull(const tchar* val) const {return val == 0 || 0 == _tcslen(val); }

		virtual tstring	Wrapper(const tchar* val) const 
		{ 
			tstring strRet;
			Wrapper(val, strRet);
			return strRet;
		}

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

	class CDBDataTypeUnknow : public IDBDataType
	{
		virtual EnumDBDataTypeCategory Category() const { return EnumDBDataTypeCategory::Unknow; }

		virtual const tstring& ToString() const { static tstring name = TEXT("unknow"); return name; }

		virtual EnumCppDataType	PreferredCppDataType() const { return EnumCppDataType::CppUnknow; }

		virtual bool Equal(const IDBDataType* other){ return ToString() == other->ToString(); };

		virtual bool IsNull(const tchar* val) const { return true; }

		virtual tstring	Wrapper(const tchar* val) const 
		{ 
			return TEXT("null");
		}

		virtual tstring	UnWrapper(const tchar* val) const {	return TEXT(""); }

		virtual bool CompatibleWith(EnumCppDataType type) const { return false;	}
		virtual bool CompatibleWith(const IDBDataType& type) const { return false; }
	};

	class CDBDataTypeInteger : public CDBDataTypeBase
	{
	public:
		CDBDataTypeInteger()
			: Name_(TEXT("integer")), Category_(DBNumeric), CompatibleCPPTypes_(2)
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
			: Name_(TEXT("float")), Category_(DBNumeric), CompatibleCPPTypes_(3)
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
			: Name_(TEXT("double")), Category_(DBNumeric), CompatibleCPPTypes_(5)
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
			: Category_(EnumDBDataTypeCategory::DBString), CompatibleCPPTypes_(5),
			Length_(len), Precision_(precision)
		{
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppDouble);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppFloat);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppInt32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppUint32);
			CompatibleCPPTypes_.push_back(EnumCppDataType::CppBool);

			Name_ = TEXT("decimal");
			Name_.reserve(31);
			tprintf(Name_.c_str() + Name_.length(), 30, TEXT("decimal(%u,%u)"), len, precision);
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
			: Category_(EnumDBDataTypeCategory::DBString), CompatibleCPPTypes_(6),
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

			Name_.reserve(Name_.length() + 13);
			tprintf(Name_.c_str() + Name_.length(), 12, TEXT("(%u)"), len);
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