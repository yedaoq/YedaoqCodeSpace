#pragma once

#include <mytype.h>
#include <ITextFormater.h>
#include <Helper.h>
#include <Enumerator.h>
#include "Module\DBRecord.h"

namespace NSDBModule
{
	class CTextConverter4DBMap : public ITextFormatSwitcher
	{
	public:

		CTextConverter4DBMap()
			: m_SourceOwned(0), m_Source(0), m_FieldView(-1), m_FieldValue(-1)
		{}

		CTextConverter4DBMap(IEnumerator<IDBRecord>* records, int fieldView, int fieldValue)
			: m_SourceOwned(0), m_Source(records), m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			_ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Source);
		}

		template<typename iter_t>
		CTextConverter4DBMap(const iter_t& begin, const iter_t& end, int fieldView, int fieldValue)
			: m_SourceOwned(true), m_FieldView(fieldView), m_FieldValue(fieldValue)
		{
			m_Source		= new_iterator_enumerator_ex<IDBRecord>(begin, end);
			ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Source);
		}

		CTextConverter4DBMap(const CTextConverter4DBMap& other)
			: m_SourceOwned(0), m_Source(0)
		{
			operator=(other);	
		}

		CTextConverter4DBMap& operator=(const CTextConverter4DBMap& other)
		{
			Dispose();
			m_SourceOwned = other.m_SourceOwned;
			this->m_Source = m_SourceOwned ? 
				static_cast<IEnumerator<IDBRecord>*>(other.m_Source->Clone()) : other.m_Source;

			m_FieldView = other.m_FieldView;
			m_FieldValue = other.m_FieldValue;

			return *this;
		}

		~CTextConverter4DBMap()
		{
			Dispose();
		}

		void SetSource(IEnumerator<IDBRecord>& records)
		{
			Dispose();
			m_SourceOwned = true;
			m_Source = static_cast<IEnumerator<IDBRecord>*>(records.Clone());
		}

		void SetSource(IEnumerator<IDBRecord>* records)
		{
			Dispose();
			m_SourceOwned = false;
			m_Source = records;
		}

		void Dispose()
		{
			if(m_SourceOwned) delete m_Source;
			m_SourceOwned = false;
			m_Source = 0;
		}

		virtual tstring Parse(const tstring& text, IContext* ctx)
		{
			m_Source->Reset();
			CDBRecordAuto rec;
			while(m_Source->MoveNext(rec))
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
			//TTRACE(TEXT("DBMap : %s -> "), val.c_str());
			m_Source->Reset();
			CDBRecordAuto rec;
			while(m_Source->MoveNext(rec))
			{
				TTRACE(TEXT("DBMap Enum : %s  Target : %s\r\n"), rec.GetField(m_FieldValue).c_str(), val.c_str());
				if(rec.GetField(m_FieldValue) == val)
				{
					TTRACE(TEXT("DBMap Format Result : %s\r\n"), rec.GetField(m_FieldView).c_str());
					return rec.GetField(m_FieldView);
				}
			}

			//TTRACE(TEXT("%s\r\n"), val.c_str());

			return val;
		}

	protected:

		bool					m_SourceOwned;
		IEnumerator<IDBRecord>*	m_Source;

		int						m_FieldView;
		int						m_FieldValue;

	public:
		tstring m_DefaultView;
		tstring m_DefaultValue;
	};
}
															