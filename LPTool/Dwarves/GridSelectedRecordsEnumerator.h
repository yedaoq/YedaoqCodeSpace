#pragma once

class CGridSelectedRecordsEnumerator : public CEnumeratorBase<IDBRecord>
{
public:
	CGridSelectedRecordsEnumerator(CGridCtrl* pGrid, CDBTableViewer4GridCtrl* pViewer)
		: Grid_(pGrid), Viewer_(pViewer), Idx_(-1)
	{}

	~CGridSelectedRecordsEnumerator(void)
	{
		Grid_ = 0;
		Viewer_ = 0;
	}

	virtual void Reset() { Idx_ = -1; }

	virtual bool MoveNext()
	{
		while(++Idx_ < Viewer_->GetRecordCount())
		{
			if(CEditStyleBool::GetInstance().strTrue == Grid_->GetCell(0, Idx_ + Viewer_->HeadRowCount())->GetText())
			{
				return true;
			}
		}
		return false;
	}

	virtual const IDBRecord& Current()
	{
		ASSERT(Idx_ >= 0 && Idx_ < Viewer_->GetRecordCount());
		Viewer_->GetRecordAt(Idx_, &Rec_);
		return Rec_;
	}

	virtual ICloneable* Clone() const { return new CGridSelectedRecordsEnumerator(*this); }

protected:
	CGridCtrl*					Grid_;
	CDBTableViewer4GridCtrl*	Viewer_;
	int							Idx_;
	NSDBModule::CDBRecordAuto	Rec_;
};
