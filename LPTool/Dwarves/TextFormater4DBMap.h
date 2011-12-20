#pragma once
#include "DBEnumeratorProvider.h"

class CTextConverter4DBMap : public ITextFormatSwitcher
{
public:

	CTextConverter4DBMap()
		: m_Table(-1), m_FieldView(-1), m_FieldValue(-1)
	{}

	CTextConverter4DBMap(int tbl, int fieldView, int fieldValue)
		: m_Table(tbl), m_FieldView(fieldView), m_FieldValue(fieldValue)
	{
		_ASSERT(m_FieldView >= 0 && m_FieldValue >= 0 && m_Table >= 0);
	}

	CTextConverter4DBMap(const CTextConverter4DBMap& other)
		: m_Table(other.m_Table), m_FieldView(other.m_FieldView), m_FieldValue(other.m_FieldValue)
	{}

	CTextConverter4DBMap& operator=(const CTextConverter4DBMap& other)
	{
		m_Table = other.m_Table;
		m_FieldView = other.m_FieldView;
		m_FieldValue = other.m_FieldValue;

		return *this;
	}

	~CTextConverter4DBMap()
	{
		m_Table = -1;
	}

	virtual tstring Parse(const tstring& text, IContext* ctx)
	{
		CDBRecordBase rec;
		IEnumerator<IDBRecord>* pEnumRec = &CDBEnumeratorProvider::GetInstance()[m_Table].GetFilterEnumerator();
		pEnumRec->Reset();

		while(pEnumRec->MoveNext())
		{
			rec = pEnumRec->Current();
			if(rec.GetField(m_FieldView) == text)
			{
				//TTRACE(TEXT("DBMap Parse End \n"));
				return rec.GetField(m_FieldValue);
			}
		}
		//TTRACE(TEXT("DBMap Parse End \n"));
		return text;
	}

	virtual tstring Format(const tstring& val, IContext* ctx)
	{
		//TTRACE(TEXT("DBMap : %s -> "), val.c_str());
		CDBRecordBase rec;
		IEnumerator<IDBRecord>* pEnumRec = &CDBEnumeratorProvider::GetInstance()[m_Table].GetFilterEnumerator();
		pEnumRec->Reset();

		while(pEnumRec->MoveNext())
		{
			rec = pEnumRec->Current();
			//TTRACE(TEXT("DBMap Enum : %s  Target : %s\r\n"), rec.GetField(m_FieldValue).c_str(), val.c_str());
			if(rec.GetField(m_FieldValue) == val)
			{
				//TTRACE(TEXT("DBMap Format End \n"));
				//TTRACE(TEXT("DBMap Format Result : %s\r\n"), rec.GetField(m_FieldView).c_str());
				return rec.GetField(m_FieldView);
			}
		}

		//TTRACE(TEXT("%s\r\n"), val.c_str());
		//TTRACE(TEXT("DBMap Format End \n"));
		return val;
	}

protected:

	int		m_Table;

	int		m_FieldView;
	int		m_FieldValue;

public:
	tstring m_DefaultView;
	tstring m_DefaultValue;
};