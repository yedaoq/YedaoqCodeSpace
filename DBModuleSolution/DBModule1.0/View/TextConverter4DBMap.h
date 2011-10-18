#pragma once

#include <mytype.h>
#include <ITextFormater.h>

namespace NSDBModule
{
	class CTextConverter4DBMap : public ITextFormatSwitcher
	{
	public:

		CTextConverter4DBMap()
			: m_InnerAdapter(0), m_Records(0), m_FieldView(-1), m_FieldValue(-1)
		{}

		CTextConverter4DBMap(IEnumerator<IDBRecord>* records, int fieldView, int fieldValue)
			: m_InnerAdapter(0), m_Records(records), m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Records);
		}

		template<typename iter_t>
		CTextConverter4DBMap(const iter_t& begin, const iter_t& end, int fieldView, int fieldValue)
			: m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			m_InnerAdapter	= new CIteratorEnumerator<iter_t>(begin, end);
			m_Records		= m_InnerAdapter;
			ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Records);
		}

		CTextConverter4DBMap(const CTextConverter4DBMap& other)
			:  m_FieldView(other.m_FieldView), m_FieldValue(other.m_FieldValue)
		{
			if(other.m_InnerAdapter)
			{
				m_InnerAdapter = static_cast<IEnumerator<IDBRecord>*>(other.m_InnerAdapter->Clone());
				m_Records = m_InnerAdapter;
			}
			else
			{
				m_InnerAdapter = 0;
				m_Records = other.m_Records;
			}		
		}

		CTextConverter4DBMap& operator=(const CTextConverter4DBMap& other)
		{
			if(other.m_InnerAdapter)
			{
				m_InnerAdapter = static_cast<IEnumerator<IDBRecord>*>(other.m_InnerAdapter->Clone());
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

		~CTextConverter4DBMap()
		{
			if(m_InnerAdapter)
			{
				delete m_InnerAdapter;
				m_InnerAdapter = 0;
			}
		}

		virtual tstring Parse(const tstring& text, IContext* ctx)
		{
			m_Records->Reset();
			CDBRecordAuto rec;
			while(m_Records->MoveNext(rec))
			{
				if(rec.GetField(m_FieldView) == text)
				{
					return rec.GetField(m_FieldValue);
				}
			}
			return text;
		}

		virtual tstring Format(const tstring& val, IContext* ctx)
		{
			m_Records->Reset();
			CDBRecordAuto rec;
			while(m_Records->MoveNext(rec))
			{
				if(rec.GetField(m_FieldValue) == val)
				{
					return rec.GetField(m_FieldView);
				}
			}

			return val;
		}

	protected:

		IEnumerator<IDBRecord>*	m_InnerAdapter;
		IEnumerator<IDBRecord>*	m_Records;

		int m_FieldView;
		int m_FieldValue;

	public:
		tstring m_DefaultView;
		tstring m_DefaultValue;
	};
}
															