#pragma once

#include "Enumerator.h"
#include "mytype.h"

namespace NSDBModule
{
	template<typename iter_t>
	class CDBRecordBaseEnumerator : IEnumerator<CDBRecordBase>
	{
	public:
		CDBRecordBaseEnumerator(){}

		CDBRecordBaseEnumerator(iter_t begin, iter_t end)
			: inner_(begin, end)
		{}

		virtual bool MoveNext()
		{
			return inner_.MoveNext();
		}

		virtual bool MoveNext(CDBRecordBase& obj)
		{
			if(MoveNext())
			{
				obj = Current();
				return true;
			}
			return false;
		}

		virtual const CDBRecordBase& Current()
		{
			return static_cast<const CDBRecordBase>(*inner_.Current());
		}

		virtual void Reset()
		{
			inner_.Reset();
		}

		virtual ICloneable* Clone() const
		{
			return new CDBRecordBaseEnumerator(*this);
		}

	protected:
		CRangeEnumerator<iter_t> inner_;
	};

	template<typename iter_t>
	CDBRecordBaseEnumerator<iter_t> make_recset_enumerator(iter_t first, iter_t last)
	{
		return CDBRecordBaseEnumerator<iter_t>(first, last);
	}

	template<typename iter_t>
	CDBRecordBaseEnumerator<iter_t>* new_recset_enumerator(iter_t first, iter_t last)
	{
		return new CDBRecordBaseEnumerator<iter_t>(first, last);
	}
}