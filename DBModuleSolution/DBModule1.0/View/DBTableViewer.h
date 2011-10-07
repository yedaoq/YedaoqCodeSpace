#pragma once
#include "Enumerator.h"
#include "DBRecord.h"
#include "DBColumnViewInfo.h"
#include <vector>

namespace NSDBModule
{
	struct DBTableViewColumn
	{
		DBTableViewColumn()
			: ViewInfo(0), IdxRecord(-1), IdxView(-1)
		{}

		DBTableViewColumn(CDBColumnViewInfo* viewInfo, int idxRec, int idxView)
			: ViewInfo(viewInfo), IdxRecord(idxRec), IdxView(idxView)
		{}

		CDBColumnViewInfo*	ViewInfo;
		int					IdxRecord;
		int					IdxView;
	};

	class CDBTableViewColumnCollection
	{
	public:
		typedef IEnumerator<DBTableViewColumn> ColumnEnumerator;
		typedef std::vector<DBTableViewColumn> ColumnVct;

	public:
		CDBTableViewColumnCollection(int iCount = 8)
			: Items_(0)
		{
			Items_.reserve(iCount);
		}

		//CDBTableViewColumnCollection(const CDBTableViewColumnCollection& other)
		//	: Items_(other.Items_.begin(), other.Items_.end())
		//{}

		int					Append(CDBColumnViewInfo* viewInfo, int viewCol = -1, int recCol = -1);
		int					RemoveAt(int idx);
		
		ColumnEnumerator*	Enum();

	protected:
		int					GetMaxIdxRecord();
		int					GetMaxIdxView();

	protected:
		ColumnVct Items_;
	};

	class IDBTableViewer
	{
	public:
		typedef IEnumerator<IDBRecord>			RecordEnumerator;
		typedef IEnumerator<DBTableViewColumn>	ColumnEnumerator;

	public:
		~IDBTableViewer(void) {};

		virtual int					Fill(const IEnumerator<IDBRecord>&) = 0;
		
		// record enumerate
		virtual int					GetRecordCount() = 0;
		virtual RecordEnumerator*	EnumRecord() = 0;	

		// selection info
		virtual int					GetCurRecord(IDBRecord*) = 0;
		virtual DBTableViewColumn	GetCurColumn() = 0;
		
		// single record r/w
		virtual int					GetRecordAt(int row, IDBRecord*) = 0;
		virtual int					DelRecordAt(int row, IDBRecord*) = 0;
		virtual int					SetRecordAt(int row, const IDBRecord&) = 0;
		virtual int					NewRecordAt(int row, const IDBRecord&) = 0;

		// column info
		virtual ColumnEnumerator*	EnumColumn() = 0;
	};
}
