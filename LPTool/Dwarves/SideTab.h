#pragma once

interface ISideTab
{
public:
	virtual bool IsRelatedToMainView();
	virtual int  GetViewID();

	virtual int  GetValidityCounter();
	virtual int  SetValidityCounter(int counter);
	virtual int  ContentUpdate(int mainViewID, DwarfViewOperationContext* pCtx);
};
