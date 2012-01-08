#include "StdAfx.h"
#include "TwainScanistor.h"
#include "TwainSourceManager.h"
#include <exception>


CTwainScanistor::CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open)
	: ManagerPtr(pMgr), ID(id)
{
	if(!pMgr)
	{
		throw std::exception();
	}

	if (open)
	{
		Open();
	}
}

TW_UINT16 CTwainScanistor::Call( TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData )
{
	return ManagerPtr->CallDSMEntry(&ID, DG, DAT, MSG, pData);
}

CTwainScanistor::~CTwainScanistor(void)
{
}

int CTwainScanistor::Open()
{
	TW_UINT16 twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_OPENDS, &ID);
	return twRC;
}

int CTwainScanistor::Close()
{
	TW_UINT16 twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_CLOSEDS, &ID);
	return twRC;
}

