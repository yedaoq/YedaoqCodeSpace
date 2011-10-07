#pragma once

#include "Enumerator.h"
#include "mytype.h"
#include "DBRecordFunction.h"
#include "DBRecordComparison.h"

namespace NSDBModule
{
	class CDBEnumeratorSuit
	{
	public:
		typedef CFilterEnumerator<CDBRecordBase, CDBRecordFilter&>				DBRecordFilterEnumerator;
		typedef CConvertEnumerator<tstring, CDBRecordBase, CDBRecordToField>	DBRecordFieldEnumerator;

	public:
		CDBEnumeratorSuit()
			: SourceEnumerator_(0), FilterEnumerator_(SourceEnumerator_, Filter)
		{}

		template<typename iter_t>
		CDBEnumeratorSuit(iter_t first, iter_t last)
			: SourceEnumerator_(new CDBRecordBaseEnumerator(first, last)), FilterEnumerator_(SourceEnumerator_, Filter)
		{}

		CDBEnumeratorSuit(const IEnumerator<CDBRecordBase>& source)
			: SourceEnumerator_(static_cast<IEnumerator<CDBRecordBase>*>(source.Clone())),
			FilterEnumerator_(SourceEnumerator_, Filter)
		{}

		CDBEnumeratorSuit(const CDBEnumeratorSuit& other)
			: Filter(other.Filter),
			SourceEnumerator_(static_cast<IEnumerator<CDBRecordBase>*>(other.SourceEnumerator_->Clone())),
			FilterEnumerator_(SourceEnumerator_, Filter)
		{}

		CDBEnumeratorSuit& operator=(const CDBEnumeratorSuit& other)
		{
			Filter = other.Filter;
			SourceEnumerator_ = static_cast<IEnumerator<CDBRecordBase>*>(other.SourceEnumerator_->Clone());
			FilterEnumerator_.m_Source = SourceEnumerator_;

			return *this;
		}

		~CDBEnumeratorSuit()
		{
			if(SourceEnumerator_)
			{
				delete SourceEnumerator_;
				SourceEnumerator_ = 0;
			}
		}

		template<typename iter_t>
		void SetSource(iter_t first, iter_t last)
		{
			SetSource(make_recset_enumerator(first,last));
		}

		void SetSource(const IEnumerator<CDBRecordBase>& source)
		{
			IEnumerator<CDBRecordBase>* tmp = SourceEnumerator_;
			SourceEnumerator_ = static_cast<IEnumerator<CDBRecordBase>*>(source.Clone());
			FilterEnumerator_.m_Source = SourceEnumerator_;

			if(tmp)
			{
				delete tmp;
			}
		}

		CDBRecordFilter SetFilter(const CDBRecordFilter& newfilter)
		{
			CDBRecordFilter tmp = Filter;
			Filter = newfilter;
			return tmp;
		}

		DBRecordFieldEnumerator& GetFieldEnumerator(int field)
		{
			DBRecordFieldEnumerator& result = FieldEnumerators_[field];
			if(result.m_Converter.Field_ < 0)
			{
				result.m_Converter.Field_ = field;
				result.m_Source = &FilterEnumerator_;
			}

			return result;
		}

		DBRecordFilterEnumerator& GetFilterEnumerator()	{ return FilterEnumerator_; }
		IEnumerator<CDBRecordBase>& GetSourceEnumerator()	{ return *SourceEnumerator_; }

	public:
		CDBRecordFilter							Filter;

	protected:
		IEnumerator<CDBRecordBase>*				SourceEnumerator_;
		DBRecordFilterEnumerator				FilterEnumerator_;
		std::map<int, DBRecordFieldEnumerator>	FieldEnumerators_;
	};
}