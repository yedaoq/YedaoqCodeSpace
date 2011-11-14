#pragma once

struct DwarfViewOperationContext;

interface ISideTab
{
public:
	virtual bool IsRelatedToMainView() = 0;
	virtual int  GetViewID() = 0;

	virtual int  GetValidityCounter() = 0;
	virtual int  SetValidityCounter(int counter) = 0;
	virtual int  ContentUpdate(int mainViewID, DwarfViewOperationContext* pCtx) = 0;
};
