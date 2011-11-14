#pragma once
#include "SideTab.h"
#include <Layout\FlowLayout.h>
#include <GridCtrl.h>
#include <DBTableViewer4GridCtrl.h>

interface IDwarfViewInfo;

class CDwarfSideTab : public CWnd, public ISideTab
{
public:
	CDwarfSideTab(void);
	~CDwarfSideTab(void);

	enum { EIDC_GRID = 1 };

	virtual int  Initialize(CWnd* pParent, IDwarfViewInfo* pView);

	virtual bool IsRelatedToMainView();
	virtual int  GetViewID();

	virtual int  GetValidityCounter();
	virtual int  SetValidityCounter(int);
	virtual int  ContentUpdate(int mainViewID, DwarfViewOperationContext* pCtx);

protected:
	IDwarfViewInfo*				View;
	NSYedaoqLayout::CFlowLayout FlowLayoutMain;
	CGridCtrl					Grid;
	CDBTableViewer4GridCtrl		GridViewer;
	int							ValidityCounter;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
