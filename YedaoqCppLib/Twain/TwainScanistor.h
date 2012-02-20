#pragma once
#include "Twain.h"

class CTwainSourceManager;

class CTwainScanistor
{
public:
	enum EnumState
	{
		Closed = 3,		
		Opened,			
		Enabled,		
		Transferring,	
	};

	enum EnumXferMode
	{
		Native = TWSX_NATIVE,
		Memory = TWSX_MEMORY,
		File = TWSX_FILE,	
		Unknow,		
	};

public:
	CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open = true);

	~CTwainScanistor(void);

	CTwainSourceManager*	Manager() const { return ManagerPtr_; }
	const TW_IDENTITY&		ID() const { return ID_; }
	EnumState				State() const { return State_; }

	TW_UINT16				Call(TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

	TW_UINT16				Open();
	TW_UINT16				Close();

	TW_UINT16				Enable();
	TW_UINT16				Disable();

	TW_UINT16				XferMode() const { return XferMode_; }
	bool					SetXferMode(EnumXferMode mode);

	TW_UINT16				GetCapbility(TW_UINT16 id, TW_CAPABILITY& cap);
	TW_UINT16				SetCapbility(TW_UINT16 id, const CCapDataContainerBase& data);
	TW_UINT16				ResetCapbility();

	void					OnEvent( TW_UINT16 msg );

protected:
	TW_UINT16				GetImage();

protected:
	CTwainSourceManager*	ManagerPtr_;
	TW_IDENTITY				ID_;
	EnumState				State_;
	TW_USERINTERFACE		Interface_;
	EnumXferMode			XferMode_;
};

