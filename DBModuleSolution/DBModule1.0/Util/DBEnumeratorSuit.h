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
		typedef CFilterEnumerator<IDBRecord, CDBRecordFilter>				DBRecordFilterEnumerator;
		typedef CConvertEnumerator<tstring, IDBRecord, CDBRecordToField>	DBRecordFieldEnumerator;

	public:
		CDBEnumeratorSuit()
			: SourceOwned_(false), SourceEnumerator_(0), FilterEnumerator_(SourceEnumerator_, Filter)
		{}

		template<typename iter_t>
		CDBEnumeratorSuit(iter_t first, iter_t last)
			: SourceOwned_(false), SourceEnumerator_(0), FilterEnumerator_(SourceEnumerator_, Filter)
		{
			SetSource(first, last);
		}

		CDBEnumeratorSuit(const IEnumerator<IDBRecord>& source)
			: SourceOwned_(false), SourceEnumerator_(0), FilterEnumerator_(SourceEnumerator_, Filter)
		{
			SetSource(source);
		}

		CDBEnumeratorSuit(const IEnumerator<IDBRecord>* source)
			: SourceOwned_(false), SourceEnumerator_(0), FilterEnumerator_(SourceEnumerator_, Filter)
		{
			SetSource(source);
		}

		CDBEnumeratorSuit(const CDBEnumeratorSuit& other)
			: SourceOwned_(false), SourceEnumerator_(0), Filter(other.Filter), FilterEnumerator_(SourceEnumerator_, Filter)
		{
			if(other.SourceOwned_)
			{
				SetSource(*other.SourceEnumerator_);
			}
			else
			{
				SetSource(other.SourceEnumerator_);
			}
		}

		CDBEnumeratorSuit& operator=(const CDBEnumeratorSuit& other)
		{
			if(other.SourceOwned_)
			{
				SetSource(*other.SourceEnumerator_);
			}
			else
			{
				SetSource(other.SourceEnumerator_);
			}

			Filter = other.Filter;

			return *this;
		}

		~CDBEnumeratorSuit()
		{
			Dispose();
		}

		void Dispose()
		{
			if(SourceOwned_ && SourceEnumerator_) delete SourceEnumerator_;
			SourceEnumerator_ = 0;			
		}

		template<typename iter_t>
		void SetSource(iter_t first, iter_t last)
		{
			Dispose();
			SourceOwned_ = true;
			FilterEnumerator_.m_Source = SourceEnumerator_ = new_iterator_enumerator_ex<IDBRecord>(first,last);
		}

		void SetSource(IEnumerator<IDBRecord>& source)
		{
			Dispose();
			SourceOwned_ = true;
			FilterEnumerator_.m_Source = SourceEnumerator_ = static_cast<IEnumerator<IDBRecord>*>(source.Clone());
		}

		void SetSource(IEnumerator<IDBRecord>* source)
		{
			Dispose();
			SourceOwned_ = false;
			FilterEnumerator_.m_Source = SourceEnumerator_ = source;
		}

		const CDBRecordFilter& Filter() {return FilterEnumerator_.m_Filter; }

		CDBRecordFilter SetFilter(const CDBRecordFilter& newfilter)
		{
			CDBRecordFilter tmp = Filter;
			FilterEnumerator_.m_Filter = newfilter;
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

		DBRecordFilterEnumerator& GetFilterEnumerator()		{ return FilterEnumerator_; }
		IEnumerator<CDBRecordBase>& GetSourceEnumerator()	{ return *SourceEnumerator_; }

	protected:
		bool									SourceOwned_;
		IEnumerator<IDBRecord>*					SourceEnumerator_;
		DBRecordFilterEnumerator				FilterEnumerator_;
		std::map<int, DBRecordFieldEnumerator>	FieldEnumerators_;
	};
}