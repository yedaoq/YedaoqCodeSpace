//#include "StdAfx.h"
#include "crtdbg.h"
#include "DBTableViewer.h"
#include "DBColumnViewInfo.h"

using namespace NSDBModule;

int	CDBTableViewColumnCollection::Append(CDBColumnViewInfo* viewInfo, int viewCol, int recCol)
{
	if(!viewInfo)
	{
		_ASSERT(false);
		return -1;
	}

	if(viewCol == -1) viewCol = GetMaxIdxView() + 1;
	if(recCol == -1) recCol = GetMaxIdxRecord() + 1;

	Items_.push_back(DBTableViewColumn(viewInfo, viewCol, recCol));
}

int	CDBTableViewColumnCollection::RemoveAt(int idx)
{
	if(idx < 0 || idx >= Items_.size())
	{
		_ASSERT(false);
		return -1;
	}

	CDBTableViewColumnCollection::ColumnVct::iterator iter = Items_.begin() + idx;
	Items_.erase(iter);

	return 1;
}

CDBTableViewColumnCollection::ColumnEnumerator* CDBTableViewColumnCollection::Enum()
{ 
	return new_iterator_enumerator(Items_.begin(), Items_.end()); 
}

int	CDBTableViewColumnCollection::GetMaxIdxRecord()
{
	int iRet = -1;
	for(CDBTableViewColumnCollection::ColumnVct::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		if(iter->IdxRecord > iRet)
		{
			iRet = iter->IdxRecord;
		}
	}

	return iRet;
}

int	CDBTableViewColumnCollection::GetMaxIdxView()
{
	int iRet = -1;
	for(CDBTableViewColumnCollection::ColumnVct::iterator iter = Items_.begin(); iter != Items_.end(); ++iter)
	{
		if(iter->IdxView > iRet)
		{
			iRet = iter->IdxView;
		}
	}

	return iRet;
}