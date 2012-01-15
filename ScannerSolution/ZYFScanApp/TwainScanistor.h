#pragma once
#include "..\twcommon\Twain.h"

class CTwainSourceManager;

class CTwainScanistor
{
public:
	enum EnumState
	{
		Closed,			// 关闭
		Open,			// 打开
		Enable,			// 启用
		Transfer,		// 传输
	};

public:
	CTwainScanistor(CTwainSourceManager* pMgr, const TW_IDENTITY& id, bool open = true);

	~CTwainScanistor(void);

	CTwainSourceManager*	Manager() const { return ManagerPtr_; }
	const TW_IDENTITY&		ID() const { return ID_; }

	TW_UINT16				Call(TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

	int						Open();
	int						Close();

	void					OnEvent( TW_UINT16 msg );

protected:
	CTwainSourceManager*	ManagerPtr_;
	TW_IDENTITY				ID_;
	
};

