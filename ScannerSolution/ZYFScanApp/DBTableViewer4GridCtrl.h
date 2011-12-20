#pragma once
#include "..\DBModuleSolution\DBModule1.0\View\DBTableViewer.h"

class CGridCtrl;
class CGridColumn;

using NSDBModule::IDBTableViewer;
using NSDBModule::CDBTableViewColumnCollection;
using NSDBModule::DBTableViewColumn;
using NSDBModule::IDBRecord;
using NSDBModule::CDBColumnViewInfo;


class CDBTableViewer4GridCtrl : public NSDBModule::IDBTableViewer
{
public:
	CDBTableViewer4GridCtrl(void){};
	CDBTableViewer4GridCtrl(CGridCtrl* grid, int gridHeaderRows)
		: Grid_(grid), HeadRowCount_(gridHeaderRows)
	{}

	virtual int					Initialize(const CDBTableViewColumnCollection& columns);

	virtual	CGridCtrl*			Grid() { return Grid_; }
	virtual int					HeadRowCount() {return HeadRowCount_; }

	virtual int					Fill(IEnumerator<IDBRecord>&);
	virtual int					Clear();// { Grid_->SetRowCount(HeadRowCount_); return 1; }

	// record enumerate
	virtual int					GetRecordCount();
	virtual RecordEnumerator*	EnumRecord();	

	// selection info
	virtual int					GetCurRecord(IDBRecord*);
	virtual DBTableViewColumn*	GetCurColumn();

	// single record r/w
	virtual int					GetRecordAt(int row, IDBRecord*);
	virtual int					DelRecordAt(int row, IDBRecord*);
	virtual int					SetRecordAt(int row, const IDBRecord&);
	virtual int					NewRecordAt(int row, const IDBRecord&);

	// column info
	IEnumerator<DBTableViewColumn>*	EnumColumn();

protected:
	CGridColumn					GetGridColumnFromDBColInfo(CDBColumnViewInfo* info);

protected:
	CDBTableViewColumnCollection ColumnInfos_;
	CGridCtrl*					 Grid_;
	int							 HeadRowCount_;
};
