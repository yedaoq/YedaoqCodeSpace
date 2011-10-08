#pragma once
#include "..\DBModule1.0\View\DBTableViewer.h"

class CGridCtrl;

using NSDBModule::IDBTableViewer;
using NSDBModule::CDBTableViewColumnCollection;
using NSDBModule::DBTableViewColumn;
using NSDBModule::IDBRecord;


class CDBTableViewer4GridCtrl : public NSDBModule::IDBTableViewer
{
public:

	~CDBTableViewer4GridCtrl(void);

	CDBTableViewer4GridCtrl(void){};
	CDBTableViewer4GridCtrl(const CDBTableViewColumnCollection& columns, CGridCtrl* grid, int gridHeaderRows)
		: ColumnInfos_(columns), Grid_(grid), HeadRowCount(gridHeaderRows)
	{}

	virtual int					Initialize();

	virtual int					Fill(const IEnumerator<IDBRecord>&);

	// record enumerate
	virtual int					GetRecordCount();
	virtual RecordEnumerator*	EnumRecord();	

	// selection info
	virtual int					GetCurRecord(IDBRecord*);
	virtual DBTableViewColumn	GetCurColumn();

	// single record r/w
	virtual int					GetRecordAt(int row, IDBRecord*);
	virtual int					DelRecordAt(int row, IDBRecord*);
	virtual int					SetRecordAt(int row, const IDBRecord&);
	virtual int					NewRecordAt(int row, const IDBRecord&);

	// column info
	IDBTableViewer::ColumnEnumerator*	EnumColumn();

protected:
	CGridColumn					GetGridColumnFromDBColInfo(CDBColumnViewInfo* info);

protected:
	CDBTableViewColumnCollection ColumnInfos_;
	CGridCtrl*					 Grid_;
	int							 HeadRowCount;
};
