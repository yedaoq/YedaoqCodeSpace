#pragma once

#include "Singlton.h"
#include "Enumerator.h"
#include "..\twcommon\Twain.h"
#include "TwainScanistor.h"
#include <exception>



class CTwainSourceManager : public CSingleton<CTwainSourceManager>
{
	friend class CSingleton<CTwainSourceManager>;
public:
	enum EnumState
	{
		Prepare,	// nothing can do
		DSMLoad,	// DSM dll has been loaded in, but does not init
		DSMOpen,	// DSM has been initialized
	};

protected:
	CTwainSourceManager(void);
	
public:
	~CTwainSourceManager(void);

	EnumState					GetState() const { return State; }

	int							MessageLevel (VOID) const {	return (MessageLevelVAR); }
	void						SetMessageLevel (int level) { MessageLevelVAR = level; }

	int							Load();
	int							UnLoad();

	int							OpenDSM();
	int							CloseDSM();

	IEnumerator<TW_IDENTITY>*	EnumSource();

	int							PromptSelectSource(CTwainScanistor** ppScanistor);
	CTwainScanistor*			OpenSource(LPCTSTR lpProductName);
	CTwainScanistor*			OpenSource(const TW_IDENTITY& sourceID);

	TW_UINT16 CallDSMEntry(pTW_IDENTITY pSrc, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData);

protected:
	EnumState		State;			// glue code flag for an Open Source Manager
	HMODULE			DSMModule;		// handle to Source Manager for explicit load
	DSMENTRYPROC	DSMEntry;		// entry point to the SM
	TW_IDENTITY		AppID;			// twain application identify
	HWND			WndNotify;		// 

	int				MessageLevelVAR;
};

class CTwainSourceEnumerator : public CEnumeratorBase<TW_IDENTITY>
{
public:
	CTwainSourceEnumerator(CTwainSourceManager* mgr)
		: SourceMgr(mgr), MsgID(MSG_GETFIRST)
	{}

	virtual bool MoveNext();
	virtual const TW_IDENTITY& Current();
	virtual void Reset();

	ICloneable* Clone() const
	{
		return new CTwainSourceEnumerator(SourceMgr);
	}

protected:
	CTwainSourceManager*	SourceMgr;
	TW_UINT16				MsgID;
	TW_IDENTITY				SourceID;
};