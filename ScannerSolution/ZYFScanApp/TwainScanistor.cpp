#include "StdAfx.h"
#include "TwainScanistor.h"
#include "TwainSourceManager.h"
#include <exception>


CTwainScanistor::CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open)
	: ManagerPtr_(pMgr), ID_(id)
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
	return ManagerPtr_->CallDSMEntry(&ID_, DG, DAT, MSG, pData);
}

CTwainScanistor::~CTwainScanistor(void)
{

}

int CTwainScanistor::Open()
{
	TW_UINT16 twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_OPENDS, &ID_);
	return twRC;
}

int CTwainScanistor::Close()
{
	TW_UINT16 twRC = Call(DG_CONTROL, DAT_IDENTITY,	MSG_CLOSEDS, &ID_);
	return twRC;
}

void CTwainScanistor::OnEvent( TW_UINT16 msg )
{
	switch (msg)
	{
		case MSG_XFERREADY:
			//If AcqFlag >0 then we are in state 5
			if (AcqFlag)
			{
				TWTransferImage(hWnd);
			}
			else if (MessageLevel() >= ML_ERROR)
			{
				ShowRC_CC(hWnd, 0, 0, 0, 
							"Received while not in state 5", 
							"MSG_XFERREADY");
			}
			break;

		case MSG_CLOSEDSREQ:
		case MSG_CLOSEDSOK:
			/*
			* Disable, CloseDS, CloseDSM    
			*/
			LogMessage("CloseDSReq\r\n");
			if (TWDisableDS())
			{
				if (TWCloseDS())
				{
					if (TWCloseDSM(NULL))
					{
						/*
						* SUCCESS
						*/
						CheckSpecialMenu(hWnd, TW_APP_CLOSESM);
					}
					else
					{ 
						CheckSpecialMenu(hWnd, TW_APP_CLOSEDS);
					}
				}
				else 
				{
					CheckSpecialMenu(hWnd, TW_APP_DISABLE);
				}
			}
			break;
			
	// No message from the Source to the App break;
	// possible new message
		case MSG_NULL:
		default:
			break;
	}   
}