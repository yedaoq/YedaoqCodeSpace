#pragma once

#include <mytype.h>
#include "CellView.h

namespace NSDBModule
{
	class CCellViewDBMap : public ICellView
	{
	public:

		CCellViewDBMap()
			: m_InnerAdapter(0), m_Records(0), m_FieldView(-1), m_FieldValue(-1)
		{}

		CCellViewDBMap(IEnumerator<CDBRecordBase>* records, int fieldView, int fieldValue)
			: m_InnerAdapter(0), m_Records(records), m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Records);
		}

		template<typename iter_t>
		CCellViewDBMap(const iter_t& begin, const iter_t& end, int fieldView, int fieldValue)
			: m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			m_InnerAdapter	= new CIteratorEnumerator<iter_t>(begin, end);
			m_Records		= m_InnerAdapter;
			ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Records);
		}

		CCellViewDBMap(const CCellViewDBMap& other)
			:  m_FieldView(other.m_FieldView), m_FieldValue(other.m_FieldValue)
		{
			if(other.m_InnerAdapter)
			{
				m_InnerAdapter = static_cast<IEnumerator<CDBRecordBase>*>(other.m_InnerAdapter->Clone());
				m_Records = m_InnerAdapter;
			}
			else
			{
				m_InnerAdapter = 0;
				m_Records = other.m_Records;
			}		
		}

		CCellViewDBMap& operator=(const CCellViewDBMap& other)
		{
			if(other.m_InnerAdapter)
			{
				m_InnerAdapter = static_cast<IEnumerator<CDBRecordBase>*>(other.m_InnerAdapter->Clone());
				m_Records = m_InnerAdapter;
			}
			else
			{
				m_InnerAdapter = 0;
				m_Records = other.m_Records;
			}	
			m_FieldView = other.m_FieldView;
			m_FieldValue = other.m_FieldValue;

			return *this;
		}

		~CCellViewDBMap()
		{
			if(m_InnerAdapter)
			{
				delete m_InnerAdapter;
				m_InnerAdapter = 0;
			}
		}

		virtual tstring GetValue(LPCTSTR view, ICellContext* ctx)
		{
			m_Records->Reset();
			CDBRecordBase rec;
			while(m_Records->MoveNext(rec))
			{
				if(rec.GetField(m_FieldView) == view)
				{
					return rec.GetField(m_FieldValue);
				}
			}
			return m_DefaultValue;
		}

		virtual tstring GetView(LPCTSTR val, ICellContext* ctx)
		{
			m_Records->Reset();
			CDBRecordBase rec;
			while(m_Records->MoveNext(rec))
			{
				if(rec.GetField(m_FieldValue) == val)
				{
					return rec.GetField(m_FieldView);
				}
			}

			return m_DefaultView;
		}

	protected:

		IEnumerator<CDBRecordBase>*	m_InnerAdapter;
		IEnumerator<CDBRecordBase>*	m_Records;

		int m_FieldView;
		int m_FieldValue;

	public:
		tstring m_DefaultView;
		tstring m_DefaultValue;
	};
}