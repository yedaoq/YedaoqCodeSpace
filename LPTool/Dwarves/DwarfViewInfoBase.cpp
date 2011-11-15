#include "StdAfx.h"
#include "DwarfViewInfoBase.h"

IEnumerator<DwarfViewOperationItem>* CDwarfViewInfoBase::EnumOperation() 
{ 
	Initialize();
	return new_iterator_enumerator(Operations.Items.begin(), Operations.Items.end()); 
}

IEnumerator<IDwarfViewInfo*>* CDwarfViewInfoBase::EnumReleatedView() 
{ 
	Initialize();
	return new_iterator_enumerator(ReleatedViews.begin(), ReleatedViews.end()); 
}

CDBTableViewColumnCollection& CDwarfViewInfoBase::GetViewColumnCollection()
{ 
	Initialize();
	return ViewColumns; 
}

void CDwarfViewInfoBase::ExecuteOperation(index_t id, DwarfViewOperationContext* pCtx)
{

}